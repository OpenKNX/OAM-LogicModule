#pragma once
#include <knx_facade.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>
#include "Timer.h"
#include "TimerRestore.h"
#include "KnxHelper.h"
#include "EepromManager.h"
#include "IncludeManager.h"
#include "Hardware.h"

#define SAVE_BUFFER_START_PAGE 0 // All stored KO data begin at this page and takes 40 pages,
#define SAVE_BUFFER_NUM_PAGES 41 // so next store should start at page 41

// here we define, how many channels are compiled into firmware, has to be greater equal the number in knxprod
#define LOG_ChannelsFirmware COUNT_LOG_CHANNEL

// enum input defaults
#define VAL_InputDefault_Undefined 0
#define VAL_InputDefault_Read 1
#define VAL_InputDefault_False 2
#define VAL_InputDefault_True 3
#define VAL_InputDefault_EEPROM 4

// enum input converter
#define VAL_InputConvert_Interval 0
#define VAL_InputConvert_DeltaInterval 1
#define VAL_InputConvert_Hysterese 2
#define VAL_InputConvert_DeltaHysterese 3
#define VAL_InputConvert_Values 4
#define VAL_InputConvert_Constant 5

// enum logical function
#define VAL_Logic_And 1
#define VAL_Logic_Or 2
#define VAL_Logic_ExOr 3
#define VAL_Logic_Gate 4
#define VAL_Logic_Timer 5

// enum gate trigger values
#define VAL_Gate_Send_Nothing 0
#define VAL_Gate_Send_Off 1
#define VAL_Gate_Send_On 2
#define VAL_Gate_Send_Input 3

// enum gate transitions
#define VAL_Gate_Closed_Close 0
#define VAL_Gate_Closed_Open 1
#define VAL_Gate_Open_Close 2
#define VAL_Gate_Open_Open 3

// enum delay extend
#define VAL_Delay_Nothing 0
#define VAL_Delay_Extend 1
#define VAL_Delay_Immediate 2

// enum delay reset
#define VAL_Reset_Nothing 0
#define VAL_Reset_Reset 1

// enum output values
#define VAL_Out_No 0
#define VAL_Out_Constant 1
#define VAL_Out_ValE1 2
#define VAL_Out_ValE2 3
#define VAL_Out_ReadRequest 4
#define VAL_Out_ResetDevice 5
#define VAL_Out_Buzzer 6
#define VAL_Out_RGBLed 7
#define VAL_Out_Function 8

// enum output filter
#define VAL_AllowRepeat_All 0
#define VAL_AllowRepeat_On 1
#define VAL_AllowRepeat_Off 2
#define VAL_AllowRepeat_None 3

// enum for buzzer volume
#define VAL_Buzzer_Off 0
#define VAL_Buzzer_Silent 2
#define VAL_Buzzer_Normal 3
#define VAL_Buzzer_Loud 1

// flags for in- and output
#define BIT_EXT_INPUT_1 0x01
#define BIT_EXT_INPUT_2 0x02
#define BIT_INT_INPUT_1 0x04
#define BIT_INT_INPUT_2 0x08
#define BIT_INPUT_MASK 0x0F
#define BIT_FIRST_PROCESSING 0x20
#define BIT_PREVIOUS_GATE 0x40

#define BIT_OUTPUT_LOGIC 0x01    
#define BIT_OUTPUT_BLINK 0x02    
#define BIT_OUTPUT_PREVIOUS 0x04 
#define BIT_OUTPUT_DEBUG 0x10    

// enum fo IOIndex
#define IO_Absolute 0
#define IO_Input1 BIT_EXT_INPUT_1 // 1
#define IO_Input2 BIT_EXT_INPUT_2 // 2
#define IO_Output 3

