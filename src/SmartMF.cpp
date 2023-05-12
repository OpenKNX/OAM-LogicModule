#include "SmartMF.h"

SmartMF::SmartMF()
{}

/*
 * Read hardware variant if pin are defined
 * Read hardware revision if pins are defined
 */
void SmartMF::init()
{
#ifdef SmartMF_HardwareRevision
    readHardwareRevision();
#endif
#ifdef SmartMF_HardwareVariant
    readHardwareVariant();
#endif
}

#ifdef SmartMF_HardwareRevision
void SmartMF::readHardwareRevision()
{
    pinMode(SmartMF_HardwareRevision_ID1, INPUT_PULLUP);
    pinMode(SmartMF_HardwareRevision_ID2, INPUT_PULLUP);
    pinMode(SmartMF_HardwareRevision_ID3, INPUT_PULLUP);

    uint8_t revision = 1;
    revision += (uint8_t)digitalRead(SmartMF_HardwareRevision_ID1) << 0;
    revision += (uint8_t)digitalRead(SmartMF_HardwareRevision_ID2) << 1;
    revision += (uint8_t)digitalRead(SmartMF_HardwareRevision_ID3) << 2;

    printf("SmartMF: Revision %i", revision);

    _hardwareRevision = revision;
}
#endif

#ifdef SmartMF_HardwareVariant
void SmartMF::readHardwareVariant()
{
    analogReadResolution(12);

    // Need to read one time before you got after a delay a correct value
    analogRead(SmartMF_HardwareVariant_PIN);
    delay(10);

    uint32_t variantRaw = analogRead(SmartMF_HardwareVariant_PIN);
    _hardwareVariant = mapHardwareVariant(variantRaw);
    printf("SmartMF: Variant %i (%i)", _hardwareVariant, variantRaw);
}

uint8_t SmartMF::mapHardwareVariant(uint32_t value)
{
    if (value >= 0 && 284 >= value)
        return 1;
    if (value >= 569 && 675 >= value)
        return 2;
    if (value >= 927 && 1078 >= value)
        return 3;
    if (value >= 1369 && 1557 >= value)
        return 4;
    if (value >= 1760 && 1963 >= value)
        return 5;
    if (value >= 2131 && 2334 >= value)
        return 6;
    if (value >= 2537 && 2725 >= value)
        return 7;
    if (value >= 3016 && 3167 >= value)
        return 8;
    if (value >= 3420 && 3525 >= value)
        return 9;
    if (value >= 3810 && 4095 >= value)
        return 10;

    return 0;
}
#endif

/*
 * Return SmartFM Hardware Revision
 */
uint8_t SmartMF::hardwareRevision()
{
    return _hardwareRevision;
}

/*
 * Return SmartFM Hardware Variant
 */
uint8_t SmartMF::hardwareVariant()
{
    return _hardwareVariant;
}

/* SmartFM Instance */
SmartMF smartMF;