#pragma once
#include "LogicChannel.h"
#include "Timer.h"
#include "TimerRestore.h"

// Watchdog reset causees
#define WDT_RCAUSE_SYSTEM 6   // reset by system itself
#define WDT_RCAUSE_WDT 5      // reset by watchdog
#define WDT_RCAUSE_EXT 4      // reset by reset signal
#define WDT_RCAUSE_POR 0      // power on reset

typedef void (*loopCallback)(void *iThis);
struct sLoopCallbackParams {
    loopCallback callback;
    void *instance;
};

class Logic
{
  public:
    Logic();
    ~Logic();

    // static
    static void onInputKoHandler(GroupObject &iKo);
    static void onBeforeRestartHandler();
    static void onBeforeTablesUnloadHandler();
    static void onSafePinInterruptHandler();
    static char *initDiagnose(GroupObject &iKo);
    static char *getDiagnoseBuffer();
    static void addLoopCallback(loopCallback iLoopCallback, void *iThis);

    // instance
    EepromManager *getEEPROM();
    void writeAllInputsToEEPROMFacade();
    void processAllInternalInputs(LogicChannel *iChannel, bool iValue);
    void processReadRequests();
    void processInputKo(GroupObject &iKo);
    void processInterrupt(bool iForce = false);
    bool processDiagnoseCommand();
    void outputDiagnose(GroupObject &iKo);
    void debug();
    void setup(bool iSaveSupported);
    void loop();

  private:
    static uint8_t sMagicWord[];
    static Timer &sTimer;
    static TimerRestore &sTimerRestore;
    static char sDiagnoseBuffer[16];
    static sLoopCallbackParams sLoopCallbacks[5];
    static uint8_t sNumLoopCallbacks;

    LogicChannel *mChannel[LOG_ChannelsFirmware];
    uint8_t mNumChannels; // Number of channels defined in knxprod
    uint32_t mSaveInterruptTimestamp = 0;
    uint16_t mSaveInterruptCount = 0;

    uint32_t mLastWriteToEEPROM = 0;
    bool mIsValidEEPROM = false;
    EepromManager *mEEPROM;

    LogicChannel *getChannel(uint8_t iChannelId);
    uint8_t getChannelId(LogicChannel *iChannel);
    bool prepareChannels();

    void writeAllDptToEEPROM();
    void writeAllInputsToEEPROM();

    void onSavePinInterruptHandler();
    void beforeRestartHandler();
    void beforeTableUnloadHandler(TableObject &iTableObject, LoadState &iNewState);
    void processDiagnoseCommand(GroupObject &iKo);

    void processTime();
    void calculateSunriseSunset();
    void calculateEaster();

    void sendHoliday();
    void processTimerRestore();

    void loopSubmodules();
};
