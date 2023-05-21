#include "oknx.h"
#include "Logic.h"
#include "Helper.h"
#include "Timer.h"
#include "TimerRestore.h"
#include "PCA9632.h"
#include "MemoryFree.h"
#include "SmartMF.h"
#ifdef WATCHDOG
#include <Adafruit_SleepyDog.h>
uint32_t gWatchdogDelay;
uint8_t gWatchdogResetCause;
#endif


uint8_t Logic::sMagicWord[] = {0xAE, 0x49, 0xD2, 0x9F};
Timer &Logic::sTimer = Timer::instance(); // singleton
TimerRestore &Logic::sTimerRestore = TimerRestore::instance(); // singleton

char Logic::sDiagnoseBuffer[16] = {0};
sLoopCallbackParams Logic::sLoopCallbacks[5] = {nullptr};
uint8_t Logic::sNumLoopCallbacks = 0;

// callbacks have to be static members
void Logic::onInputKoHandler(GroupObject &iKo) {
    LogicChannel::sLogic->processInputKo(iKo);
}

void Logic::addLoopCallback(loopCallback iLoopCallback, void *iThis) {
    sLoopCallbackParams lParams;
    lParams.callback = iLoopCallback;
    lParams.instance = iThis;
    Logic::sLoopCallbacks[sNumLoopCallbacks++] = lParams;
}

// implement IFlashUserData
const uint8_t *Logic::restore(const uint8_t *iBuffer)
{
    return LogicChannel::sLogic->loadFromFlash(iBuffer);
}

uint8_t *Logic::save(uint8_t *iBuffer)
{
    return LogicChannel::sLogic->saveToFlash(iBuffer); 
}

uint16_t Logic::saveSize() 
{
    return 1004; // just the size for logic
}

bool Logic::powerOn()
{
    return true;
}

const char* Logic::name()
{
    return "LogicModule";
}

// end of IFlashUserData

Logic::Logic()
{
    LogicChannel::sLogic = this;
}

Logic::~Logic()
{
}

LogicChannel *Logic::getChannel(uint8_t iChannelId) {
    return mChannel[iChannelId];
}

uint8_t Logic::getChannelId(LogicChannel *iChannel) {
    uint8_t lResult = -1;
    for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
    {
        if (mChannel[lIndex] == iChannel) {
            lResult = lIndex;
            break;
        }
    }
    return lResult;
}

void Logic::addKoLookup(uint16_t iKoNumber, uint8_t iChannelId, uint8_t iIOIndex) 
{
    // first implementation, in future we use sorted insert
    mKoLookup[mNumKoLookups].koNumber = iKoNumber;
    mKoLookup[mNumKoLookups].channelIndex = iChannelId;
    mKoLookup[mNumKoLookups].ioIndex = iIOIndex;
    if (mNumKoLookups < cCountKoLookups)
        mNumKoLookups++;
}

bool Logic::getKoLookup(uint16_t iKoNumber, sKoLookup **iKoLookup) 
{
    sKoLookup *lIterator = *iKoLookup;
    if (*iKoLookup == 0)
        lIterator = &mKoLookup[0];
    else
        lIterator++;
    while (lIterator->koNumber > 0)
    {
        if (lIterator->koNumber == iKoNumber)
        {
            *iKoLookup = lIterator;
            return true;
        }
        else
        {
            lIterator++;
        }
    }
    return false;
}

bool Logic::prepareChannels() {
    bool lResult = false;
    printDebug("prepareChannels called...\n");
    for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
    {
        // Important: lResult has to be the last argument in this OR, 
        // otherwise prepareChannel might be not called
        lResult = mChannel[lIndex]->prepareChannel() || lResult;
    }
    return lResult;
}

// we trigger all associated internal inputs with the new value
void Logic::processAllInternalInputs(LogicChannel *iChannel, bool iValue)
{
    // search for any internal input associated to this channel
    for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
    {
        LogicChannel *lChannel = mChannel[lIndex];
        uint8_t lChannelId = getChannelId(iChannel);
        lChannel->processInternalInputs(lChannelId, iValue);
    }
}

