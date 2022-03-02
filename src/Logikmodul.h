#pragma once
#include <knx.h>

// Parameter with single occurance
#define LOG_NumChannels                0      // uint8_t
#define LOG_StartupDelay               1      // int32_t
#define LOG_Heartbeat                  5      // int32_t
#define LOG_ReadTimeDate               9      // 1 Bit, Bit 7
#define     LOG_ReadTimeDateMask 0x80
#define     LOG_ReadTimeDateShift 7
#define LOG_BuzzerInstalled            9      // 1 Bit, Bit 6
#define     LOG_BuzzerInstalledMask 0x40
#define     LOG_BuzzerInstalledShift 6
#define LOG_LedInstalled               9      // 1 Bit, Bit 5
#define     LOG_LedInstalledMask 0x20
#define     LOG_LedInstalledShift 5
#define LOG_EepromInstalled            9      // 1 Bit, Bit 4
#define     LOG_EepromInstalledMask 0x10
#define     LOG_EepromInstalledShift 4
#define LOG_NCN5130Installed           9      // 1 Bit, Bit 3
#define     LOG_NCN5130InstalledMask 0x08
#define     LOG_NCN5130InstalledShift 3
#define LOG_VacationKo                 9      // 1 Bit, Bit 2
#define     LOG_VacationKoMask 0x04
#define     LOG_VacationKoShift 2
#define LOG_HolidayKo                  9      // 1 Bit, Bit 1
#define     LOG_HolidayKoMask 0x02
#define     LOG_HolidayKoShift 1
#define LOG_VacationRead               9      // 1 Bit, Bit 0
#define     LOG_VacationReadMask 0x01
#define     LOG_VacationReadShift 0
#define LOG_HolidaySend               10      // 1 Bit, Bit 7
#define     LOG_HolidaySendMask 0x80
#define     LOG_HolidaySendShift 7
#define LOG_Timezone                  10      // 2 Bits, Bit 6-5
#define     LOG_TimezoneMask 0x60
#define     LOG_TimezoneShift 5
#define LOG_UseSummertime             10      // 1 Bit, Bit 4
#define     LOG_UseSummertimeMask 0x10
#define     LOG_UseSummertimeShift 4
#define LOG_Diagnose                  10      // 1 Bit, Bit 3
#define     LOG_DiagnoseMask 0x08
#define     LOG_DiagnoseShift 3
#define LOG_Watchdog                  10      // 1 Bit, Bit 2
#define     LOG_WatchdogMask 0x04
#define     LOG_WatchdogShift 2
#define LOG_Neujahr                   11      // 1 Bit, Bit 7
#define     LOG_NeujahrMask 0x80
#define     LOG_NeujahrShift 7
#define LOG_DreiKoenige               11      // 1 Bit, Bit 6
#define     LOG_DreiKoenigeMask 0x40
#define     LOG_DreiKoenigeShift 6
#define LOG_Weiberfastnacht           11      // 1 Bit, Bit 5
#define     LOG_WeiberfastnachtMask 0x20
#define     LOG_WeiberfastnachtShift 5
#define LOG_Rosenmontag               11      // 1 Bit, Bit 4
#define     LOG_RosenmontagMask 0x10
#define     LOG_RosenmontagShift 4
#define LOG_Fastnachtsdienstag        11      // 1 Bit, Bit 3
#define     LOG_FastnachtsdienstagMask 0x08
#define     LOG_FastnachtsdienstagShift 3
#define LOG_Aschermittwoch            11      // 1 Bit, Bit 2
#define     LOG_AschermittwochMask 0x04
#define     LOG_AschermittwochShift 2
#define LOG_Gruendonnerstag           11      // 1 Bit, Bit 1
#define     LOG_GruendonnerstagMask 0x02
#define     LOG_GruendonnerstagShift 1
#define LOG_Karfreitag                11      // 1 Bit, Bit 0
#define     LOG_KarfreitagMask 0x01
#define     LOG_KarfreitagShift 0
#define LOG_Ostersonntag              12      // 1 Bit, Bit 7
#define     LOG_OstersonntagMask 0x80
#define     LOG_OstersonntagShift 7
#define LOG_Ostermontag               12      // 1 Bit, Bit 6
#define     LOG_OstermontagMask 0x40
#define     LOG_OstermontagShift 6
#define LOG_TagDerArbeit              12      // 1 Bit, Bit 5
#define     LOG_TagDerArbeitMask 0x20
#define     LOG_TagDerArbeitShift 5
#define LOG_Himmelfahrt               12      // 1 Bit, Bit 4
#define     LOG_HimmelfahrtMask 0x10
#define     LOG_HimmelfahrtShift 4
#define LOG_Pfingstsonntag            12      // 1 Bit, Bit 3
#define     LOG_PfingstsonntagMask 0x08
#define     LOG_PfingstsonntagShift 3
#define LOG_Pfingstmontag             12      // 1 Bit, Bit 2
#define     LOG_PfingstmontagMask 0x04
#define     LOG_PfingstmontagShift 2
#define LOG_Frohleichnam              12      // 1 Bit, Bit 1
#define     LOG_FrohleichnamMask 0x02
#define     LOG_FrohleichnamShift 1
#define LOG_Friedensfest              12      // 1 Bit, Bit 0
#define     LOG_FriedensfestMask 0x01
#define     LOG_FriedensfestShift 0
#define LOG_MariaHimmelfahrt          13      // 1 Bit, Bit 7
#define     LOG_MariaHimmelfahrtMask 0x80
#define     LOG_MariaHimmelfahrtShift 7
#define LOG_DeutscheEinheit           13      // 1 Bit, Bit 6
#define     LOG_DeutscheEinheitMask 0x40
#define     LOG_DeutscheEinheitShift 6
#define LOG_Reformationstag           13      // 1 Bit, Bit 5
#define     LOG_ReformationstagMask 0x20
#define     LOG_ReformationstagShift 5
#define LOG_Allerheiligen             13      // 1 Bit, Bit 4
#define     LOG_AllerheiligenMask 0x10
#define     LOG_AllerheiligenShift 4
#define LOG_BussBettag                13      // 1 Bit, Bit 3
#define     LOG_BussBettagMask 0x08
#define     LOG_BussBettagShift 3
#define LOG_Advent1                   13      // 1 Bit, Bit 2
#define     LOG_Advent1Mask 0x04
#define     LOG_Advent1Shift 2
#define LOG_Advent2                   13      // 1 Bit, Bit 1
#define     LOG_Advent2Mask 0x02
#define     LOG_Advent2Shift 1
#define LOG_Advent3                   13      // 1 Bit, Bit 0
#define     LOG_Advent3Mask 0x01
#define     LOG_Advent3Shift 0
#define LOG_Advent4                   14      // 1 Bit, Bit 7
#define     LOG_Advent4Mask 0x80
#define     LOG_Advent4Shift 7
#define LOG_Heiligabend               14      // 1 Bit, Bit 6
#define     LOG_HeiligabendMask 0x40
#define     LOG_HeiligabendShift 6
#define LOG_Weihnachtstag1            14      // 1 Bit, Bit 5
#define     LOG_Weihnachtstag1Mask 0x20
#define     LOG_Weihnachtstag1Shift 5
#define LOG_Weihnachtstag2            14      // 1 Bit, Bit 4
#define     LOG_Weihnachtstag2Mask 0x10
#define     LOG_Weihnachtstag2Shift 4
#define LOG_Silvester                 14      // 1 Bit, Bit 3
#define     LOG_SilvesterMask 0x08
#define     LOG_SilvesterShift 3
#define LOG_Latitude                  15      // float
#define LOG_Longitude                 19      // float
#define LOG_BuzzerSilent              23      // uint16_t
#define LOG_BuzzerNormal              25      // uint16_t
#define LOG_BuzzerLoud                27      // uint16_t
#define LOG_LedMapping                29      // 3 Bits, Bit 7-5
#define     LOG_LedMappingMask 0xE0
#define     LOG_LedMappingShift 5

