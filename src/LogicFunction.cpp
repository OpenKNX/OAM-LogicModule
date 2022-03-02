#include "KnxHelper.h"
#include "LogicFunction.h"

// native functions, implemented as a simple example how to use user functions
float LogicFunction::nativeAdd(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return E1 + E2;
}

float LogicFunction::nativeSubtract(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return E1 - E2;
}

float LogicFunction::nativeMultiply(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return E1 * E2;
}

float LogicFunction::nativeDivide(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return E1 / E2;
}

float LogicFunction::nativeAverage(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (E1 + E2) / 2;
}

float LogicFunction::nativeMinimum(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (E1 < E2) ? E1 : E2;
}

float LogicFunction::nativeMaximum(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (E1 > E2) ? E1 : E2;
}

// do not touch after this point

LogicFunction::LogicFunction(){};

LogicFunction::~LogicFunction(){};

float (*LogicFunction::nativeFunction[NUM_NATIVE_FUNCTIONS])(uint8_t, float, uint8_t, float, uint8_t *){
    nativeAdd,
    nativeSubtract,
    nativeMultiply,
    nativeDivide,
    nativeAverage,
    nativeMinimum,
    nativeMaximum};

float (*LogicFunction::userFunction[30])(uint8_t, float, uint8_t, float, uint8_t *){
    userFunction01,
    userFunction02,
    userFunction03,
    userFunction04,
    userFunction05,
    userFunction06,
    userFunction07,
    userFunction08,
    userFunction09,
    userFunction10,
    userFunction11,
    userFunction12,
    userFunction13,
    userFunction14,
    userFunction15,
    userFunction16,
    userFunction17,
    userFunction18,
    userFunction19,
    userFunction20,
    userFunction21,
    userFunction22,
    userFunction23,
    userFunction24,
    userFunction25,
    userFunction26,
    userFunction27,
    userFunction28,
    userFunction29,
    userFunction30};

// dispatcher
uint32_t LogicFunction::callFunction(uint8_t iId, uint8_t iDptE1, uint32_t iE1, uint8_t iDptE2, uint32_t iE2, uint8_t *cDptOut)
{
    // DPT9 is transported as int with factor 100, we transform here the base
    float lE1 = (float)iE1;
    float lE2 = (float)iE2;
    if (iDptE1 == VAL_DPT_9)
        lE1 /= 100.0;
    if (iDptE2 == VAL_DPT_9)
        lE2 /= 100.0;
    float lResult = 0.0;
    if (iId > 0 && iId <= NUM_NATIVE_FUNCTIONS)
    {
        lResult = nativeFunction[iId - 1](iDptE1, iE1, iDptE2, iE2, cDptOut);
    }
    else if (iId > 200 && iId <= 230)
    {
        lResult = userFunction[iId - 201](iDptE1, iE1, iDptE2, iE2, cDptOut);
    }
    // if (*cDptOut == VAL_DPT_9)
    //     lResult *= 100.0;
    return (uint32_t)lResult;
}