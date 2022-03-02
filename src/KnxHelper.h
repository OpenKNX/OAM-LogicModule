#pragma once

#include <knx.h>

// enum supported dpt
#define VAL_DPT_1 0
#define VAL_DPT_2 1
#define VAL_DPT_5 2
#define VAL_DPT_5001 3
#define VAL_DPT_6 4
#define VAL_DPT_7 5
#define VAL_DPT_8 6
#define VAL_DPT_9 7
#define VAL_DPT_16 8
#define VAL_DPT_17 9
#define VAL_DPT_232 10
#define VAL_DPT_10 11   // Time
#define VAL_DPT_11 12   // Date

Dpt &getDPT(uint8_t iDptIndex);

#define VAL_TIMEBASE_SECONDS 0x0000
#define VAL_TIMEBASE_MINUTES 0x4000
#define VAL_TIMEBASE_HOURS 0x8000
#define VAL_TIMEBASE_TENTH_SECONDS 0xC000

#define VAL_TIME_MASK 0x3FFF
#define VAL_TIMEBASE_MASK 0xC000

uint32_t getDelayPattern(uint16_t iParamIndex, bool iAsSeconds = false);
