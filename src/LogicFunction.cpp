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

float LogicFunction::nativeModulo(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (int32_t)E1 % (int32_t)E2;
}

float LogicFunction::nativeAnd(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (int32_t)E1 & (int32_t)E2;
}

float LogicFunction::nativeOr(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (int32_t)E1 | (int32_t)E2;
}

float LogicFunction::nativeXor(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (int32_t)E1 ^ (int32_t)E2;
}

float LogicFunction::nativeLShift(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (int32_t)E1 << (int32_t)E2;
}

float LogicFunction::nativeRShift(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    return (int32_t)E1 >> (int32_t)E2;
}

float LogicFunction::nativeBool2Int(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut)
{
    // we assume E1, E2 are boolean
    // if not, standard conversion applies
    return (bool)E2 * 2 + (bool)E1;
}


// do not touch after this point

LogicFunction::LogicFunction(){};

LogicFunction::~LogicFunction(){};

float (*LogicFunction::nativeFunction[NUM_NATIVE_FUNCTIONS])(uint8_t, float, uint8_t, float, uint8_t *)
{
    nativeAdd,
    nativeSubtract,
    nativeMultiply,
    nativeDivide,
    nativeAverage,
    nativeMinimum,
    nativeMaximum,
    nativeModulo,
    nativeAnd,
    nativeOr,
    nativeXor,
    nativeLShift,
    nativeRShift,
    nativeBool2Int,
    };

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
uValue LogicFunction::callFunction(uint8_t iId, uint8_t iDptE1, uValue iE1, uint8_t iDptE2, uValue iE2, uint8_t *cDptOut)
{
    uValue lResult = {};
    float lE1 = iDptE1 == VAL_DPT_9 ? iE1.floatValue : (float)iE1.intValue;
    float lE2 = iDptE2 == VAL_DPT_9 ? iE2.floatValue : (float)iE2.intValue;
    float lResultf = 0;
    if (iId > 0 && iId <= NUM_NATIVE_FUNCTIONS)
    {
        lResultf = nativeFunction[iId - 1](iDptE1, lE1, iDptE2, lE2, cDptOut);
    }
    else if (iId > 200 && iId <= 230)
    {
        lResultf = userFunction[iId - 201](iDptE1, lE1, iDptE2, lE2, cDptOut);
    }

    if (*cDptOut == VAL_DPT_9)
    {
        lResult.floatValue = lResultf;
    }
    else
    {
        lResult.intValue = lResultf;
    }
    return lResult;
}
