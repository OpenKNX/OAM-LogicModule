#include "Timer.h"
#include "Arduino.h"
#include "Helper.h"
#include <ctime>

sDay Timer::cHolidays[29] = {{1, 1}, {6, 1}, {-52, EASTER}, {-48, EASTER}, {-47, EASTER}, {-46, EASTER}, {-3, EASTER}, {-2, EASTER}, {0, EASTER}, {1, EASTER}, {1, 5}, {39, EASTER}, {49, EASTER}, {50, EASTER}, {60, EASTER}, {8, 8}, {15, 8}, {3, 10}, {31, 10}, {1, 11}, {-32, ADVENT}, {-21, ADVENT}, {-14, ADVENT}, {-7, ADVENT}, {0, ADVENT}, {24, 12}, {25, 12}, {26, 12}, {31, 12}};

Timer::Timer()
{
    mNow.tm_year = 120;
    mNow.tm_mon = 0;
    mNow.tm_mday = 1;
    mNow.tm_wday = 3;
    mktime(&mNow);
    mTimeDelay = millis();
}

Timer::~Timer()
{
}

Timer &Timer::instance() {
    static Timer sInstance;
    return sInstance;
}

void Timer::setup(double iLongitude, double iLatitude, int8_t iTimezone, bool iUseSummertime, uint32_t iHolidayBitmask) {

    mLongitude = iLongitude;
    mLatitude = iLatitude;
    mTimezone = iTimezone;
    mUseSummertime = iUseSummertime;
    mTimezone = iTimezone;
    // we delete all unnecessary holidays from holiday data
    for (uint8_t i = 0; i < 29; i++)
    {
        if ((iHolidayBitmask & 0x80000000) == 0)
            cHolidays[i].month = REMOVED;
        iHolidayBitmask <<= 1;
    }
}

void Timer::loop() {
    if (delayCheck(mTimeDelay, 1000))
    {
        mTimeDelay = millis();
        mNow.tm_sec += 1;
        mktime(&mNow);
        if (mTimeValid == tmValid)
        {
            if (mMinuteTick != mNow.tm_min)
            {
                mMinuteChanged = true;
                // just call once a minute
                mMinuteTick = mNow.tm_min;
                if (mUseSummertime && (getMonth() == 3 || getMonth() == 10) && getHour() == 3 && getMinute() == 1)
                    calculateSummertime();
            }
            if (mYearTick != mNow.tm_year)
            {
                calculateEaster();
                calculateAdvent();
                calculateSummertime(); // initial summertime calculation if year changes
                calculateHolidays();
                mYearTick = mNow.tm_year;
            }
            // important: Day calculations AFTER year calculations
            if (mDayTick != mNow.tm_mday)
            {
                calculateSunriseSunset();
                if (!mHolidayChanged)
                    calculateHolidays();
                mDayTick = mNow.tm_mday;
            }
        }
    }
}

void Timer::calculateSunriseSunset()
{
    double rise, set;
    // sunrise/sunset calculation
    sunRiseSet(getYear(), getMonth(), getDay(),
               mLongitude, mLatitude, 35.0 / 60.0, 1, &rise, &set);
    double lTmp;
    mSunrise.minute = round(modf(rise, &lTmp) * 60.0);
    mSunrise.hour = lTmp + mTimezone + ((mIsSummertime) ? 1 : 0);
    mSunset.minute = round(modf(set, &lTmp) * 60.0);
    mSunset.hour = lTmp + mTimezone + ((mIsSummertime) ? 1 : 0);
}

void Timer::setTimeFromBus(tm *iTime) {
    if (mNow.tm_min != iTime->tm_min || mNow.tm_hour != iTime->tm_hour)
        mMinuteChanged = true;
    mNow.tm_sec = iTime->tm_sec;
    mNow.tm_min = iTime->tm_min;
    mNow.tm_hour = iTime->tm_hour;
    mktime(&mNow);
    mTimeDelay = millis();
    mTimeValid = static_cast<eTimeValid>(mTimeValid | tmMinutesValid);
}

