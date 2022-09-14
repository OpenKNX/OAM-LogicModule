// implements a simple static scheduler
// for cooperative calls to loop() functions of modules
#include <knx.h>

#define SCHEDULE_MAX_CALLBACKS 10

class Schedule
{
public:
    typedef void (*loopCallback)(void *iThis);

    static void addCallback(loopCallback iCallback, void *iThis);
    static void loop();

private:
    struct sCallbackParams {
        loopCallback callback;
        void *instance = nullptr;
        bool called = false;
    };

    // static class, no instance necessary
    Schedule();
    ~Schedule();

    static sCallbackParams sCallbacks[SCHEDULE_MAX_CALLBACKS];
    static uint8_t sNumCallbacks;

};
