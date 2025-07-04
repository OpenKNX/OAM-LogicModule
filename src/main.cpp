#include "Logic.h"
#ifdef ARDUINO_ARCH_RP2040
    #include "FileTransferModule.h"
    #include "UsbExchangeModule.h"
    #if defined(KNX_IP_LAN) || defined(KNX_IP_WIFI)
        #include "NetworkModule.h"
    #endif
#endif
#include "OpenKNX.h"
#ifdef ARDUINO_ARCH_RP2040
    #pragma message "Pico Core Version: " ARDUINO_PICO_VERSION_STR
#endif

void setup()
{
    // change this also in library.json
    const uint8_t firmwareRevision = 0;
    openknx.init(firmwareRevision);
    openknx.addModule(1, openknxLogic);
#ifdef ARDUINO_ARCH_RP2040
    // openknx.addModule(2, new UpdaterModule());
    openknx.addModule(9, openknxFileTransferModule);
    openknx.addModule(8, openknxUsbExchangeModule);
    #if defined(KNX_IP_LAN) || defined(KNX_IP_WIFI)
    openknx.addModule(7, openknxNetwork);
    #endif
#endif
    openknx.setup();
}

void loop()
{
    openknx.loop();
}