void Timer::setDateFromBus(tm *iDate) {
    // we have to check, if some date dependant calculations have to be done
    // in case of date changes
    if (iDate->tm_year != getYear())
    {
        mYearTick = -1; // triggers easter calculation
        mDayTick = -1;    // triggers sunrise/sunset calculation
        mMinuteChanged = true;
    }
    else if (iDate->tm_mon != getMonth() || iDate->tm_mday != getDay())
    {
        mDayTick = -1; // triggers sunrise/sunset calculation
        mMinuteChanged = true;
    }
    mNow.tm_mday = iDate->tm_mday;
    mNow.tm_mon = iDate->tm_mon - 1;
    mNow.tm_year = iDate->tm_year - 1900;
    mktime(&mNow);
    mTimeDelay = millis();
    mTimeValid = static_cast<eTimeValid>(mTimeValid | tmDateValid);
}

bool Timer::minuteChanged() {
    return mMinuteChanged && mTimeValid == tmValid;
}

void Timer::clearMinuteChanged() {
    mMinuteChanged = false;
}

uint16_t Timer::getYear()
{
    return mNow.tm_year + 1900;
}

uint8_t Timer::getMonth()
{
    return mNow.tm_mon + 1;
}

uint8_t Timer::getDay()
{
    return mNow.tm_mday;
}

uint8_t Timer::getHour()
{
    return mNow.tm_hour;
}

uint8_t Timer::getMinute()
{
    return mNow.tm_min;
}

uint8_t Timer::getSecond()
{
    return mNow.tm_sec;
}

uint8_t Timer::getWeekday()
{
    return mNow.tm_wday;
}

sTime *Timer::getSunInfo(uint8_t iSunInfo)
{
    if (iSunInfo == SUN_SUNRISE)
        return &mSunrise;
    else if (iSunInfo == SUN_SUNSET)
        return &mSunset;
    else
        return NULL;
}

sDay *Timer::getEaster() {
    return &mEaster;
}

char *Timer::getTimeAsc() {
    return asctime(&mNow);
}

uint8_t Timer::holidayToday() {
    return mHolidayToday;
}

uint8_t Timer::holidayTomorrow() {
    return mHolidayTomorrow;
}

bool Timer::holidayChanged() {
    return mHolidayChanged;
}

void Timer::clearHolidayChanged() {
    mHolidayChanged = false;
}

eTimeValid Timer::isTimerValid() {
    return mTimeValid;
}

uint8_t Timer::calculateLastSundayInMonth(uint8_t iMonth) {
    mTimeHelper.tm_year = mNow.tm_year;
    mTimeHelper.tm_mon = iMonth - 1;
    mTimeHelper.tm_mday = 31;
    mktime(&mTimeHelper);
    return mTimeHelper.tm_mday - mTimeHelper.tm_wday;
}

// should be called only at 03:01 o'clock
void Timer::calculateSummertime() {
    // first we do easy win
    mIsSummertime = false;
    if (mUseSummertime) {
      if (getMonth() == 3) {
          // find last Sunday in March
          uint8_t lLastSunday = calculateLastSundayInMonth(3);
          if (lLastSunday == mNow.tm_mday) {
              // we have to take time into account
              mIsSummertime = (mNow.tm_hour > 3);
          } else {
              mIsSummertime = (lLastSunday < mNow.tm_mday);
          }
      } else if (getMonth() == 10) {
          // find last Sunday in October
          uint8_t lLastSunday = calculateLastSundayInMonth(10);
          if (lLastSunday == mNow.tm_mday)
          {
              // we have to take time into account
              // here might be a problem if called between 
              // 2 and 3, because these times exist in both
              // summer and wintertime. This is currently
              // mitigated with the fact, that this routine
              // is called only once at 03:01. Currently just used
              // for sunrise/sunset calculation, so calling
              // time is no problem.
              mIsSummertime = (mNow.tm_hour < 3);
          }
          else
          {
              mIsSummertime = (lLastSunday > mNow.tm_mday);
          }
      } else {
          mIsSummertime = (getMonth() > 3 && getMonth() < 10);
      }
    }
}

void Timer::calculateAdvent() {
    // calculates the 4th advent
    mTimeHelper.tm_year = mNow.tm_year;
    mTimeHelper.tm_mon = 11;
    mTimeHelper.tm_mday = 24;
    mTimeHelper.tm_hour = 12;
    mTimeHelper.tm_min = 0;
    mTimeHelper.tm_sec = 0;
    mktime(&mTimeHelper); //   -timezone;
    mAdvent.day = 24 - mTimeHelper.tm_wday;
    mAdvent.month = 12;
}