void Logic::processReadRequests() {
    static bool sLogicProcessReadRequestsCalled = false;
    static uint32_t sDelay = 19000;

    // the following code should be called only once after initial startup delay 
    if (!sLogicProcessReadRequestsCalled) {
        if (knx.paramByte(LOG_VacationRead) & LOG_VacationReadMask)
        {
            knx.getGroupObject(LOG_KoVacation).requestObjectRead();
        }
        // we put this here, because all other places are too early, knx stack is not up to date to send according init values
        // got from flash storage
        prepareChannels();
        sLogicProcessReadRequestsCalled = true;
    }
    // date and time are red from bus every 30 seconds until a response is received
    if ((knx.paramByte(LOG_ReadTimeDate) & LOG_ReadTimeDateMask))
    {
        eTimeValid lValid = sTimer.isTimerValid();
        if (delayCheck(sDelay, 30000) && lValid != tmValid)
        {
            sDelay = millis();
            if (knx.paramByte(LOG_CombinedTimeDate) & LOG_CombinedTimeDateMask) {
                // combined date and time
                knx.getGroupObject(LOG_KoTime).requestObjectRead();
            } else {
                // date and time from separate KOs
                if (lValid != tmMinutesValid)
                    knx.getGroupObject(LOG_KoTime).requestObjectRead();
                if (lValid != tmDateValid)
                    knx.getGroupObject(LOG_KoDate).requestObjectRead();
            }
        }
        // if date and/or time is known, we read also summertime information
        if (sDelay > 0 && lValid == tmValid)
        {
            sDelay = 0;
            knx.getGroupObject(LOG_KoIsSummertime).requestObjectRead();
        }
    }
}

// For inputs, which are not set as "store in memory", we write a dpt 0xFF
uint8_t *Logic::writeAllDptToFlash(uint8_t *iBuffer)
{
    if (iBuffer && knx.configured())
    {
        printDebug("Writing DPTs to Flash\n");
        for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
        {
            iBuffer = mChannel[lIndex]->writeSingleDptToFlash(IO_Input1, iBuffer);
            iBuffer = mChannel[lIndex]->writeSingleDptToFlash(IO_Input2, iBuffer);
        }
        printDebug("\n");
    }
    return iBuffer;
}

uint8_t *Logic::writeAllInputsToFlash(uint8_t *iBuffer)
{
    if (knx.configured())
    {
        println("Logic: Writing KOs to Flash");
        for (uint8_t lChannel = 0; lChannel < mNumChannels; lChannel++)
        {
            GroupObject *lKo = LogicChannel::getKoForChannel(IO_Input1, lChannel);
            for (uint8_t i = 0; i < 4; i++)
                *iBuffer++ = (i < lKo->valueSize()) ? lKo->valueRef()[i] : 0;
            lKo = LogicChannel::getKoForChannel(IO_Input2, lChannel);
            for (uint8_t i = 0; i < 4; i++)
                *iBuffer++ = (i < lKo->valueSize()) ? lKo->valueRef()[i] : 0;
        }
    }
    return iBuffer;
}

const uint8_t *Logic::loadFromFlash(const uint8_t *iBuffer)
{
    bool lValid = true;
    SERIAL_DEBUG.println("Logic: Reading state from Flash");
    // read magic word
    for (uint8_t i = 0; i < 4 && lValid; i++)
    {
        // printDebug("%02X ", iBuffer[i]);
        lValid = lValid && (iBuffer[i] == sMagicWord[i]);
    }

    mFlashBuffer = lValid ? iBuffer : nullptr;

    if (!lValid)
    {
        println("No valid data in buffer");
    }
    return iBuffer + saveSize();
}

