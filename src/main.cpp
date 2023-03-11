#include "Logic.h"
#include "OpenKNX.h"
#ifdef ARDUINO_ARCH_RP2040
#pragma message "Pico Core Version: " ARDUINO_PICO_VERSION_STR 
#endif

void setup()
{
    const uint8_t firmwareRevision = 3;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new Logic());
    openknx.setup();
}

void loop()
{
    openknx.loop();
}