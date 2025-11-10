#include "Logic.h"
#ifdef ARDUINO_ARCH_RP2040
    #include "FileTransferModule.h"
    #include "UsbExchangeModule.h"
#endif
#if defined(KNX_IP_LAN) || defined(KNX_IP_WIFI)
    #include "NetworkModule.h"
#endif
#include "OpenKNX.h"
#ifdef ARDUINO_ARCH_RP2040
    #pragma message "Pico Core Version: " ARDUINO_PICO_VERSION_STR
#endif

void setup()
{
    // change this also in library.json
    openknx.init();
    openknx.addModule(1, openknxLogic);
#ifdef ARDUINO_ARCH_RP2040
    openknx.addModule(9, openknxFileTransferModule);
    #ifndef OPENKNX_DEBUG
    openknx.addModule(8, openknxUsbExchangeModule);
    #endif
#endif
#if defined(KNX_IP_LAN) || defined(KNX_IP_WIFI)
    openknx.addModule(7, openknxNetwork);
#else
    openknx.unsupportedEtsModule(ETS_ModuleId_NET);
#endif
    openknx.setup();
}

void loop()
{
    openknx.loop();
}

#ifdef OPENKNX_DUALCORE
void setup1()
{
    openknx.setup1();
}

void loop1()
{
    openknx.loop1();
}
#endif