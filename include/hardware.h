#pragma once
#include "HardwareConfig.h"

#define FIRMWARE_NAME "Logikmodul"

// verified hardware
#ifdef DEVICE_SMARTMF_SENSORMODUL
    #define KNX_I2C_SDA_PIN SMARTMF_I2C0_SDA_PIN
    #define KNX_I2C_SCL_PIN SMARTMF_I2C0_SCL_PIN
    #define BUZZER_PIN SMARTMF_BUZZER_PIN
    #define SmartMF_HardwareVariant_PIN SMARTMF_HARDWARE_REVISION_ADC_PIN
    #define SmartMF_HardwareRevision_ID1 SMARTMF_HARDWARE_REVISION_ID1_PIN
    #define SmartMF_HardwareRevision_ID2 SMARTMF_HARDWARE_REVISION_ID2_PIN 
    #define SmartMF_HardwareRevision_ID3 SMARTMF_HARDWARE_REVISION_ID3_PIN
#endif

#ifdef DEVICE_SMARTMF_1TE_REG
    // no additional defines needed
#endif

#ifdef DEVICE_SMARTMF_REALPRESENCE_V2
    #define SmartMF_HardwareVariant_PIN 29
    #define SmartMF_HardwareRevision_ID1 22
    #define SmartMF_HardwareRevision_ID2 23
    #define SmartMF_HardwareRevision_ID3 24
    #define HF_S1_PIN 2
    #define HF_S2_PIN 3
    #define HF_POWER_PIN 10
#endif

#ifdef DEVICE_PIPICO_BCU_CONNECTOR
    #define BUZZER_PIN 27
#endif


    #ifdef BOARD_AB_TOUCH_ROUND_RP2350
        #define DEVICE_ID "AB-TOUCH-ROUND"
        #define HARDWARE_NAME "AB-TOUCH-ROUND"
        #define DEVICE_NAME "AB-SmartHouse Touch Round"
        #define INFO_LED_PIN 11
        #define INFO_LED_PIN_ACTIVE_ON HIGH
        #define PROG_LED_PIN 10
        #define PROG_LED_PIN_ACTIVE_ON HIGH
        #define PROG_BUTTON_PIN 9
        #define PROG_BUTTON_PIN_INTERRUPT_ON FALLING

        #define KNX_UART_NUM 0
        #define KNX_UART_TX_PIN 12
        #define KNX_UART_RX_PIN 13

        #define SAVE_INTERRUPT_PIN 0

        #define TOUCH_LEFT_PIN 24
        #define TOUCH_RIGHT_PIN 25

        #define I2C_WIRE Wire1
        #define I2C_SDA_PIN 26
        #define I2C_SCL_PIN 27
    #endif

// legacy hardware
#include "LogikmodulHardware.h"