uint8_t *Logic::saveToFlash(uint8_t *iBuffer)
{
    if (knx.configured())
    {
        // write magic word
        for (uint8_t i = 0; i < 4; i++)
        {
            iBuffer[i] = sMagicWord[i];
            // printDebug("%02X ", iBuffer[i]);
        }
        println();
        // we support max 100 channels
        // - first 100 Bytes are reserved for DPT information
        // - starting at 100, all input KO values are stored
        //     - each KO has 4 bytes
        //     - each channel has 2 Inputs
        //     - so we have 8 bytes per channel, 800 byte at all
        writeAllDptToFlash(iBuffer + USERDATA_DPT_OFFSET);
        writeAllInputsToFlash(iBuffer + USERDATA_KO_OFFSET);
    }
    else if (mFlashBuffer != nullptr)
    {
        println("Logic: Restore old KOs to Flash");
        // in case knx is not configured but there are old values, we copy them back to buffer
        for (size_t i = 0; i < saveSize(); i++)
            iBuffer[i] = mFlashBuffer[i];
        
    }
    return iBuffer + saveSize(); // we always return a fixed size!
}

// on input level, all dpt > 1 values are converted to bool by the according converter
void Logic::processInputKo(GroupObject &iKo)
{
    // we have to check first, if external KO are used
    sKoLookup *lKoLookup = nullptr;
    while (getKoLookup(iKo.asap(), &lKoLookup)) 
    {
        LogicChannel *lChannel = mChannel[lKoLookup->channelIndex];
        lChannel->processInput(lKoLookup->ioIndex);
    }
    if (iKo.asap() == LOG_KoTime) {
        if (knx.paramByte(LOG_CombinedTimeDate) & LOG_CombinedTimeDateMask) {
            KNXValue value = "";

            // first ensure we have a valid data-time content
            // (including the correct length)
            if (iKo.tryValue(value, getDPT(VAL_DPT_19))) {

                // use raw value, as current version of knx do not provide access to all fields
                // TODO DPT19: check integration of extended DPT19 access into knx or OpenKNX-Commons
                // size is ensured to be 8 Byte
                uint8_t *raw = iKo.valueRef();

                /*
                const bool flagFault = raw[6] & 0x80;
                // ignore working day (WD, NWD): raw[6] & 0x40, raw[6] & 0x20
                const bool flagNoYear = raw[6] & 0x10;
                const bool flagNoDate = raw[6] & 0x08;
                // ignore NDOW: raw[6] & 0x04
                const bool flagNoTime = raw[6] & 0x02;
                const bool flagSuti = raw[6] & 0x01;
                // ignore quality of clock (CLQ): raw[7] & 0x80
                // ignore synchronisation source reliablity (SRC): raw[7] & 0x40
                */

                // ignore inputs with:
                // * F - fault
                // * NY - missing year
                // * ND - missing date
                // * NT - missing time
                if (!(raw[6] & (DPT19_FAULT | DPT19_NO_YEAR | DPT19_NO_DATE | DPT19_NO_TIME))) {
                    struct tm lTmp = value;
                    sTimer.setDateTimeFromBus(&lTmp);
                    const bool lSummertime = raw[6] & DPT19_SUMMERTIME;
                    if (((knx.paramByte(LOG_SummertimeAll) & LOG_SummertimeAllMask) >> LOG_SummertimeAllShift) == VAL_STIM_FROM_DPT19)
                        sTimer.setIsSummertime(lSummertime);
                }
            }
        } else {
            KNXValue value = "";
            // ensure we have a valid time content
            if (iKo.tryValue(value, getDPT(VAL_DPT_10))) {
                struct tm lTmp = value;
                sTimer.setTimeFromBus(&lTmp);
            }   
        }
    } else if (iKo.asap() == LOG_KoDate) {
        KNXValue value = "";
        // ensure we have a valid date content
        if (iKo.tryValue(value, getDPT(VAL_DPT_11))) {
            struct tm lTmp = value;
            sTimer.setDateFromBus(&lTmp);
        }
    } else if (iKo.asap() == LOG_KoIsSummertime) {
        sTimer.setIsSummertime(iKo.value(getDPT(VAL_DPT_1)));
    } else if (iKo.asap() == LOG_KoDiagnose) {
        processDiagnoseCommand(iKo);
    }
#ifdef BUZZER_PIN
    else if (iKo.asap() == LOG_KoBuzzerLock) {
        // turn off buzzer in case of lock
        if (iKo.value(getDPT(VAL_DPT_1)))
            noTone(BUZZER_PIN);
    }
#endif
#ifdef I2C_RGBLED_DEVICE_ADDRESS
    else if (iKo.asap() == LOG_KoLedLock)
    {
        // turn off LED in case of lock
        if (iKo.value(getDPT(VAL_DPT_1)))
            PCA9632_SetColor(0, 0, 0);
    }
#endif
    else if (iKo.asap() >= LOG_KoOffset + LOG_KoKOfE1 && iKo.asap() < LOG_KoOffset + LOG_KoKOfE1 + mNumChannels * LOG_KoBlockSize)
    {
        uint16_t lKoNumber = iKo.asap() - LOG_KoOffset - LOG_KoKOfE1;
        uint8_t lChannelId = lKoNumber / LOG_KoBlockSize;
        uint8_t lIOIndex = lKoNumber % LOG_KoBlockSize + 1;
        LogicChannel *lChannel = mChannel[lChannelId];
        lChannel->processInput(lIOIndex);
    }
}

