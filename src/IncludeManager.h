#pragma once
#define STR(x) #x
#define XSTR(x) STR(x)

#ifdef LOGICMODULE
#pragma message "Building Logic for LOGIC-MODULE"
#include "Logikmodul.h"
#endif
#ifdef SENSORMODULE
#pragma message "Building Logic for SENSOR-MODULE"
#include "../../OAM-SensorModule/src/Sensormodul.h"
#define WIREMODULE 1
#endif
#ifdef PMMODULE
#pragma message "Building Logic for PM-MODULE"
#include "../../OAM-PresenceModule/src/PMmodul.h"
#endif
#ifdef SENSORVPMMODULE
#pragma message "Building Logic for SENSOR-VPM-MODULE"
#include "../../OAM-SensorVpmModule/src/SensormodulVpm.h"
#define SENSORMODULE 1
#define PMMOULE 1
#endif
#ifdef WIREGATEWAY
#pragma message "Building Logic for WIRE-GATEWAY"
#include "../../OAM-OneWireModule/src/WireGateway.h"
#define WIREMODULE 1
#endif
#ifdef ENOCEANGATEWAY
#pragma message "Building Logic for ENOCEAN-GATEWAY"
#include "../../OAM-EnoceanGateway/src/EnoceanGateway.h"
#endif
#ifdef GARDENCONTROL
#pragma message "Building Logic for GARDEN-CONTROL"
#include "../../BEM-GardenControl/src/GardenControl.h"
#endif
// #ifdef TEST
// #include "../../knx-test/src/Test.h"
// #pragma message "Building Logic for TEST"
// #endif
