#pragma once

#include <inttypes.h>
// #include <KnxHelper.h>
#include <LogicValue.h>

#define NUM_NATIVE_FUNCTIONS 14

class LogicFunction
{
  private:
    LogicFunction();
    ~LogicFunction();

    static LogicValue (*nativeFunction[NUM_NATIVE_FUNCTIONS])(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue (*userFunction[30])(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);

    // implemented native functions, as an simple example
    static LogicValue nativeAdd(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeSubtract(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeMultiply(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeDivide(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeAverage(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeMinimum(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeMaximum(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeModulo(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeAnd(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeOr(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeXor(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeLShift(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeRShift(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue nativeBool2Int(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);

    // user functions (empty, implemented by user)
    static LogicValue userFunction01(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction02(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction03(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction04(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction05(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction06(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction07(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction08(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction09(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction10(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction11(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction12(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction13(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction14(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction15(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction16(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction17(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction18(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction19(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction20(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction21(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction22(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction23(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction24(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction25(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction26(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction27(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction28(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction29(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);
    static LogicValue userFunction30(uint8_t DptE1, LogicValue E1, uint8_t DptE2, LogicValue E2, uint8_t *DptOut);

  public:
    static LogicValue callFunction(uint8_t iId, uint8_t iDptE1, LogicValue iE1, uint8_t iDptE2, LogicValue iE2, uint8_t *cDptOut);
};
