#include "Logic.h"
#ifdef ARDUINO_ARCH_RP2040
    #include "FileTransferModule.h"
    #if defined(KNX_IP_GENERIC)
        #include "NetworkModule.h"
        #include "UsbExchangeModule.h"
    #endif
#endif
#include "OpenKNX.h"
#ifdef ARDUINO_ARCH_RP2040
    #pragma message "Pico Core Version: " ARDUINO_PICO_VERSION_STR
#endif

void setup()
{
    const uint8_t firmwareRevision = 3;
    openknx.init(firmwareRevision);
    openknx.addModule(1, openknxLogic);
#ifdef ARDUINO_ARCH_RP2040
    // openknx.addModule(2, new UpdaterModule());
    openknx.addModule(9, openknxFileTransferModule);
    #if defined(KNX_IP_GENERIC)
    openknx.addModule(7, openknxNetwork);
    openknx.addModule(8, openknxUsbExchangeModule);
    #endif
#endif
    openknx.setup();
}

void loop()
{
    openknx.loop();
}
