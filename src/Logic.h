#pragma once
#include "OpenKNX.h"
#include "LogicChannel.h"
#include "Timer.h"
#include "TimerRestore.h"
// #include "IFlashUserData.h"

#define USERDATA_MAGIC_OFFSET 0 // start of magic word in flash
#define USERDATA_DPT_OFFSET 4   // start of DPT storage in flash
#define USERDATA_KO_OFFSET 204  // start of KO values in flash (2 KO per channel, 4 Byte per KO)

// Watchdog reset causes
#define WDT_RCAUSE_SYSTEM 6   // reset by system itself
#define WDT_RCAUSE_WDT 5      // reset by watchdog
#define WDT_RCAUSE_EXT 4      // reset by reset signal
#define WDT_RCAUSE_POR 0      // power on reset

// DPT19 special flags
#define DPT19_FAULT 0x80
#define DPT19_WORKING_DAY 0x40
#define DPT19_NO_WORKING_DAY 0x20
#define DPT19_NO_YEAR 0x10
#define DPT19_NO_DATE 0x08
#define DPT19_NO_DAY_OF_WEEK 0x04
#define DPT19_NO_TIME 0x02
#define DPT19_SUMMERTIME 0x01

// Values for Summertime
#define VAL_STIM_FROM_KO 0
#define VAL_STIM_FROM_DPT19 1
#define VAL_STIM_FROM_INTERN 2


// typedef void (*loopCallback)(void *iThis);
// struct sLoopCallbackParams {
//     loopCallback callback;
//     void *instance;
// };

struct sKoLookup
{
    uint16_t koNumber;
    uint8_t channelIndex;
    uint8_t ioIndex;
};

class Logic : public OpenKNX::Module
{
  public:
    Logic();
    ~Logic();

    // static
    //static void onInputKoHandler(GroupObject &iKo);
    static char *initDiagnose(GroupObject &iKo);
    static char *getDiagnoseBuffer();
    // static void addLoopCallback(loopCallback iLoopCallback, void *iThis);

    // instance
    void addKoLookup(uint16_t iKoNumber, uint8_t iChannelId, uint8_t iIOIndex);
    bool getKoLookup(uint16_t iKoNumber, sKoLookup **iKoLookup = nullptr);

    //const uint8_t *getFlash();
    // void writeAllInputsToEEPROMFacade();
    void processAllInternalInputs(LogicChannel *iChannel, bool iValue);
    void processAfterStartupDelay();
    void processReadRequests();
    void processInputKo(GroupObject &iKo);
    bool processDiagnoseCommand();
    void outputDiagnose(GroupObject &iKo);
    void debug();
    void setup();
    void loop();
    void readFlash(const uint8_t* iBuffer, const uint16_t iSize) override;
    void writeFlash() override;
    uint16_t flashSize() override;
    const char* name() override;

  private:
    static uint8_t sMagicWord[];
    static Timer &sTimer;
    static TimerRestore &sTimerRestore;
    static char sDiagnoseBuffer[16];
    // static sLoopCallbackParams sLoopCallbacks[5];
    // static uint8_t sNumLoopCallbacks;

    LogicChannel *mChannel[LOG_ChannelsFirmware];
    uint8_t mNumChannels; // Number of channels defined in knxprod

    // we need a lookup for external KO
    static const uint16_t cCountKoLookups = LOG_ChannelsFirmware * 3;
    sKoLookup mKoLookup[cCountKoLookups]; // max 3*4*100 = 1200 Byte, too much?
    uint16_t mNumKoLookups = 0;

    //const uint8_t *mFlashBuffer = nullptr; // Pointer to stored flash content

    LogicChannel *getChannel(uint8_t iChannelId);
    uint8_t getChannelId(LogicChannel *iChannel);
    void prepareChannels();

    void processDiagnoseCommand(GroupObject &iKo);

    void sendHoliday();
    void processTimerRestore();

    // void loopSubmodules();
};
