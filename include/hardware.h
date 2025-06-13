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

// legacy hardware
#include "LogikmodulHardware.h"