char *Logic::initDiagnose(GroupObject &iKo) {
    memcpy(sDiagnoseBuffer, iKo.valueRef(), 14);
    return sDiagnoseBuffer;
}

char *Logic::getDiagnoseBuffer() {
    return sDiagnoseBuffer;
}

bool Logic::processDiagnoseCommand() {
    bool lResult = false;
    //diagnose is interactive and reacts on commands
    switch (sDiagnoseBuffer[0]) {
        case 'l': {
            // Command l<nn>: Logic inputs and output of last execution
            // find channel and dispatch
            uint8_t lIndex = (sDiagnoseBuffer[1] - '0') * 10 + sDiagnoseBuffer[2] - '0' - 1;
            if (lIndex < LOG_ChannelCount) {
                lResult = mChannel[lIndex]->processDiagnoseCommand(sDiagnoseBuffer);
            } else {
                // ignore invalid channel
                lResult = false;
            }
            break;
        }
        case 't': {
            // return internal time (might differ from external
            uint8_t lHour = sTimer.getHour();
            uint8_t lMinute = sTimer.getMinute();
            uint8_t lSecond = sTimer.getSecond();
            uint8_t lDay = sTimer.getDay();
            uint8_t lMonth = sTimer.getMonth();
            // this if prevents stupid warnings
            if (lHour < 24 && lMinute < 60 && lSecond < 60 && lDay < 32 && lMonth < 13)
                snprintf(sDiagnoseBuffer, 15, "%02d:%02d:%02d %02d.%02d", lHour, lMinute, lSecond, lDay, lMonth);
            lResult = true;
            break;
        }
        case 'r': {
            if (sDiagnoseBuffer[1] == 'e')
            {
                // return sunrise and sunset for a specific elevation teSDD,
                // where S=Sign(+,-) and DD ist elevation in degree
                if (sDiagnoseBuffer[2] == '-' || sDiagnoseBuffer[2] == '+')
                {
                    double lDegree = ((sDiagnoseBuffer[3] - '0') * 10 + sDiagnoseBuffer[4] - '0');
                    uint8_t lMinute = ((sDiagnoseBuffer[5] - '0') * 10 + sDiagnoseBuffer[6] - '0'); 
                    lDegree = (lDegree + lMinute / 60.0) * (sDiagnoseBuffer[2] == '+' ? 1 : -1);
                    sTime lSunrise;
                    sTime lSunset;
                    sTimer.getSunDegree(SUN_SUNRISE, lDegree, &lSunrise);
                    sTimer.getSunDegree(SUN_SUNSET, lDegree, &lSunset);
                    // this if prevents stupid warnings
                    if (lSunrise.hour >= 0 && lSunrise.hour < 24 && lSunrise.minute >= 0 && lSunrise.minute < 60 && lSunset.hour >= 0 && lSunset.hour < 24 && lSunset.minute >= 0 && lSunset.minute < 60)
                        snprintf(sDiagnoseBuffer, 15, "R%02d:%02d S%02d:%02d", lSunrise.hour, lSunrise.minute, lSunset.hour, lSunset.minute);
                }
                else
                {
                    snprintf(sDiagnoseBuffer, 15, "TRY re-0600");
                }
                lResult = true;
            }
            else
            {
                // return sunrise and sunset
                sTime *lSunrise = sTimer.getSunInfo(SUN_SUNRISE);
                sTime *lSunset = sTimer.getSunInfo(SUN_SUNSET);
                // this if prevents stupid warnings
                if (lSunrise->hour >= 0 && lSunrise->hour < 24 && lSunrise->minute >= 0 && lSunrise->minute < 60 && lSunset->hour >= 0 && lSunset->hour < 24 && lSunset->minute >= 0 && lSunset->minute < 60)
                    snprintf(sDiagnoseBuffer, 15, "R%02d:%02d S%02d:%02d", lSunrise->hour, lSunrise->minute, lSunset->hour, lSunset->minute);
                lResult = true;
            }
            break;
        }
        case 'o': {
            // calculate easter date
            snprintf(sDiagnoseBuffer, 15, "O%02d.%02d", sTimer.getEaster()->day, sTimer.getEaster()->month);
            lResult = true;
            break;
        }
        case 'm': {
            snprintf(sDiagnoseBuffer, 15, "%i", freeMemory());
            lResult = true;
            break;
        }
        case 'w': {
            // Watchdog information
#ifdef WATCHDOG
            if (((knx.paramByte(LOG_Watchdog) & LOG_WatchdogMask) >> LOG_WatchdogShift) == 0)
            {
                snprintf(sDiagnoseBuffer, 15, "WD not active");
            }
            else if (gWatchdogResetCause & WDT_RCAUSE_EXT)
            {
                snprintf(sDiagnoseBuffer, 15, "WD reset button");
            }
            else if (gWatchdogResetCause & WDT_RCAUSE_POR)
            {
                snprintf(sDiagnoseBuffer, 15, "WD bus reset");
            }
            else if (gWatchdogResetCause & WDT_RCAUSE_SYSTEM)
            {
                snprintf(sDiagnoseBuffer, 15, "WD ETS program");
            }
            else if (gWatchdogResetCause & WDT_RCAUSE_WDT)
            {
                snprintf(sDiagnoseBuffer, 15, "WD watchdog");
            }
            else
            {
                snprintf(sDiagnoseBuffer, 15, "WD unknown");
            }
#else
            snprintf(sDiagnoseBuffer, 15, "WD no compile");
#endif
            lResult = true;
            break;
        }
        default:
            lResult = false;
            break;
    }
    return lResult;
}

