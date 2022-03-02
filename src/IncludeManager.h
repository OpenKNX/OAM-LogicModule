#pragma once
#define STR(x) #x
#define XSTR(x) STR(x)

#ifdef LOGICMODULE
#pragma message "Building Logic for LOGICMODULE"
#include "Logikmodul.h"
#endif
#ifdef SENSORMODULE
#pragma message "Building Logic for SENSORMODULE"
#include "../../knx-sensor/src/Sensormodul.h"
#endif
#ifdef PMMODULE
#pragma message "Building Logic for PMMODULE"
#include "../../knx-pm/src/PMmodul.h"
#endif
#ifdef WIREGATEWAY
#pragma message "Building Logic for WIREGATEWAY"
#include "../../knx-wire/src/WireGateway.h"
#endif
#ifdef ENOCEANGATEWAY
#pragma message "Building Logic for ENOCEANGATEWAY"
#include "../../knx-enocean-gateway/src/EnoceanGateway.h"
// #elif TEST
// #include "../../knx-test/src/Test.h"
// #pragma message "Building Logic for TEST"
#endif