// pipeline steps
#define PIP_STARTUP 1                     // startup delay for each channel
#define PIP_REPEAT_INPUT1 2               // send read requests for input 1
#define PIP_REPEAT_INPUT2 4               // send read requests for input 2
#define PIP_CONVERT_INPUT1 8              // convert input value 1 to bool
#define PIP_CONVERT_INPUT2 16             // convert input value 2 to bool
#define PIP_LOGIC_EXECUTE 32              // do logical step
#define PIP_STAIRLIGHT 64                 // do stairlight delay
#define PIP_BLINK 128                     // do blinking during stairlight
#define PIP_ON_DELAY 256                  // delay on signal
#define PIP_OFF_DELAY 512                 // delay off signal
#define PIP_OUTPUT_FILTER_ON 1024         // Filter repeated signals
#define PIP_OUTPUT_FILTER_OFF 2048        // Filter repeated signals
#define PIP_ON_REPEAT 4096                // repeat on signal
#define PIP_OFF_REPEAT 8192               // repeat off signal
#define PIP_TIMER_INPUT 16384             // process timer as input signal
#define PIP_RUNNING 32768                 // is a currently running channel
#define PIP_TIMER_RESTORE_STATE 65536     // timer restore is active for this channel
#define PIP_TIMER_RESTORE_STEP 131072     // timer restore for this channel was processed an other day back

#define TIMD_WEEKDAY_MASK 0x0007
#define TIMD_WEEKDAY_SHIFT 0
#define TIMD_MINUTE_MASK 0x01F8
#define TIMD_MINUTE_SHIFT 3
#define TIMD_HOUR_MASK 0x3E00
#define TIMD_HOUR_SHIFT 9
#define TIMD_VALUE_MASK 0x8000
#define TIMD_VALUE_SHIFT 15

#define TIMY_MONTH_MASK 0x00F8
#define TIMY_MONTH_SHIFT 3
#define TIMY_ISWEEKDAY_MASK 0x0100
#define TIMY_ISWEEKDAY_SHIFT 8
#define TIMY_DAY_MASK 0xFE00
#define TIMY_DAY_SHIFT 9
#define TIMY_MONDAY_MASK 0x8000
#define TIMY_MONDAY_SHIFT 15
#define TIMY_TUESDAY_MASK 0x4000
#define TIMY_TUESDAY_SHIFT 14
#define TIMY_WEDNESDAY_MASK 0x2000
#define TIMY_WEDNESDAY_SHIFT 13
#define TIMY_THURSDAY_MASK 0x1000
#define TIMY_THURSDAY_SHIFT 12
#define TIMY_FRIDAY_MASK 0x0800
#define TIMY_FRIDAY_SHIFT 11
#define TIMY_SATURDAY_MASK 0x0400
#define TIMY_SATURDAY_SHIFT 10
#define TIMY_SUNDAY_MASK 0x0200
#define TIMY_SUNDAY_SHIFT 9

#define VAL_Tim_Inactive 0
#define VAL_Tim_PointInTime 1
#define VAL_Tim_Sunrise_Plus 4
#define VAL_Tim_Sunrise_Minus 5
#define VAL_Tim_Sunrise_Earliest 6
#define VAL_Tim_Sunrise_Latest 7
#define VAL_Tim_Sunset_Plus 8
#define VAL_Tim_Sunset_Minus 9
#define VAL_Tim_Sunset_Earliest 10
#define VAL_Tim_Sunset_Latest 11

#define VAL_Tim_Restore_No 0
#define VAL_Tim_Restore_Last 1
#define VAL_Tim_Restore_Last2 2

#define VAL_Tim_Special_Skip 0
#define VAL_Tim_Special_No 1
#define VAL_Tim_Special_Only 2
#define VAL_Tim_Special_Sunday 3

#define VAL_Tim_Timer_Daily 0
#define VAL_Tim_Timer_Yearly 1

#define VAL_Tim_Every_Minute 63
#define VAL_Tim_Every_Hour 31
#define VAL_Tim_Every_Weekday 0
#define VAL_Tim_Every_Day 0
#define VAL_Tim_Every_Month 0
#define VAL_Tim_Last_Day 32

