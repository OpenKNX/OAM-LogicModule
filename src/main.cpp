#include "Logic.h"
#ifdef ARDUINO_ARCH_RP2040
    #include "UpdaterModule.h"
#endif
#if defined(KNX_IP_W5500) || defined(KNX_IP_WIFI)
    #include "UsbExchangeModule.h"
    #include "NetworkModule.h"
#endif
#include "OpenKNX.h"
#ifdef ARDUINO_ARCH_RP2040
    #pragma message "Pico Core Version: " ARDUINO_PICO_VERSION_STR
#endif


void setup()
{
    const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, new Logic());
#ifdef ARDUINO_ARCH_RP2040
    openknx.addModule(2, new UpdaterModule());
    // openknx.addModule(9, openknxFileTransferModule);
#endif
#if defined(KNX_IP_W5500) || defined(KNX_IP_WIFI)
    openknx.addModule(7, openknxNetwork);
    openknx.addModule(8, openknxUsbExchangeModule);
#endif
    openknx.setup();
}

void loop()
{
    openknx.loop();
}
