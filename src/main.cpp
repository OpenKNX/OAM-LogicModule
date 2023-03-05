#include "Logic.h"
#include "OpenKNX.h"

void setup()
{
    const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new Logic());
    openknx.setup();
}

void loop()
{
    openknx.loop();
}