// Parameter per channel
#define LOG_ParamBlockOffset 30
#define LOG_ParamBlockSize 100
#define LOG_fChannelDelay              0      // int32_t
#define LOG_fLogic                     4      // 8 Bits, Bit 7-0
#define LOG_fCalculate                 5      // 2 Bits, Bit 1-0
#define     LOG_fCalculateMask 0x03
#define     LOG_fCalculateShift 0
#define LOG_fDisable                   5      // 1 Bit, Bit 2
#define     LOG_fDisableMask 0x04
#define     LOG_fDisableShift 2
#define LOG_fAlarm                     5      // 1 Bit, Bit 3
#define     LOG_fAlarmMask 0x08
#define     LOG_fAlarmShift 3
#define LOG_fTrigger                   6      // 8 Bits, Bit 7-0
#define LOG_fTriggerE1                 6      // 1 Bit, Bit 0
#define     LOG_fTriggerE1Mask 0x01
#define     LOG_fTriggerE1Shift 0
#define LOG_fTriggerE2                 6      // 1 Bit, Bit 1
#define     LOG_fTriggerE2Mask 0x02
#define     LOG_fTriggerE2Shift 1
#define LOG_fTriggerI1                 6      // 1 Bit, Bit 2
#define     LOG_fTriggerI1Mask 0x04
#define     LOG_fTriggerI1Shift 2
#define LOG_fTriggerI2                 6      // 1 Bit, Bit 3
#define     LOG_fTriggerI2Mask 0x08
#define     LOG_fTriggerI2Shift 3
#define LOG_fTriggerTime               6      // 8 Bits, Bit 7-0
#define LOG_fTriggerGateClose          7      // 8 Bits, Bit 7-0
#define LOG_fTriggerGateOpen           8      // 8 Bits, Bit 7-0
#define LOG_fE1                        9      // 4 Bits, Bit 3-0
#define     LOG_fE1Mask 0x0F
#define     LOG_fE1Shift 0
#define LOG_fE1Convert                 9      // 4 Bits, Bit 7-4
#define     LOG_fE1ConvertMask 0xF0
#define     LOG_fE1ConvertShift 4
#define LOG_fE1ConvertFloat            9      // 4 Bits, Bit 7-4
#define     LOG_fE1ConvertFloatMask 0xF0
#define     LOG_fE1ConvertFloatShift 4
#define LOG_fE1ConvertSpecial          9      // 4 Bits, Bit 7-4
#define     LOG_fE1ConvertSpecialMask 0xF0
#define     LOG_fE1ConvertSpecialShift 4
#define LOG_fE1Dpt                    10      // 8 Bits, Bit 7-0
#define LOG_fE1Default                11      // 2 Bits, Bit 1-0
#define     LOG_fE1DefaultMask 0x03
#define     LOG_fE1DefaultShift 0
#define LOG_fE1DefaultEEPROM          11      // 1 Bit, Bit 2
#define     LOG_fE1DefaultEEPROMMask 0x04
#define     LOG_fE1DefaultEEPROMShift 2
#define LOG_fE1DefaultRepeat          11      // 1 Bit, Bit 3
#define     LOG_fE1DefaultRepeatMask 0x08
#define     LOG_fE1DefaultRepeatShift 3
#define LOG_fTYearDay                 11      // 1 Bit, Bit 4
#define     LOG_fTYearDayMask 0x10
#define     LOG_fTYearDayShift 4
#define LOG_fTRestoreState            11      // 2 Bits, Bit 6-5
#define     LOG_fTRestoreStateMask 0x60
#define     LOG_fTRestoreStateShift 5
#define LOG_fE1Repeat                 12      // int32_t
#define LOG_fE2                       16      // 4 Bits, Bit 3-0
#define     LOG_fE2Mask 0x0F
#define     LOG_fE2Shift 0
#define LOG_fE2Convert                16      // 4 Bits, Bit 7-4
#define     LOG_fE2ConvertMask 0xF0
#define     LOG_fE2ConvertShift 4
#define LOG_fE2ConvertFloat           16      // 4 Bits, Bit 7-4
#define     LOG_fE2ConvertFloatMask 0xF0
#define     LOG_fE2ConvertFloatShift 4
#define LOG_fE2ConvertSpecial         16      // 4 Bits, Bit 7-4
#define     LOG_fE2ConvertSpecialMask 0xF0
#define     LOG_fE2ConvertSpecialShift 4
#define LOG_fE2Dpt                    17      // 8 Bits, Bit 7-0
#define LOG_fE2Default                18      // 2 Bits, Bit 1-0
#define     LOG_fE2DefaultMask 0x03
#define     LOG_fE2DefaultShift 0
#define LOG_fE2DefaultEEPROM          18      // 1 Bit, Bit 2
#define     LOG_fE2DefaultEEPROMMask 0x04
#define     LOG_fE2DefaultEEPROMShift 2
#define LOG_fE2DefaultRepeat          18      // 1 Bit, Bit 3
#define     LOG_fE2DefaultRepeatMask 0x08
#define     LOG_fE2DefaultRepeatShift 3
#define LOG_fTHoliday                 18      // 2 Bits, Bit 4-3
#define     LOG_fTHolidayMask 0x18
#define     LOG_fTHolidayShift 3
#define LOG_fTVacation                18      // 2 Bits, Bit 6-5
#define     LOG_fTVacationMask 0x60
#define     LOG_fTVacationShift 5
#define LOG_fE2Repeat                 19      // int32_t
#define LOG_fTd1DuskDawn              19      // 4 Bits, Bit 7-4
#define     LOG_fTd1DuskDawnMask 0xF0
#define     LOG_fTd1DuskDawnShift 4
#define LOG_fTd2DuskDawn              19      // 4 Bits, Bit 3-0
#define     LOG_fTd2DuskDawnMask 0x0F
#define     LOG_fTd2DuskDawnShift 0
#define LOG_fTd3DuskDawn              20      // 4 Bits, Bit 7-4
#define     LOG_fTd3DuskDawnMask 0xF0
#define     LOG_fTd3DuskDawnShift 4
#define LOG_fTd4DuskDawn              20      // 4 Bits, Bit 3-0
#define     LOG_fTd4DuskDawnMask 0x0F
#define     LOG_fTd4DuskDawnShift 0
#define LOG_fTd5DuskDawn              21      // 4 Bits, Bit 7-4
#define     LOG_fTd5DuskDawnMask 0xF0
#define     LOG_fTd5DuskDawnShift 4
#define LOG_fTd6DuskDawn              21      // 4 Bits, Bit 3-0
#define     LOG_fTd6DuskDawnMask 0x0F
#define     LOG_fTd6DuskDawnShift 0
#define LOG_fTd7DuskDawn              22      // 4 Bits, Bit 7-4
#define     LOG_fTd7DuskDawnMask 0xF0
#define     LOG_fTd7DuskDawnShift 4
#define LOG_fTd8DuskDawn              22      // 4 Bits, Bit 3-0
#define     LOG_fTd8DuskDawnMask 0x0F
#define     LOG_fTd8DuskDawnShift 0
#define LOG_fE1LowDelta               23      // int32_t
#define LOG_fE1HighDelta              27      // int32_t
#define LOG_fE1LowDeltaFloat          23      // float
#define LOG_fE1HighDeltaFloat         27      // float
#define LOG_fE1Low0Valid              30      // 1 Bit, Bit 7
#define     LOG_fE1Low0ValidMask 0x80
#define     LOG_fE1Low0ValidShift 7
#define LOG_fE1Low1Valid              30      // 1 Bit, Bit 6
#define     LOG_fE1Low1ValidMask 0x40
#define     LOG_fE1Low1ValidShift 6
#define LOG_fE1Low2Valid              30      // 1 Bit, Bit 5
#define     LOG_fE1Low2ValidMask 0x20
#define     LOG_fE1Low2ValidShift 5
#define LOG_fE1Low3Valid              30      // 1 Bit, Bit 4
#define     LOG_fE1Low3ValidMask 0x10
#define     LOG_fE1Low3ValidShift 4
#define LOG_fE1Low4Valid              30      // 1 Bit, Bit 3
#define     LOG_fE1Low4ValidMask 0x08
#define     LOG_fE1Low4ValidShift 3
#define LOG_fE1Low5Valid              30      // 1 Bit, Bit 2
#define     LOG_fE1Low5ValidMask 0x04
#define     LOG_fE1Low5ValidShift 2
#define LOG_fE1Low6Valid              30      // 1 Bit, Bit 1
#define     LOG_fE1Low6ValidMask 0x02
#define     LOG_fE1Low6ValidShift 1
#define LOG_fE1Low7Valid              30      // 1 Bit, Bit 0
#define     LOG_fE1Low7ValidMask 0x01
#define     LOG_fE1Low7ValidShift 0
#define LOG_fE1LowDpt2                23      // 8 Bits, Bit 7-0
#define LOG_fE1Low1Dpt2               24      // 8 Bits, Bit 7-0
#define LOG_fE1Low2Dpt2               25      // 8 Bits, Bit 7-0
#define LOG_fE1Low3Dpt2               26      // 8 Bits, Bit 7-0
#define LOG_fE1LowDpt2Fix             23      // 8 Bits, Bit 7-0
#define LOG_fE1LowDpt5                23      // uint8_t
#define LOG_fE1HighDpt5               27      // uint8_t
#define LOG_fE1Low0Dpt5In             23      // uint8_t
#define LOG_fE1Low1Dpt5In             24      // uint8_t
#define LOG_fE1Low2Dpt5In             25      // uint8_t
#define LOG_fE1Low3Dpt5In             26      // uint8_t
#define LOG_fE1Low4Dpt5In             27      // uint8_t
#define LOG_fE1Low5Dpt5In             28      // uint8_t
#define LOG_fE1Low6Dpt5In             29      // uint8_t
#define LOG_fE1LowDpt5Fix             23      // uint8_t
#define LOG_fE1LowDpt5001             23      // uint8_t
#define LOG_fE1HighDpt5001            27      // uint8_t
#define LOG_fE1Low0Dpt5xIn            23      // uint8_t
#define LOG_fE1Low1Dpt5xIn            24      // uint8_t
#define LOG_fE1Low2Dpt5xIn            25      // uint8_t
#define LOG_fE1Low3Dpt5xIn            26      // uint8_t
#define LOG_fE1Low4Dpt5xIn            27      // uint8_t
#define LOG_fE1Low5Dpt5xIn            28      // uint8_t
#define LOG_fE1Low6Dpt5xIn            29      // uint8_t
#define LOG_fE1LowDpt5xFix            23      // uint8_t
#define LOG_fE1LowDpt6                23      // int8_t
#define LOG_fE1HighDpt6               27      // int8_t
#define LOG_fE1Low0Dpt6In             23      // int8_t
#define LOG_fE1Low1Dpt6In             24      // int8_t
#define LOG_fE1Low2Dpt6In             25      // int8_t
#define LOG_fE1Low3Dpt6In             26      // int8_t
#define LOG_fE1Low4Dpt6In             27      // int8_t
#define LOG_fE1Low5Dpt6In             28      // int8_t
#define LOG_fE1Low6Dpt6In             29      // int8_t
#define LOG_fE1LowDpt6Fix             23      // int8_t
#define LOG_fE1LowDpt7                23      // uint16_t
#define LOG_fE1HighDpt7               27      // uint16_t
#define LOG_fE1Low0Dpt7In             23      // uint16_t
#define LOG_fE1Low1Dpt7In             25      // uint16_t
#define LOG_fE1Low2Dpt7In             27      // uint16_t
#define LOG_fE1LowDpt7Fix             23      // uint16_t
#define LOG_fE1LowDpt8                23      // int16_t
#define LOG_fE1HighDpt8               27      // int16_t
#define LOG_fE1Low0Dpt8In             23      // int16_t
#define LOG_fE1Low1Dpt8In             25      // int16_t
#define LOG_fE1Low2Dpt8In             27      // int16_t
#define LOG_fE1LowDpt8Fix             23      // int16_t
#define LOG_fE1LowDpt9                23      // float
#define LOG_fE1HighDpt9               27      // float
#define LOG_fE1LowDpt9Fix             23      // float
#define LOG_fE1Low0Dpt17              23      // 8 Bits, Bit 7-0
#define LOG_fE1Low1Dpt17              24      // 8 Bits, Bit 7-0
#define LOG_fE1Low2Dpt17              25      // 8 Bits, Bit 7-0
#define LOG_fE1Low3Dpt17              26      // 8 Bits, Bit 7-0
#define LOG_fE1Low4Dpt17              27      // 8 Bits, Bit 7-0
#define LOG_fE1Low5Dpt17              28      // 8 Bits, Bit 7-0
#define LOG_fE1Low6Dpt17              29      // 8 Bits, Bit 7-0
#define LOG_fE1Low7Dpt17              30      // 8 Bits, Bit 7-0
#define LOG_fE1LowDpt17Fix            23      // 8 Bits, Bit 7-0
#define LOG_fE1LowDptRGB              23      // int32_t
#define LOG_fE1HighDptRGB             27      // int32_t
#define LOG_fE1LowDptRGBFix           23      // int32_t
#define LOG_fE2LowDelta               31      // int32_t
#define LOG_fE2HighDelta              35      // int32_t
#define LOG_fE2LowDeltaFloat          31      // float
#define LOG_fE2HighDeltaFloat         35      // float
#define LOG_fE2Low0Valid              38      // 1 Bit, Bit 7
#define     LOG_fE2Low0ValidMask 0x80
#define     LOG_fE2Low0ValidShift 7
#define LOG_fE2Low1Valid              38      // 1 Bit, Bit 6
#define     LOG_fE2Low1ValidMask 0x40
#define     LOG_fE2Low1ValidShift 6
#define LOG_fE2Low2Valid              38      // 1 Bit, Bit 5
#define     LOG_fE2Low2ValidMask 0x20
#define     LOG_fE2Low2ValidShift 5
#define LOG_fE2Low3Valid              38      // 1 Bit, Bit 4
#define     LOG_fE2Low3ValidMask 0x10
#define     LOG_fE2Low3ValidShift 4
#define LOG_fE2Low4Valid              38      // 1 Bit, Bit 3
#define     LOG_fE2Low4ValidMask 0x08
#define     LOG_fE2Low4ValidShift 3
#define LOG_fE2Low5Valid              38      // 1 Bit, Bit 2
#define     LOG_fE2Low5ValidMask 0x04
#define     LOG_fE2Low5ValidShift 2
#define LOG_fE2Low6Valid              38      // 1 Bit, Bit 1
#define     LOG_fE2Low6ValidMask 0x02
#define     LOG_fE2Low6ValidShift 1
#define LOG_fE2Low7Valid              38      // 1 Bit, Bit 0
#define     LOG_fE2Low7ValidMask 0x01
#define     LOG_fE2Low7ValidShift 0
#define LOG_fE2Low0Dpt2               31      // 8 Bits, Bit 7-0
#define LOG_fE2Low1Dpt2               32      // 8 Bits, Bit 7-0
#define LOG_fE2Low2Dpt2               33      // 8 Bits, Bit 7-0
#define LOG_fE2Low3Dpt2               34      // 8 Bits, Bit 7-0
#define LOG_fE2LowDpt2Fix             31      // 8 Bits, Bit 7-0
#define LOG_fE2LowDpt5                31      // uint8_t
#define LOG_fE2HighDpt5               35      // uint8_t
#define LOG_fE2Low0Dpt5In             31      // uint8_t
#define LOG_fE2Low1Dpt5In             32      // uint8_t
#define LOG_fE2Low2Dpt5In             33      // uint8_t
#define LOG_fE2Low3Dpt5In             34      // uint8_t
#define LOG_fE2Low4Dpt5In             35      // uint8_t
#define LOG_fE2Low5Dpt5In             36      // uint8_t
#define LOG_fE2Low6Dpt5In             37      // uint8_t
#define LOG_fE2LowDpt5Fix             31      // uint8_t
#define LOG_fE2LowDpt5001             31      // uint8_t
#define LOG_fE2HighDpt5001            35      // uint8_t
#define LOG_fE2Low0Dpt5xIn            31      // uint8_t
#define LOG_fE2Low1Dpt5xIn            32      // uint8_t
#define LOG_fE2Low2Dpt5xIn            33      // uint8_t
#define LOG_fE2Low3Dpt5xIn            34      // uint8_t
#define LOG_fE2Low4Dpt5xIn            35      // uint8_t
#define LOG_fE2Low5Dpt5xIn            36      // uint8_t
#define LOG_fE2Low6Dpt5xIn            37      // uint8_t
#define LOG_fE2LowDpt5xFix            31      // uint8_t
#define LOG_fE2LowDpt6                31      // int8_t
#define LOG_fE2HighDpt6               35      // int8_t
#define LOG_fE2Low0Dpt6In             31      // int8_t
#define LOG_fE2Low1Dpt6In             32      // int8_t
#define LOG_fE2Low2Dpt6In             33      // int8_t
#define LOG_fE2Low3Dpt6In             34      // int8_t
#define LOG_fE2Low4Dpt6In             35      // int8_t
#define LOG_fE2Low5Dpt6In             36      // int8_t
#define LOG_fE2Low6Dpt6In             37      // int8_t
#define LOG_fE2LowDpt6Fix             31      // int8_t
#define LOG_fE2LowDpt7                31      // uint16_t
#define LOG_fE2HighDpt7               35      // uint16_t
#define LOG_fE2Low0Dpt7In             31      // uint16_t
#define LOG_fE2Low1Dpt7In             33      // uint16_t
#define LOG_fE2Low2Dpt7In             35      // uint16_t
#define LOG_fE2LowDpt7Fix             31      // uint16_t
#define LOG_fE2LowDpt8                31      // int16_t
#define LOG_fE2HighDpt8               35      // int16_t
#define LOG_fE2Low0Dpt8In             31      // int16_t
#define LOG_fE2Low1Dpt8In             33      // int16_t
#define LOG_fE2Low2Dpt8In             35      // int16_t
#define LOG_fE2LowDpt8Fix             31      // int16_t
#define LOG_fE2LowDpt9                31      // float
#define LOG_fE2HighDpt9               35      // float
#define LOG_fE2LowDpt9Fix             31      // float
#define LOG_fE2Low0Dpt17              31      // 8 Bits, Bit 7-0
#define LOG_fE2Low1Dpt17              32      // 8 Bits, Bit 7-0
#define LOG_fE2Low2Dpt17              33      // 8 Bits, Bit 7-0
#define LOG_fE2Low3Dpt17              34      // 8 Bits, Bit 7-0
#define LOG_fE2Low4Dpt17              35      // 8 Bits, Bit 7-0
#define LOG_fE2Low5Dpt17              36      // 8 Bits, Bit 7-0
#define LOG_fE2Low6Dpt17              37      // 8 Bits, Bit 7-0
#define LOG_fE2Low7Dpt17              38      // 8 Bits, Bit 7-0
#define LOG_fE2LowDpt17Fix            31      // 8 Bits, Bit 7-0
#define LOG_fE2LowDptRGB              31      // int32_t
#define LOG_fE2HighDptRGB             35      // int32_t
#define LOG_fE2LowDptRGBFix           31      // int32_t
#define LOG_fTd1Value                 23      // 1 Bit, Bit 7
#define     LOG_fTd1ValueMask 0x80
#define     LOG_fTd1ValueShift 7
#define LOG_fTd1HourAbs               23      // 5 Bits, Bit 5-1
#define     LOG_fTd1HourAbsMask 0x3E
#define     LOG_fTd1HourAbsShift 1
#define LOG_fTd1HourRel               23      // 5 Bits, Bit 5-1
#define     LOG_fTd1HourRelMask 0x3E
#define     LOG_fTd1HourRelShift 1
#define LOG_fTd1MinuteAbs             23      // 6 Bits, Bit 0--5
#define LOG_fTd1MinuteRel             23      // 6 Bits, Bit 0--5
#define LOG_fTd1Weekday               24      // 3 Bits, Bit 2-0
#define     LOG_fTd1WeekdayMask 0x07
#define     LOG_fTd1WeekdayShift 0
#define LOG_fTd2Value                 25      // 1 Bit, Bit 7
#define     LOG_fTd2ValueMask 0x80
#define     LOG_fTd2ValueShift 7
#define LOG_fTd2HourAbs               25      // 5 Bits, Bit 5-1
#define     LOG_fTd2HourAbsMask 0x3E
#define     LOG_fTd2HourAbsShift 1
#define LOG_fTd2HourRel               25      // 5 Bits, Bit 5-1
#define     LOG_fTd2HourRelMask 0x3E
#define     LOG_fTd2HourRelShift 1
#define LOG_fTd2MinuteAbs             25      // 6 Bits, Bit 0--5
#define LOG_fTd2MinuteRel             25      // 6 Bits, Bit 0--5
#define LOG_fTd2Weekday               26      // 3 Bits, Bit 2-0
#define     LOG_fTd2WeekdayMask 0x07
#define     LOG_fTd2WeekdayShift 0
#define LOG_fTd3Value                 27      // 1 Bit, Bit 7
#define     LOG_fTd3ValueMask 0x80
#define     LOG_fTd3ValueShift 7
#define LOG_fTd3HourAbs               27      // 5 Bits, Bit 5-1
#define     LOG_fTd3HourAbsMask 0x3E
#define     LOG_fTd3HourAbsShift 1
#define LOG_fTd3HourRel               27      // 5 Bits, Bit 5-1
#define     LOG_fTd3HourRelMask 0x3E
#define     LOG_fTd3HourRelShift 1
#define LOG_fTd3MinuteAbs             27      // 6 Bits, Bit 0--5
#define LOG_fTd3MinuteRel             27      // 6 Bits, Bit 0--5
#define LOG_fTd3Weekday               28      // 3 Bits, Bit 2-0
#define     LOG_fTd3WeekdayMask 0x07
#define     LOG_fTd3WeekdayShift 0
#define LOG_fTd4Value                 29      // 1 Bit, Bit 7
#define     LOG_fTd4ValueMask 0x80
#define     LOG_fTd4ValueShift 7
#define LOG_fTd4HourAbs               29      // 5 Bits, Bit 5-1
#define     LOG_fTd4HourAbsMask 0x3E
#define     LOG_fTd4HourAbsShift 1
#define LOG_fTd4HourRel               29      // 5 Bits, Bit 5-1
#define     LOG_fTd4HourRelMask 0x3E
#define     LOG_fTd4HourRelShift 1
#define LOG_fTd4MinuteAbs             29      // 6 Bits, Bit 0--5
#define LOG_fTd4MinuteRel             29      // 6 Bits, Bit 0--5
#define LOG_fTd4Weekday               30      // 3 Bits, Bit 2-0
#define     LOG_fTd4WeekdayMask 0x07
#define     LOG_fTd4WeekdayShift 0
#define LOG_fTd5Value                 31      // 1 Bit, Bit 7
#define     LOG_fTd5ValueMask 0x80
#define     LOG_fTd5ValueShift 7
#define LOG_fTd5HourAbs               31      // 5 Bits, Bit 5-1
#define     LOG_fTd5HourAbsMask 0x3E
#define     LOG_fTd5HourAbsShift 1
#define LOG_fTd5HourRel               31      // 5 Bits, Bit 5-1
#define     LOG_fTd5HourRelMask 0x3E
#define     LOG_fTd5HourRelShift 1
#define LOG_fTd5MinuteAbs             31      // 6 Bits, Bit 0--5
#define LOG_fTd5MinuteRel             31      // 6 Bits, Bit 0--5
#define LOG_fTd5Weekday               32      // 3 Bits, Bit 2-0
#define     LOG_fTd5WeekdayMask 0x07
#define     LOG_fTd5WeekdayShift 0
#define LOG_fTd6Value                 33      // 1 Bit, Bit 7
#define     LOG_fTd6ValueMask 0x80
#define     LOG_fTd6ValueShift 7
#define LOG_fTd6HourAbs               33      // 5 Bits, Bit 5-1
#define     LOG_fTd6HourAbsMask 0x3E
#define     LOG_fTd6HourAbsShift 1
#define LOG_fTd6HourRel               33      // 5 Bits, Bit 5-1
#define     LOG_fTd6HourRelMask 0x3E
#define     LOG_fTd6HourRelShift 1
#define LOG_fTd6MinuteAbs             33      // 6 Bits, Bit 0--5
#define LOG_fTd6MinuteRel             33      // 6 Bits, Bit 0--5
#define LOG_fTd6Weekday               34      // 3 Bits, Bit 2-0
#define     LOG_fTd6WeekdayMask 0x07
#define     LOG_fTd6WeekdayShift 0
#define LOG_fTd7Value                 35      // 1 Bit, Bit 7
#define     LOG_fTd7ValueMask 0x80
#define     LOG_fTd7ValueShift 7
#define LOG_fTd7HourAbs               35      // 5 Bits, Bit 5-1
#define     LOG_fTd7HourAbsMask 0x3E
#define     LOG_fTd7HourAbsShift 1
#define LOG_fTd7HourRel               35      // 5 Bits, Bit 5-1
#define     LOG_fTd7HourRelMask 0x3E
#define     LOG_fTd7HourRelShift 1
#define LOG_fTd7MinuteAbs             35      // 6 Bits, Bit 0--5
#define LOG_fTd7MinuteRel             35      // 6 Bits, Bit 0--5
#define LOG_fTd7Weekday               36      // 3 Bits, Bit 2-0
#define     LOG_fTd7WeekdayMask 0x07
#define     LOG_fTd7WeekdayShift 0
#define LOG_fTd8Value                 37      // 1 Bit, Bit 7
#define     LOG_fTd8ValueMask 0x80
#define     LOG_fTd8ValueShift 7
#define LOG_fTd8HourAbs               37      // 5 Bits, Bit 5-1
#define     LOG_fTd8HourAbsMask 0x3E
#define     LOG_fTd8HourAbsShift 1
#define LOG_fTd8HourRel               37      // 5 Bits, Bit 5-1
#define     LOG_fTd8HourRelMask 0x3E
#define     LOG_fTd8HourRelShift 1
#define LOG_fTd8MinuteAbs             37      // 6 Bits, Bit 0--5
#define LOG_fTd8MinuteRel             37      // 6 Bits, Bit 0--5
#define LOG_fTd8Weekday               38      // 3 Bits, Bit 2-0
#define     LOG_fTd8WeekdayMask 0x07
#define     LOG_fTd8WeekdayShift 0
#define LOG_fTy1Weekday1              31      // 1 Bit, Bit 7
#define     LOG_fTy1Weekday1Mask 0x80
#define     LOG_fTy1Weekday1Shift 7
#define LOG_fTy1Weekday2              31      // 1 Bit, Bit 6
#define     LOG_fTy1Weekday2Mask 0x40
#define     LOG_fTy1Weekday2Shift 6
#define LOG_fTy1Weekday3              31      // 1 Bit, Bit 5
#define     LOG_fTy1Weekday3Mask 0x20
#define     LOG_fTy1Weekday3Shift 5
#define LOG_fTy1Weekday4              31      // 1 Bit, Bit 4
#define     LOG_fTy1Weekday4Mask 0x10
#define     LOG_fTy1Weekday4Shift 4
#define LOG_fTy1Weekday5              31      // 1 Bit, Bit 3
#define     LOG_fTy1Weekday5Mask 0x08
#define     LOG_fTy1Weekday5Shift 3
#define LOG_fTy1Weekday6              31      // 1 Bit, Bit 2
#define     LOG_fTy1Weekday6Mask 0x04
#define     LOG_fTy1Weekday6Shift 2
#define LOG_fTy1Weekday7              31      // 1 Bit, Bit 1
#define     LOG_fTy1Weekday7Mask 0x02
#define     LOG_fTy1Weekday7Shift 1
#define LOG_fTy1Day                   31      // 7 Bits, Bit 7-1
#define LOG_fTy1IsWeekday             31      // 1 Bit, Bit 0
#define     LOG_fTy1IsWeekdayMask 0x01
#define     LOG_fTy1IsWeekdayShift 0
#define LOG_fTy1Month                 32      // 4 Bits, Bit 7-4
#define     LOG_fTy1MonthMask 0xF0
#define     LOG_fTy1MonthShift 4
#define LOG_fTy2Weekday1              33      // 1 Bit, Bit 7
#define     LOG_fTy2Weekday1Mask 0x80
#define     LOG_fTy2Weekday1Shift 7
#define LOG_fTy2Weekday2              33      // 1 Bit, Bit 6
#define     LOG_fTy2Weekday2Mask 0x40
#define     LOG_fTy2Weekday2Shift 6
#define LOG_fTy2Weekday3              33      // 1 Bit, Bit 5
#define     LOG_fTy2Weekday3Mask 0x20
#define     LOG_fTy2Weekday3Shift 5
#define LOG_fTy2Weekday4              33      // 1 Bit, Bit 4
#define     LOG_fTy2Weekday4Mask 0x10
#define     LOG_fTy2Weekday4Shift 4
#define LOG_fTy2Weekday5              33      // 1 Bit, Bit 3
#define     LOG_fTy2Weekday5Mask 0x08
#define     LOG_fTy2Weekday5Shift 3
#define LOG_fTy2Weekday6              33      // 1 Bit, Bit 2
#define     LOG_fTy2Weekday6Mask 0x04
#define     LOG_fTy2Weekday6Shift 2
#define LOG_fTy2Weekday7              33      // 1 Bit, Bit 1
#define     LOG_fTy2Weekday7Mask 0x02
#define     LOG_fTy2Weekday7Shift 1
#define LOG_fTy2Day                   33      // 7 Bits, Bit 7-1
#define LOG_fTy2IsWeekday             33      // 1 Bit, Bit 0
#define     LOG_fTy2IsWeekdayMask 0x01
#define     LOG_fTy2IsWeekdayShift 0
#define LOG_fTy2Month                 34      // 4 Bits, Bit 7-4
#define     LOG_fTy2MonthMask 0xF0
#define     LOG_fTy2MonthShift 4
#define LOG_fTy3Weekday1              35      // 1 Bit, Bit 7
#define     LOG_fTy3Weekday1Mask 0x80
#define     LOG_fTy3Weekday1Shift 7
#define LOG_fTy3Weekday2              35      // 1 Bit, Bit 6
#define     LOG_fTy3Weekday2Mask 0x40
#define     LOG_fTy3Weekday2Shift 6
#define LOG_fTy3Weekday3              35      // 1 Bit, Bit 5
#define     LOG_fTy3Weekday3Mask 0x20
#define     LOG_fTy3Weekday3Shift 5
#define LOG_fTy3Weekday4              35      // 1 Bit, Bit 4
#define     LOG_fTy3Weekday4Mask 0x10
#define     LOG_fTy3Weekday4Shift 4
#define LOG_fTy3Weekday5              35      // 1 Bit, Bit 3
#define     LOG_fTy3Weekday5Mask 0x08
#define     LOG_fTy3Weekday5Shift 3
#define LOG_fTy3Weekday6              35      // 1 Bit, Bit 2
#define     LOG_fTy3Weekday6Mask 0x04
#define     LOG_fTy3Weekday6Shift 2
#define LOG_fTy3Weekday7              35      // 1 Bit, Bit 1
#define     LOG_fTy3Weekday7Mask 0x02
#define     LOG_fTy3Weekday7Shift 1
#define LOG_fTy3Day                   35      // 7 Bits, Bit 7-1
#define LOG_fTy3IsWeekday             35      // 1 Bit, Bit 0
#define     LOG_fTy3IsWeekdayMask 0x01
#define     LOG_fTy3IsWeekdayShift 0
#define LOG_fTy3Month                 36      // 4 Bits, Bit 7-4
#define     LOG_fTy3MonthMask 0xF0
#define     LOG_fTy3MonthShift 4
#define LOG_fTy4Weekday1              37      // 1 Bit, Bit 7
#define     LOG_fTy4Weekday1Mask 0x80
#define     LOG_fTy4Weekday1Shift 7
#define LOG_fTy4Weekday2              37      // 1 Bit, Bit 6
#define     LOG_fTy4Weekday2Mask 0x40
#define     LOG_fTy4Weekday2Shift 6
#define LOG_fTy4Weekday3              37      // 1 Bit, Bit 5
#define     LOG_fTy4Weekday3Mask 0x20
#define     LOG_fTy4Weekday3Shift 5
#define LOG_fTy4Weekday4              37      // 1 Bit, Bit 4
#define     LOG_fTy4Weekday4Mask 0x10
#define     LOG_fTy4Weekday4Shift 4
#define LOG_fTy4Weekday5              37      // 1 Bit, Bit 3
#define     LOG_fTy4Weekday5Mask 0x08
#define     LOG_fTy4Weekday5Shift 3
#define LOG_fTy4Weekday6              37      // 1 Bit, Bit 2
#define     LOG_fTy4Weekday6Mask 0x04
#define     LOG_fTy4Weekday6Shift 2
#define LOG_fTy4Weekday7              37      // 1 Bit, Bit 1
#define     LOG_fTy4Weekday7Mask 0x02
#define     LOG_fTy4Weekday7Shift 1
#define LOG_fTy4Day                   37      // 7 Bits, Bit 7-1
#define LOG_fTy4IsWeekday             37      // 1 Bit, Bit 0
#define     LOG_fTy4IsWeekdayMask 0x01
#define     LOG_fTy4IsWeekdayShift 0
#define LOG_fTy4Month                 38      // 4 Bits, Bit 7-4
#define     LOG_fTy4MonthMask 0xF0
#define     LOG_fTy4MonthShift 4
#define LOG_fI1                       39      // 4 Bits, Bit 7-4
#define     LOG_fI1Mask 0xF0
#define     LOG_fI1Shift 4
#define LOG_fI2                       39      // 4 Bits, Bit 3-0
#define     LOG_fI2Mask 0x0F
#define     LOG_fI2Shift 0
#define LOG_fI1Function               40      // uint8_t
#define LOG_fI2Function               41      // uint8_t
#define LOG_fOTimeBase                42      // 8 Bits, Bit 7-0
#define LOG_fOTime                    43      // int32_t
#define LOG_fOBlink                   47      // int32_t
#define LOG_fODelay                   51      // 1 Bit, Bit 7
#define     LOG_fODelayMask 0x80
#define     LOG_fODelayShift 7
#define LOG_fODelayOnRepeat           51      // 2 Bits, Bit 6-5
#define     LOG_fODelayOnRepeatMask 0x60
#define     LOG_fODelayOnRepeatShift 5
#define LOG_fODelayOnReset            51      // 1 Bit, Bit 4
#define     LOG_fODelayOnResetMask 0x10
#define     LOG_fODelayOnResetShift 4
#define LOG_fODelayOffRepeat          51      // 2 Bits, Bit 3-2
#define     LOG_fODelayOffRepeatMask 0x0C
#define     LOG_fODelayOffRepeatShift 2
#define LOG_fODelayOffReset           51      // 1 Bit, Bit 1
#define     LOG_fODelayOffResetMask 0x02
#define     LOG_fODelayOffResetShift 1
#define LOG_fODelayOn                 52      // int32_t
#define LOG_fODelayOff                56      // int32_t
#define LOG_fOStair                   60      // 1 Bit, Bit 7
#define     LOG_fOStairMask 0x80
#define     LOG_fOStairShift 7
#define LOG_fORetrigger               60      // 1 Bit, Bit 6
#define     LOG_fORetriggerMask 0x40
#define     LOG_fORetriggerShift 6
#define LOG_fOStairOff                60      // 1 Bit, Bit 5
#define     LOG_fOStairOffMask 0x20
#define     LOG_fOStairOffShift 5
#define LOG_fORepeat                  60      // 1 Bit, Bit 4
#define     LOG_fORepeatMask 0x10
#define     LOG_fORepeatShift 4
#define LOG_fOOutputFilter            60      // 2 Bits, Bit 3-2
#define     LOG_fOOutputFilterMask 0x0C
#define     LOG_fOOutputFilterShift 2
#define LOG_fORepeatOn                61      // int32_t
#define LOG_fORepeatOff               65      // int32_t
#define LOG_fODpt                     69      // 8 Bits, Bit 7-0
#define LOG_fOOn                      70      // 8 Bits, Bit 7-0
#define LOG_fOOnBuzzer                70      // 8 Bits, Bit 7-0
#define LOG_fOOnLed                   70      // 8 Bits, Bit 7-0
#define LOG_fOOnAll                   70      // 8 Bits, Bit 7-0
#define LOG_fOOnTone                  71      // 8 Bits, Bit 7-0
#define LOG_fOOnDpt1                  71      // 8 Bits, Bit 7-0
#define LOG_fOOnDpt2                  71      // 8 Bits, Bit 7-0
#define LOG_fOOnDpt5                  71      // uint8_t
#define LOG_fOOnDpt5001               71      // uint8_t
#define LOG_fOOnDpt6                  71      // int8_t
#define LOG_fOOnDpt7                  71      // uint16_t
#define LOG_fOOnDpt8                  71      // int16_t
#define LOG_fOOnDpt9                  71      // float
#define LOG_fOOnDpt16                 71      // char*, 14 Byte
#define LOG_fOOnDpt17                 71      // 8 Bits, Bit 7-0
#define LOG_fOOnRGB                   71      // color, uint, 3 Byte
#define LOG_fOOnPAArea                71      // 4 Bits, Bit 7-4
#define     LOG_fOOnPAAreaMask 0xF0
#define     LOG_fOOnPAAreaShift 4
#define LOG_fOOnPALine                71      // 4 Bits, Bit 3-0
#define     LOG_fOOnPALineMask 0x0F
#define     LOG_fOOnPALineShift 0
#define LOG_fOOnPADevice              72      // uint8_t
#define LOG_fOOnFunction              71      // 8 Bits, Bit 7-0
#define LOG_fOOff                     85      // 8 Bits, Bit 7-0
#define LOG_fOOffBuzzer               85      // 8 Bits, Bit 7-0
#define LOG_fOOffLed                  85      // 8 Bits, Bit 7-0
#define LOG_fOOffAll                  85      // 8 Bits, Bit 7-0
#define LOG_fOOffTone                 86      // 8 Bits, Bit 7-0
#define LOG_fOOffDpt1                 86      // 8 Bits, Bit 7-0
#define LOG_fOOffDpt2                 86      // 8 Bits, Bit 7-0
#define LOG_fOOffDpt5                 86      // uint8_t
#define LOG_fOOffDpt5001              86      // uint8_t
#define LOG_fOOffDpt6                 86      // int8_t
#define LOG_fOOffDpt7                 86      // uint16_t
#define LOG_fOOffDpt8                 86      // int16_t
#define LOG_fOOffDpt9                 86      // float
#define LOG_fOOffDpt16                86      // char*, 14 Byte
#define LOG_fOOffDpt17                86      // 8 Bits, Bit 7-0
#define LOG_fOOffRGB                  86      // color, uint, 3 Byte
#define LOG_fOOffPAArea               86      // 4 Bits, Bit 7-4
#define     LOG_fOOffPAAreaMask 0xF0
#define     LOG_fOOffPAAreaShift 4
#define LOG_fOOffPALine               86      // 4 Bits, Bit 3-0
#define     LOG_fOOffPALineMask 0x0F
#define     LOG_fOOffPALineShift 0
#define LOG_fOOffPADevice             87      // uint8_t
#define LOG_fOOffFunction             86      // 8 Bits, Bit 7-0

// Communication objects per channel (multiple occurance)
#define LOG_KoOffset 20
#define LOG_KoBlockSize 3
#define LOG_KoKOfE1 0
#define LOG_KoKOfE2 1
#define LOG_KoKOfO 2

// Communication objects with single occurance
#define LOG_KoHeartbeat 1
#define LOG_KoTime 2
#define LOG_KoDate 3
#define LOG_KoVacation 4
#define LOG_KoHoliday1 5
#define LOG_KoHoliday2 6
#define LOG_KoDiagnose 7
#define LOG_KoLedLock 8
#define LOG_KoBuzzerLock 9