void Timer::calculateEaster()
{
    uint16_t lYear = getYear();
    uint8_t a = lYear % 19;
    uint8_t b = lYear % 4;
    uint8_t c = lYear % 7;

    uint8_t k = lYear / 100;
    uint8_t q = k / 4;
    uint8_t p = ((8 * k) + 13) / 25;
    uint8_t Egz = (38 - (k - q) + p) % 30; // Die Jahrhundertepakte
    uint8_t M = (53 - Egz) % 30;
    uint8_t N = (4 + k - q) % 7;

    uint8_t d = ((19 * a) + M) % 30;
    uint8_t e = ((2 * b) + (4 * c) + (6 * d) + N) % 7;

    // Ausrechnen des Ostertermins:
    if ((22 + d + e) <= 31)
    {
        mEaster.day = 22 + d + e;
        mEaster.month = 3;
    }
    else
    {
        mEaster.day = d + e - 9;
        mEaster.month = 4;

        // Zwei Ausnahmen berücksichtigen:
        if (mEaster.day == 26)
            mEaster.day = 19;
        else if ((mEaster.day == 25) && (d == 28) && (a > 10))
            mEaster.day = 18;
    }
}

void Timer::debug()
{
    if (mTimeValid & tmMinutesValid) {
        printDebug("Aktuelle Zeit: %s", getTimeAsc());
    }
#if LOGIC_TRACE
    if (mTimeValid & tmDateValid) {
        printDebug("\nFeiertage %d: ", getYear());
        calculateHolidays(true);
        printDebug("\nEnd of holiday debug\n");
        printDebug("Sonnenaufgang: %02d:%02d, Sonnenuntergang: %02d:%02d\n\n", mSunrise.hour, mSunrise.minute, mSunset.hour, mSunset.minute);
    }
#endif
}

void Timer::calculateHolidays(bool iDebugOutput) {
    // we check only if date is valid
    if (mTimeValid < tmDateValid)
        return;
    // check if today or tomorrow is a holiday
    sDay lToday = {(int8_t)getDay(), (int8_t)getMonth()};
    sDay lTomorrow = getDayByOffset(1, lToday);
    uint8_t lHolidayToday = 0;
    uint8_t lHolidayTomorrow = 0;
    for (uint8_t i = 0; i < 29; i++)
    {
        sDay lHoliday = {REMOVED, REMOVED};
        switch (cHolidays[i].month)
        {
            case REMOVED:
                // do nothing
                break;
            case EASTER:
                lHoliday = getDayByOffset(cHolidays[i].day, mEaster);
                break;
            case ADVENT:
                lHoliday = getDayByOffset(cHolidays[i].day, mAdvent);
                // do nothing
                break;
            default:
                // constant holiday
                lHoliday = cHolidays[i];
                break;
        }
        if (lHoliday.month > REMOVED) {
            if (iDebugOutput)
                printDebug("%02d.%02d., ", lHoliday.day, lHoliday.month);
            if (isEqualDate(lHoliday, lToday))
                lHolidayToday = i + 1;
            if (isEqualDate(lHoliday, lTomorrow))
                lHolidayTomorrow = i + 1;
            if (lHolidayToday > 0 && lHolidayTomorrow > 0 && !iDebugOutput)
                break;
        }
    }
    if (lHolidayToday != mHolidayToday) {
        mHolidayToday = lHolidayToday;
        mHolidayChanged = true;
    }
    if (lHolidayTomorrow != mHolidayTomorrow) {
        mHolidayTomorrow = lHolidayTomorrow;
        mHolidayChanged = true;
    }
}

bool Timer::isEqualDate(sDay &iDate1, sDay &iDate2) {
    return (iDate1.day == iDate2.day && iDate1.month == iDate2.month);
}

sDay Timer::getDayByOffset(int8_t iOffset, sDay &iDate) {
    mTimeHelper.tm_year = mNow.tm_year;
    mTimeHelper.tm_mon = iDate.month - 1;
    mTimeHelper.tm_mday = iDate.day + iOffset;
    mTimeHelper.tm_hour = 12;
    mTimeHelper.tm_min = 0;
    mTimeHelper.tm_sec = 0;

    // save a little time, if we are for sure within same month
    if (mTimeHelper.tm_mday < 1 || mTimeHelper.tm_mday > 28)
        mktime(&mTimeHelper); //   -timezone;

    //time_t nt_seconds = mktime(&mTimeHelper);     //   -timezone;
    // return gmtime(&nt_seconds);

    sDay lResult = {(int8_t)mTimeHelper.tm_mday, (int8_t)(mTimeHelper.tm_mon + 1)};
    return lResult;
}

