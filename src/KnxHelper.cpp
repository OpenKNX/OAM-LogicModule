#include "KnxHelper.h"

static Dpt sDpt[] = {Dpt(1, 1), Dpt(2, 1), Dpt(5, 10), Dpt(5, 1), Dpt(6, 1), Dpt(7, 1), Dpt(8, 1), Dpt(9, 2), Dpt(16, 1), Dpt(17, 1), Dpt(232, 600), Dpt(10, 1, 1), Dpt(11, 1), Dpt(12, 1), Dpt(13,1), Dpt(14,1)};

Dpt &getDPT(uint8_t iDptIndex)
{
    return sDpt[iDptIndex];
}

/***********************************************************
 * calculate delay from 2 byte delay pattern used in knxprod
 * iAsSeconds: true=return seconds, false=return milliseconds
 * first 2 bit: 
 *   00 - seconds
 *   01 - minutes
 *   10 - hours
 *   11 - seconds/10
 * 
 * remaining 14 bit: uint14
*/
uint32_t getDelayPattern(uint16_t iParamIndex, bool iAsSeconds /* = false */) {

    uint16_t lParam = knx.paramWord(iParamIndex);
    uint16_t lTime = lParam & VAL_TIME_MASK;

    switch (lParam & VAL_TIMEBASE_MASK)
    {
        case VAL_TIMEBASE_TENTH_SECONDS:
            return (iAsSeconds) ? lTime / 10 : lTime * 100;
            break;
        case VAL_TIMEBASE_SECONDS:
            return (iAsSeconds) ? lTime : lTime * 1000;
            break;
        case VAL_TIMEBASE_MINUTES:
            return (iAsSeconds) ? lTime * 60 : lTime * 60000;
            break;
        case VAL_TIMEBASE_HOURS:
            // for hour, we can only cover 1193 hours, we allow just 1000 here
            if (lTime > 1000)
                lTime = 1000;
            return (iAsSeconds) ? lTime * 3600 : lTime * 3600000;
            break;

        default:
            return 0;
            break;
    }
}

// bool uValueLessThanOrEquals(uValue iLeft, uValue iRight, uint8_t iDptLeft, uint8_t iDptRight)
// {
//   if (iDptLeft == VAL_DPT_9 && iDptRight == VAL_DPT_9)
//   {
//     return iLeft.floatValue <= iRight.floatValue;
//   }
//   else if (iDptLeft != VAL_DPT_9 && iDptRight == VAL_DPT_9)
//   {
//     return iLeft.intValue <= iRight.floatValue;
//   }
//   else if (iDptLeft == VAL_DPT_9 && iDptRight != VAL_DPT_9)
//   {
//     return iLeft.floatValue <= iRight.intValue;
//   }
//   else
//   {
//     return iLeft.intValue <= iRight.intValue;
//   }
// };

// bool uValueGreaterThanOrEquals(uValue iLeft, uValue iRight, uint8_t iDptLeft, uint8_t iDptRight)
// {
//   if (iDptLeft == VAL_DPT_9 && iDptRight == VAL_DPT_9)
//   {
//     return iLeft.floatValue >= iRight.floatValue;
//   }
//   else if (iDptLeft != VAL_DPT_9 && iDptRight == VAL_DPT_9)
//   {
//     return iLeft.intValue >= iRight.floatValue;
//   }
//   else if (iDptLeft == VAL_DPT_9 && iDptRight != VAL_DPT_9)
//   {
//     return iLeft.floatValue >= iRight.intValue;
//   }
//   else
//   {
//     return iLeft.intValue >= iRight.intValue;
//   }
// };

// uValue uValueSubtract(uValue iLeft, uValue iRight, uint8_t iDptLeft, uint8_t iDptRight)
// {
//   if (iDptLeft == VAL_DPT_9 && iDptRight == VAL_DPT_9)
//   {
//     return { .floatValue = iLeft.floatValue - iRight.floatValue };
//   }
//   else if (iDptLeft != VAL_DPT_9 && iDptRight == VAL_DPT_9)
//   {
//     return { .floatValue = iLeft.intValue - iRight.floatValue };
//   }
//   else if (iDptLeft == VAL_DPT_9 && iDptRight != VAL_DPT_9)
//   {
//     return { .floatValue = iLeft.floatValue - iRight.intValue };
//   }
//   else
//   {
//     return { .intValue = iLeft.intValue - iRight.intValue };
//   }
// }
