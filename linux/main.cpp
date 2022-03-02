#include "knx/bau57B0.h"
#include "knx_facade.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

KnxFacade<LinuxPlatform, Bau57B0> knx;

void appSetup();
void appLoop();

int main(int argc, char **argv)
{
    knx.platform().cmdLineArgs(argc, argv);
    knx.readMemory();
    
    bool setProgmode = false;
    if (knx.induvidualAddress() == 0 || setProgmode)
        knx.progMode(true);

    appSetup();
    
    knx.start();

    while (1)
    {
        knx.loop();
        if(knx.configured())
            appLoop();
        delay(10);
    }
}