void Logic::processDiagnoseCommand(GroupObject &iKo) {
    // this method is called as soon as iKo is changed
    // an external change is expected
    // because this iKo also is changed within this method,
    // the method is called again. This might result in
    // an endless loop. This is prevented by the isCalled pattern.
    static bool sIsCalled = false;
    if (!sIsCalled)
    {
        sIsCalled = true;
        //diagnose is interactive and reacts on commands
        initDiagnose(iKo);
        if (processDiagnoseCommand())
            outputDiagnose(iKo);
        sIsCalled = false;
    }
};

void Logic::outputDiagnose(GroupObject &iKo) {
    sDiagnoseBuffer[15] = 0;
    iKo.value(sDiagnoseBuffer, getDPT(VAL_DPT_16));
    printDebug("Diagnose: %s\n", sDiagnoseBuffer);
}

void Logic::debug() {
    printDebug("Logik-LOG_ChannelsFirmware (in Firmware): %d\n", LOG_ChannelsFirmware);
    printDebug("Logik-gNumChannels (in knxprod):  %d\n", mNumChannels);

    // printDebug("Aktuelle Zeit: %s", sTimer.getTimeAsc());
    sTimer.debug();
#ifdef ARDUINO_ARCH_RP2040
    printDebug("Free Heap: %i\n", rp2040.getFreeHeap());
#else
    printDebug("Free Memory: %i\n", freeMemory());
#endif
}