/***************************************************************************/
/* Note: year,month,date = calendar date, 1801-2099 only.             */
/*       Eastern longitude positive, Western longitude negative       */
/*       Northern latitude positive, Southern latitude negative       */
/*       The longitude value IS critical in this function!            */
/*       altit = the altitude which the Sun should cross              */
/*               Set to -35/60 degrees for rise/set, -6 degrees       */
/*               for civil, -12 degrees for nautical and -18          */
/*               degrees for astronomical twilight.                   */
/*         upper_limb: non-zero -> upper limb, zero -> center         */
/*               Set to non-zero (e.g. 1) when computing rise/set     */
/*               times, and to zero when computing start/end of       */
/*               twilight.                                            */
/*        *rise = where to store the rise time                        */
/*        *set  = where to store the set  time                        */
/*                Both times are relative to the specified altitude,  */
/*                and thus this function can be used to compute       */
/*                various twilight times, as well as rise/set times   */
/* Return value:  0 = sun rises/sets this day, times stored at        */
/*                    *trise and *tset.                               */
/*               +1 = sun above the specified "horizon" 24 hours.     */
/*                    *trise set to time when the sun is at south,    */
/*                    minus 12 hours while *tset is set to the south  */
/*                    time plus 12 hours. "Day" length = 24 hours     */
/*               -1 = sun is below the specified "horizon" 24 hours   */
/*                    "Day" length = 0 hours, *trise and *tset are    */
/*                    both set to the time when the sun is at south.  */
/*                                                                    */
/**********************************************************************/
int Timer::sunRiseSet(int year, int month, int day, double lon, double lat,
                      double altit, int upper_limb, double *trise, double *tset)
{
    double d,    /* Days since 2000 Jan 0.0 (negative before) */
        sr,      /* Solar distance, astronomical units */
        sRA,     /* Sun's Right Ascension */
        sdec,    /* Sun's declination */
        sradius, /* Sun's apparent radius */
        t,       /* Diurnal arc */
        tsouth,  /* Time when Sun is at south */
        sidtime; /* Local sidereal time */

    int rc = 0; /* Return cde from function - usually 0 */

    /* Compute d of 12h local mean solar time */
    d = days_since_2000_Jan_0(year, month, day) + 0.5 - lon / 360.0;

    /* Compute the local sidereal time of this moment */
    sidtime = revolution(GMST0(d) + 180.0 + lon);

    /* Compute Sun's RA, Decl and distance at this moment */
    sunRadDec(d, &sRA, &sdec, &sr);

    /* Compute time when Sun is at south - in hours UT */
    tsouth = 12.0 - rev180(sidtime - sRA) / 15.0;

    /* Compute the Sun's apparent radius in degrees */
    sradius = 0.2666 / sr;

    /* Do correction to upper limb, if necessary */
    if (upper_limb)
        altit -= sradius;

    /* Compute the diurnal arc that the Sun traverses to reach */
    /* the specified altitude altit: */
    {
        double cost;
        cost = (sind(altit) - sind(lat) * sind(sdec)) /
               (cosd(lat) * cosd(sdec));
        if (cost >= 1.0)
            rc = -1, t = 0.0; /* Sun always below altit */
        else if (cost <= -1.0)
            rc = +1, t = 12.0; /* Sun always above altit */
        else
            t = acosd(cost) / 15.0; /* The diurnal arc, hours */
    }

    /* Store rise and set times - in hours UT */
    *trise = tsouth - t;
    *tset = tsouth + t;

    return rc;
}

