#include "TimerRestore.h"
#include "Arduino.h"
#include "Helper.h"

TimerRestore::TimerRestore()
{
    mNow.tm_year = 120;
    mNow.tm_mon = 0;
    mNow.tm_mday = 1;
    mNow.tm_wday = 3;
    mktime(&mNow);
    mTimeDelay = millis();
}

TimerRestore::~TimerRestore()
{
}

TimerRestore &TimerRestore::instance() {
    static TimerRestore sInstance;
    return sInstance;
}

void TimerRestore::setup(Timer &iTimer) {
    mDayIteration = 0;
    mNow = iTimer.mNow;
    mLongitude = iTimer.mLongitude;
    mLatitude = iTimer.mLatitude;
    mTimezone = iTimer.mTimezone;
    mTimeValid = tmValid;
    calculateEaster();
    calculateAdvent();
    doDayCalculations();
}

void TimerRestore::decreaseDay() {
    mNow.tm_mday -= 1;
    // Jeder ältere Tag als "Heute" wird 
    // mit dem Tagesende (25:59:59) betrachtet
    mNow.tm_hour = 23; 
    mNow.tm_min = 59;
    mNow.tm_sec = 59;
    doDayCalculations();
}

void TimerRestore::doDayCalculations() {
    uint16_t lYear = mNow.tm_year;
    mktime(&mNow);
    mDayIteration += 1;
    // printDebug("TimerRestore: Day %02d.%02d.%02d\n", this->getDay(), this->getMonth(), this->getYear());
    calculateSummertime(); // initial summertime calculation if year changes
    calculateSunriseSunset();
    if (lYear != mNow.tm_year) {
        calculateEaster();
        calculateAdvent();
    }
    calculateHolidays();
}

uint16_t TimerRestore::getDayIteration() {
    return mDayIteration;
}