void Logic::setup(bool iSaveSupported) {
    // Wire.end();   // seems to end hangs on I2C bus
    // Wire.begin(); // we use I2C in logic, so we setup the bus. It is not critical to setup it more than once
#ifdef WATCHDOG
    if ((knx.paramByte(LOG_Watchdog) & LOG_WatchdogMask) >> LOG_WatchdogShift) {
        // used for Diagnose command
        gWatchdogResetCause = Watchdog.resetCause();
        // setup watchdog to prevent endless loops
        int lWatchTime = Watchdog.enable(16384, false);
        printDebug("Watchdog started with a watchtime of %i Seconds\n", lWatchTime / 1000);
    }
#endif
    if (knx.configured())
    {
        // check for hidden parameters
        printDebug("Setting: Buzzer available: %d\n", (bool)(knx.paramByte(LOG_BuzzerInstalled) & LOG_BuzzerInstalledMask));
        printDebug("Setting: RGBLed available: %d\n", (bool)(knx.paramByte(LOG_LedInstalled) & LOG_LedInstalledMask));
        // setup channels, not possible in constructor, because knx is not configured there
        // get number of channels from knxprod
        mNumChannels = knx.paramByte(LOG_NumChannels);
        if (LOG_ChannelsFirmware < mNumChannels)
        {
            char lErrorText[80];
            sprintf(lErrorText, "FATAL: Firmware compiled for %d channels, but knxprod needs %d channels!\n", LOG_ChannelsFirmware, mNumChannels);
            fatalError(FATAL_LOG_WRONG_CHANNEL_COUNT, lErrorText);
        }
        smartMF.init();
        for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
        {
            mChannel[lIndex] = new LogicChannel(lIndex);
        }
        // setup buzzer
#ifdef BUZZER_PIN
        pinMode(BUZZER_PIN, OUTPUT);
#endif
        // we set just a callback if it is not set from a potential caller
        if (GroupObject::classCallback() == 0) GroupObject::classCallback(Logic::onInputKoHandler);
        // we store some input values in case of restart or ets programming
        if (iSaveSupported) openknx.flashUserData()->first(this);

        // prepareChannels();
        float lLat = LogicChannel::getFloat(knx.paramData(LOG_Latitude));
        float lLon = LogicChannel::getFloat(knx.paramData(LOG_Longitude));
        // sTimer.setup(8.639751, 49.310209, 1, true, 0xFFFFFFFF);
        bool lTimezoneSign = (knx.paramByte(LOG_TimezoneSign) & LOG_TimezoneSignMask) >> LOG_TimezoneSignShift;
        int8_t lTimezone = (knx.paramByte(LOG_TimezoneValue) & LOG_TimezoneValueMask) >> LOG_TimezoneValueShift;
        lTimezone = lTimezone * (lTimezoneSign ? -1 : 1);
        bool lUseSummertime = (((knx.paramByte(LOG_SummertimeAll) & LOG_SummertimeAllMask) >> LOG_SummertimeAllShift) == VAL_STIM_FROM_INTERN);
        sTimer.setup(lLon, lLat, lTimezone, lUseSummertime, knx.paramInt(LOG_Neujahr));
        // for TimerRestore we prepare all Timer channels
        for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
        {
            LogicChannel *lChannel = mChannel[lIndex];
            lChannel->startTimerRestoreState();
        }
    }
}

