#include "Schedule.h"
#include "HardwareDevices.h"

Schedule::sCallbackParams Schedule::sCallbacks[SCHEDULE_MAX_CALLBACKS] = {};
uint8_t Schedule::sNumCallbacks = 0;

Schedule::Schedule()
{
}

Schedule::~Schedule()
{
}

// static
void Schedule::addCallback(loopCallback iCallback, void *iThis) 
{
    sCallbackParams lParams;
    lParams.callback = iCallback;
    lParams.instance = iThis;
    Schedule::sCallbacks[sNumCallbacks++] = lParams;
    if (sNumCallbacks >= SCHEDULE_MAX_CALLBACKS)
        fatalError(FATAL_SCHEDULE_MAX_CALLBACKS, "Schedule.cpp: Too many callbacks added");        
}


// static
void Schedule::loop() 
{
    static uint8_t sCountCallback = 0;
    uint8_t lCount = sCountCallback / 2;
    knx.loop();
    // we call submodules half as often as knx.loop();
    if (lCount * 2 == sCountCallback && lCount < sNumCallbacks && knx.configured())
    {
        if (!sCallbacks[lCount].called)
        {
            sCallbacks[lCount].called = true;
            sCallbacks[lCount].callback(sCallbacks[lCount].instance);
        }
    }
    if (lCount < sNumCallbacks)
    {
        sCountCallback += 1;
    }
    else
    {
        sCountCallback = 0;
        for (uint8_t i = 0; i < sNumCallbacks; i++)
            sCallbacks[i].called = false;
        
    }
}