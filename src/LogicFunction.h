#pragma once

#include <inttypes.h>

#define NUM_NATIVE_FUNCTIONS 7

class LogicFunction
{
  private:
    LogicFunction(/* args */);
    ~LogicFunction();

    static float (*nativeFunction[NUM_NATIVE_FUNCTIONS])(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float (*userFunction[30])(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);

    // implemented native functions, as an simple example
    static float nativeAdd(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float nativeSubtract(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float nativeMultiply(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float nativeDivide(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float nativeAverage(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float nativeMinimum(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float nativeMaximum(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);

    // user functions (empty, implemented by user)
    static float userFunction01(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction02(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction03(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction04(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction05(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction06(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction07(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction08(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction09(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction10(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction11(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction12(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction13(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction14(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction15(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction16(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction17(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction18(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction19(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction20(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction21(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction22(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction23(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction24(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction25(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction26(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction27(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction28(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction29(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);
    static float userFunction30(uint8_t DptE1, float E1, uint8_t DptE2, float E2, uint8_t *DptOut);

  public:
    static uint32_t callFunction(uint8_t iId, uint8_t iDptE1, uint32_t iE1, uint8_t iDptE2, uint32_t iE2, uint8_t *cDptOut);
};