/******************************************************/
/* Computes the Sun's ecliptic longitude and distance */
/* at an instant given in d, number of days since     */
/* 2000 Jan 0.0.  The Sun's ecliptic latitude is not  */
/* computed, since it's always very near 0.           */
/******************************************************/
void Timer::sunPos(double d, double *lon, double *r)
{
    double M, /* Mean anomaly of the Sun */
        w,    /* Mean longitude of perihelion */
              /* Note: Sun's mean longitude = M + w */
        e,    /* Eccentricity of Earth's orbit */
        E,    /* Eccentric anomaly */
        x, y, /* x, y coordinates in orbit */
        v;    /* True anomaly */

    /* Compute mean elements */
    M = revolution(356.0470 + 0.9856002585 * d);
    w = 282.9404 + 4.70935E-5 * d;
    e = 0.016709 - 1.151E-9 * d;

    /* Compute true longitude and radius vector */
    E = M + e * RADEG * sind(M) * (1.0 + e * cosd(M));
    x = cosd(E) - e;
    y = sqrt(1.0 - e * e) * sind(E);
    *r = sqrt(x * x + y * y); /* Solar distance */
    v = atan2d(y, x);         /* True anomaly */
    *lon = v + w;             /* True solar longitude */
    if (*lon >= 360.0)
        *lon -= 360.0; /* Make it 0..360 degrees */
}

/******************************************************/
/* Computes the Sun's equatorial coordinates RA, Decl */
/* and also its distance, at an instant given in d,   */
/* the number of days since 2000 Jan 0.0.             */
/******************************************************/
void Timer::sunRadDec(double d, double *RA, double *dec, double *r)
{
    double lon, obl_ecl, x, y, z;

    /* Compute Sun's ecliptical coordinates */
    sunPos(d, &lon, r);

    /* Compute ecliptic rectangular coordinates (z=0) */
    x = *r * cosd(lon);
    y = *r * sind(lon);

    /* Compute obliquity of ecliptic (inclination of Earth's axis) */
    obl_ecl = 23.4393 - 3.563E-7 * d;

    /* Convert to equatorial rectangular coordinates - x is unchanged */
    z = y * sind(obl_ecl);
    y = y * cosd(obl_ecl);

    /* Convert to spherical coordinates */
    *RA = atan2d(y, x);
    *dec = atan2d(z, sqrt(x * x + y * y));

}

/******************************************************************/
/* This function reduces any angle to within the first revolution */
/* by subtracting or adding even multiples of 360.0 until the     */
/* result is >= 0.0 and < 360.0                                   */
/******************************************************************/

#define INV360 (1.0 / 360.0)

/*****************************************/
/* Reduce angle to within 0..360 degrees */
/*****************************************/
double Timer::revolution(double x)
{
    return (x - 360.0 * floor(x * INV360));
}

/*********************************************/
/* Reduce angle to within +180..+180 degrees */
/*********************************************/
double Timer::rev180(double x)
{
    return (x - 360.0 * floor(x * INV360 + 0.5));
}

/*******************************************************************/
/* This function computes GMST0, the Greenwich Mean Sidereal Time  */
/* at 0h UT (i.e. the sidereal time at the Greenwhich meridian at  */
/* 0h UT).  GMST is then the sidereal time at Greenwich at any     */
/* time of the day.  I've generalized GMST0 as well, and define it */
/* as:  GMST0 = GMST - UT  --  this allows GMST0 to be computed at */
/* other times than 0h UT as well.  While this sounds somewhat     */
/* contradictory, it is very practical:  instead of computing      */
/* GMST like:                                                      */
/*                                                                 */
/*  GMST = (GMST0) + UT * (366.2422/365.2422)                      */
/*                                                                 */
/* where (GMST0) is the GMST last time UT was 0 hours, one simply  */
/* computes:                                                       */
/*                                                                 */
/*  GMST = GMST0 + UT                                              */
/*                                                                 */
/* where GMST0 is the GMST "at 0h UT" but at the current moment!   */
/* Defined in this way, GMST0 will increase with about 4 min a     */
/* day.  It also happens that GMST0 (in degrees, 1 hr = 15 degr)   */
/* is equal to the Sun's mean longitude plus/minus 180 degrees!    */
/* (if we neglect aberration, which amounts to 20 seconds of arc   */
/* or 1.33 seconds of time)                                        */
/*                                                                 */
/*******************************************************************/

double Timer::GMST0(double d)
{
    double sidtim0;
    /* Sidtime at 0h UT = L (Sun's mean longitude) + 180.0 degr  */
    /* L = M + w, as defined in sunpos().  Since I'm too lazy to */
    /* add these numbers, I'll let the C compiler do it for me.  */
    /* Any decent C compiler will add the constants at compile   */
    /* time, imposing no runtime or code overhead.               */
    sidtim0 = revolution((180.0 + 356.0470 + 282.9404) +
                         (0.9856002585 + 4.70935E-5) * d);
    return sidtim0;
} /* GMST0 */