void Logic::loop()
{
    // static uint32_t sLogicLoopTime;

    if (!knx.configured())
        return;
#ifdef WATCHDOG
    if (delayCheck(gWatchdogDelay, 1000) && ((knx.paramByte(LOG_Watchdog) & LOG_WatchdogMask) >> LOG_WatchdogShift))
    {
        Watchdog.reset();
        gWatchdogDelay = millis();
    }
#endif

    // sLogicLoopTime = millis();
    sTimer.loop(); // clock and timer async methods
    // printDebug("sTimer.loop() takes: %i\n", millis() - sLogicLoopTime);
    // sLogicLoopTime = millis();
    loopSubmodules();
    // printDebug("loopSubmodules() takes: %i\n", millis() - sLogicLoopTime);

    // sLogicLoopTime = millis();
    // we loop on all channels and execute pipeline
    for (uint8_t lIndex = 0; lIndex < mNumChannels && knx.configured(); lIndex++)
    {
        LogicChannel *lChannel = mChannel[lIndex];
        if (sTimer.minuteChanged())
            lChannel->startTimerInput();
        lChannel->loop();
        // loopSubmodules(); // this leads to performance critical loop duration!!!
    }
    // printDebug("channelLoop() takes: %i\n", millis() - sLogicLoopTime);
    if (sTimer.minuteChanged() && knx.configured()) {
        // sLogicLoopTime = millis();
        sendHoliday();
        sTimer.clearMinuteChanged();
        loopSubmodules();
        // printDebug("HolidayLoop() takes: %i\n", millis() - sLogicLoopTime);
    }
    // sLogicLoopTime = millis();
    processTimerRestore();
    // printDebug("TimerRestore() takes: %i\n", millis() - sLogicLoopTime);

}

const uint8_t *Logic::getFlash() 
{
    return mFlashBuffer;
}

// start timer implementation
void Logic::processTimerRestore() {
    static uint32_t sTimerRestoreDelay = 1;
    if (!knx.configured())
        return;
        
    if (sTimerRestoreDelay == 0)
        return;
    if (sTimer.isTimerValid() == tmValid && delayCheck(sTimerRestoreDelay, 500)) {
        sTimerRestoreDelay = millis();
        if (sTimerRestoreDelay == 0)
            sTimerRestoreDelay = 1; // prevent set to 0 in case of timer overflow
        if (sTimerRestore.getDayIteration() < 365) {
            if (sTimerRestore.getDayIteration() == 0)
            {
                // initialize RestoreTimer
                sTimerRestore.setup(sTimer);
            }
            else
            {
                sTimerRestore.decreaseDay();
            }
            loopSubmodules();
        } else {
            // stop timer restore processing in logic...
            sTimerRestoreDelay = 0;
            // ... and in each channel
            for (uint8_t lIndex = 0; lIndex < mNumChannels; lIndex++)
            {
                LogicChannel *lChannel = mChannel[lIndex];
                lChannel->stopTimerRestoreState();
            }
        }
    }
}

// send holiday information on bus
void Logic::sendHoliday() {
    if (sTimer.holidayChanged())
    {
        // write the newly calculated holiday information into KO (can be read externally)
        knx.getGroupObject(LOG_KoHoliday1).valueNoSend(sTimer.holidayToday(), getDPT(VAL_DPT_5));
        knx.getGroupObject(LOG_KoHoliday2).valueNoSend(sTimer.holidayTomorrow(), getDPT(VAL_DPT_5));
        sTimer.clearHolidayChanged();
        if (knx.paramByte(LOG_HolidaySend) & LOG_HolidaySendMask) {
            // and send it, if requested by application setting
            knx.getGroupObject(LOG_KoHoliday1).objectWritten();
            knx.getGroupObject(LOG_KoHoliday2).objectWritten();
        }
    }
}

void Logic::loopSubmodules() {
    static uint8_t sCount = 0;
    uint8_t lCount = sCount / 2;
    openknx.loop();
    // we call submodules half as often as knx.loop();
    if (lCount * 2 == sCount && lCount < sNumLoopCallbacks && knx.configured())
    {
        sLoopCallbacks[lCount].callback(sLoopCallbacks[lCount].instance);
    }
    sCount = (lCount < sNumLoopCallbacks) ? sCount + 1 : 0;
    // for (uint8_t i = 0; i < sNumLoopCallbacks; i++)
    // {
    //     sLoopCallbacks[i].callback(sLoopCallbacks[i].instance);
    //     knx.loop();
    // }
}