#define VAL_Tim_YearTimerCount 4
#define VAL_Tim_DayTimerCount 8

// extern KnxFacade<LinuxPlatform, Bau57B0> knx;

const uint32_t cTimeFactors[] = {100, 1000, 60000, 3600000};

class Logic;

class LogicChannel
{
  private:
    // instance
    uint8_t mChannelId;
#if LOGIC_TRACE
    static char sFilter[30];
    int channelDebug(const char *format, ...);
    bool debugFilter();
#endif
    uint32_t calcParamIndex(uint16_t iParamIndex);
    uint16_t calcKoNumber(uint8_t iIOIndex);
    uint8_t getByteParam(uint16_t iParamIndex);
    int8_t getSByteParam(uint16_t iParamIndex);
    uint16_t getWordParam(uint16_t iParamIndex);
    int16_t getSWordParam(uint16_t iParamIndex);
    uint32_t getIntParam(uint16_t iParamIndex);
    int32_t getSIntParam(uint16_t iParamIndex);
    float getFloatParam(uint16_t iParamIndex);
    uint8_t* getStringParam(uint16_t iParamIndex);
    GroupObject *getKo(uint8_t iIOIndex);
    Dpt &getKoDPT(uint8_t iIOIndex);
    void knxWriteBool(uint8_t iIOIndex, bool iValue);
    void knxWriteInt(uint8_t iIOIndex, int32_t iValue);
    void knxWriteRawInt(uint8_t iIOIndex, int32_t iValue);
    void knxWriteFloat(uint8_t iIOIndex, float iValue);
    void knxWriteString(uint8_t iIOIndex, char* iValue);
    void knxRead(uint8_t iIOIndex);
    void knxResetDevice(uint16_t iParamIndex);
    int32_t getParamForDelta(uint8_t iDpt, uint16_t iParamIndex);
    int32_t getParamByDpt(uint8_t iDpt, uint16_t iParamIndex);
    int32_t getInputValue(uint8_t iIOIndex);
    void writeConstantValue(uint16_t iParamIndex);
    void writeParameterValue(uint8_t iIOIndex);
    void writeFunctionValue(uint16_t iParamIndex);
    void writeValue(uint32_t iValue, uint8_t iDpt);
    void setRGBColor(uint16_t iParamIndex);
    void setBuzzer(uint16_t iParamIndex);
    
    bool isInputActive(uint8_t iIOIndex);

    void startStartup();
    void processStartup();
    void processRepeatInput1();
    void processRepeatInput2();
    void stopRepeatInput(uint8_t iIOIndex);
    void startConvert(uint8_t iIOIndex);
    bool checkConvertValues(uint16_t iParamValues, uint8_t iValueSize, int32_t iValue);
    void processConvertInput(uint8_t iIOIndex);
    void startLogic(uint8_t iIOIndex, bool iValue);
    void processLogic();
    void startStairlight(bool iOutput);
    void processStairlight();
    void startBlink();
    void processBlink();
    void startOnDelay();
    void processOnDelay();
    void startOffDelay();
    void processOffDelay();
    void startOutputFilter(bool iOutput);
    void processOutputFilter();
    void startOnOffRepeat(bool iOutput);
    void processOnOffRepeat();

    void processOutput(bool iValue);

    bool readOneInputFromEEPROM(uint8_t iIOIndex);

