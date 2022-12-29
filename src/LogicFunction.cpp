#include "KnxHelper.h"
#include "LogicFunction.h"
// #include "LogicValue.h"

// native functions, implemented as a simple example how to use user functions
LogicValue LogicFunction::nativeAdd(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return E1 + E2;
}

LogicValue LogicFunction::nativeSubtract(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return E1 - E2;
}

LogicValue LogicFunction::nativeMultiply(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return E1 * E2;
}

LogicValue LogicFunction::nativeDivide(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return E1 / E2;
}

LogicValue LogicFunction::nativeAverage(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (float)(E1 + E2) / 2;
}

LogicValue LogicFunction::nativeMinimum(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (E1 < E2) ? E1 : E2;
}

LogicValue LogicFunction::nativeMaximum(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (E1 > E2) ? E1 : E2;
}

LogicValue LogicFunction::nativeModulo(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (int32_t)E1 % (int32_t)E2;
}

LogicValue LogicFunction::nativeAnd(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (int32_t)E1 & (int32_t)E2;
}

LogicValue LogicFunction::nativeOr(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (int32_t)E1 | (int32_t)E2;
}

LogicValue LogicFunction::nativeXor(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (int32_t)E1 ^ (int32_t)E2;
}

LogicValue LogicFunction::nativeLShift(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (int32_t)E1 << (int32_t)E2;
}

LogicValue LogicFunction::nativeRShift(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    return (int32_t)E1 >> (int32_t)E2;
}

LogicValue LogicFunction::nativeBool2Int(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut)
{
    // we assume E1, E2 are boolean
    // if not, standard conversion applies
    return (uint8_t)((bool)E2 * 2 + (bool)E1);
}


// do not touch after this point

LogicFunction::LogicFunction(){};

LogicFunction::~LogicFunction(){};

LogicValue (*LogicFunction::nativeFunction[NUM_NATIVE_FUNCTIONS])(uint8_t, LogicValue, uint8_t, LogicValue, uint8_t *)
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

LogicValue (*LogicFunction::userFunction[30])(uint8_t, LogicValue, uint8_t, LogicValue, uint8_t *){
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
LogicValue LogicFunction::callFunction(uint8_t iId, uint8_t iDptE1, LogicValue iE1, uint8_t iDptE2, LogicValue iE2, uint8_t *cDptOut)
{
    if (iId > 0 && iId <= NUM_NATIVE_FUNCTIONS)
    {
        LogicValue lResult = nativeFunction[iId - 1](iDptE1, iE1, iDptE2, iE2, cDptOut);
        return lResult;
    }
    else if (iId > 200 && iId <= 230)
    {
        LogicValue lResult = userFunction[iId - 201](iDptE1, iE1, iDptE2, iE2, cDptOut);
        return lResult;
    }
    return (uint8_t)0;
}
