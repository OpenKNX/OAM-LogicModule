#ifdef LOGICMODULE
#include "Helper.h"
#include "Hardware.h"
#include "Logic.h"

const uint8_t cFirmwareMajor = 3;    // 0-31
const uint8_t cFirmwareMinor = 8;    // 0-31
const uint8_t cFirmwareRevision = 0; // 0-63

struct sRuntimeInfo
{
    unsigned long startupDelay;
    unsigned long heartbeatDelay;
};

sRuntimeInfo gRuntimeData;
Logic gLogic;

void ProcessHeartbeat()
{
    // the first heartbeat is send directly after startup delay of the device
    if (gRuntimeData.heartbeatDelay == 0 || delayCheck(gRuntimeData.heartbeatDelay, knx.paramInt(LOG_Heartbeat) * 1000))
    {
        // we waited enough, let's send a heartbeat signal
        knx.getGroupObject(LOG_KoHeartbeat).value(true, getDPT(VAL_DPT_1));
        gRuntimeData.heartbeatDelay = millis();
        // debug-helper for logic module
        gLogic.debug();
    }
}

void ProcessReadRequests() {
    // this method is called after startup delay and executes read requests, which should just happen once after startup
    static bool sCalled = false;
    if (!sCalled) {
        gLogic.processReadRequests();
        sCalled = true;
    }
}

// true solgange der Start des gesamten Moduls verzögert werden soll
bool startupDelay()
{
    return !delayCheck(gRuntimeData.startupDelay, knx.paramInt(LOG_StartupDelay) * 1000);
}

void appLoop()
{
    if (!knx.configured())
        return;

    // handle KNX stuff
    if (startupDelay())
        return;

    // at this point startup-delay is done
    // we process heartbeat
    ProcessHeartbeat();
    ProcessReadRequests();
    gLogic.loop();
}

void appSetup(bool iSaveSupported)
{
    // try to get rid of occasional I2C lock...
    // savePower();
    ledProg(true);
    ledInfo(true);
    // delay(100);
    // restorePower();
    // check hardware availability
    boardCheck();
    ledInfo(false);
    ledProg(false);

    if (knx.configured())
    {
        // 5 bit major, 5 bit minor, 6 bit revision
        // knx.bau().deviceObject().version(cFirmwareMajor << 11 | cFirmwareMinor << 6 | cFirmwareRevision);
        gRuntimeData.startupDelay = millis();
        gRuntimeData.heartbeatDelay = 0;
        gLogic.setup(iSaveSupported);
    }
}
#endif