    // Start of Timer implementation
    void processTimerInput();
    bool checkTimerToday(Timer &iTimer, uint8_t iTimerIndex, bool iHandleAsSunday);
    bool checkWeekday(Timer &iTimer, uint8_t iWeekday, bool iHandleAsSunday);
    bool checkTimerTime(Timer &iTimer, uint8_t iTimerIndex, uint16_t iBitfield, uint8_t iHour, uint8_t iMinute, bool iSkipWeekday, bool iHandleAsSunday);
    bool checkPointInTime(Timer &iTimer, uint8_t iTimerIndex, uint16_t iBitfield, bool iSkipWeekday, bool iHandleAsSunday);
    bool checkSunAbs(Timer &iTimer, uint8_t iSunInfo, uint8_t iTimerIndex, uint16_t iBitfield, bool iSkipWeekday, bool iHandleAsSunday, bool iMinus);
    bool checkSunLimit(Timer &iTimer, uint8_t iSunInfo, uint8_t iTimerIndex, uint16_t iBitfield, bool iSkipWeekday, bool iHandleAsSunday, bool iLatest);
    int16_t getTimerTime(Timer &iTimer, uint8_t iTimerIndex, uint16_t iBitfield, uint8_t iHour, uint8_t iMinute, bool iSkipWeekday, bool iHandleAsSunday);
    int16_t getPointInTime(Timer &iTimer, uint8_t iTimerIndex, uint16_t iBitfield, bool iSkipWeekday, bool iHandleAsSunday);
    int16_t getSunAbs(Timer &iTimer, uint8_t iSunInfo, uint8_t iTimerIndex, uint16_t iBitfield, bool iSkipWeekday, bool iHandleAsSunday, bool iMinus);
    int16_t getSunLimit(Timer &iTimer, uint8_t iSunInfo, uint8_t iTimerIndex, uint16_t iBitfield, bool iSkipWeekday, bool iHandleAsSunday, bool iLatest);
    uint32_t getTimerNow(bool iMidnight);
    void processTimerRestoreState(TimerRestore &iTimer);

  protected:

    union uInputProcessing
    {
        struct
        {
            uint32_t repeatInput1Delay;
            uint32_t repeatInput2Delay;
        };
        struct
        {
            uint8_t activeTimer;
            uint8_t processedTimer;
        };
    };

    // static
    static Timer &sTimer;
    static TimerRestore &sTimerRestore;

    // instance
    /* Runtime information per channel */
    uint8_t pTriggerIO;        // Bitfield: Which input (0-3) triggered processing, Bit 4-7 are not used
    uint8_t pValidActiveIO;    // Bitfield: validity flags for input (0-3) values and active inputs (4-7)
    uint8_t pCurrentIn;        // Bitfield: current input (0-3), free (4), first processing (5), previous gate (6) and free (7) values
    uint8_t pCurrentOut;       // Bitfield: logic output (0), blink output (1), previous output (2)
    uint32_t pCurrentPipeline; // Bitfield: indicator for current pipeline step

    uint8_t pCurrentIODebug;   // Bitfield: current input (0-3), logic output (4)
    // uint32_t pRepeatInput1Delay;  // used also for timer preparation
    // uint32_t pRepeatInput2Delay;  // used also for timer processing
    uInputProcessing pInputProcessing;
    uint32_t pStairlightDelay;
    uint32_t pBlinkDelay;
    uint32_t pOnDelay;
    uint32_t pOffDelay;
    uint32_t pRepeatOnOffDelay;

  public:
    // Constructors
    LogicChannel(uint8_t iChannelNumber);
    ~LogicChannel();

    // static
    static Logic *sLogic;
    static uint16_t calcKoNumber(uint8_t iIOIndex, uint8_t iChannelId);
    static GroupObject *getKoForChannel(uint8_t iIOIndex, uint8_t iChannelId);
    static float getFloat(uint8_t *data);

    // instance
    bool checkDpt(uint8_t iIOIndex, uint8_t iDpt);
    void processInput(uint8_t iIOIndex);
    void processInternalInputs(uint8_t iChannelId, bool iValue);
    bool processDiagnoseCommand(char* cBuffer);
    void startTimerInput();
    void startTimerRestoreState();
    void stopTimerRestoreState();
    void writeSingleDptToEEPROM(uint8_t iIOIndex);

    bool prepareChannel();
    void loop();
};