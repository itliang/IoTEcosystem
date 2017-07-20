/*
 * Copyright (c) 2000-2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance with the terms of the license agreement
 * you entered into with SAMSUNG ELECTRONICS.
 * SAMSUNG make no representations or warranties about the suitability
 * of the software, either express or implied, including but not
 * limited to the implied warranties of merchantability, fitness for
 * a particular purpose, or non-infringement.
 * SAMSUNG shall not be liable for any damages suffered by licensee as
 * a result of using, modifying or distributing this software or its derivatives.

 */

#include <util/Log.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <util/Time.h>

using namespace std;

Time::Time()
{

}

Time::Time(const struct tm& t)
{
    m_Tm = t;
}

Time::~Time()
{

}

void Time::SetTime(const struct tm& t)
{
    m_Tm = t;
}

void Time::SetTime(const time_t t)
{
    struct timeval tv;

    memset(&tv, 0x00, sizeof(tv));

    tv.tv_sec = t / 1000;
    tv.tv_usec = (t % 1000) * 1000;

    localtime_r(&(tv.tv_sec), &m_Tm);
}

bool Time::operator==(const Time& t) const
{
    return m_Tm.tm_year == t.m_Tm.tm_year && m_Tm.tm_mon == t.m_Tm.tm_mon
            && m_Tm.tm_mday == t.m_Tm.tm_mday && m_Tm.tm_hour == t.m_Tm.tm_hour
            && m_Tm.tm_min == t.m_Tm.tm_min && m_Tm.tm_sec == t.m_Tm.tm_sec;
}

bool Time::operator!=(const Time& t) const
{
    return !(*this == t);
}

bool Time::operator>(const Time& t) const
{
    if (m_Tm.tm_year > t.m_Tm.tm_year)
        return true;
    if (m_Tm.tm_year < t.m_Tm.tm_year)
        return false;
    if (m_Tm.tm_mon > t.m_Tm.tm_mon)
        return true;
    if (m_Tm.tm_mon < t.m_Tm.tm_mon)
        return false;
    if (m_Tm.tm_mday > t.m_Tm.tm_mday)
        return true;
    if (m_Tm.tm_mday < t.m_Tm.tm_mday)
        return false;
    if (m_Tm.tm_hour > t.m_Tm.tm_hour)
        return true;
    if (m_Tm.tm_hour < t.m_Tm.tm_hour)
        return false;
    if (m_Tm.tm_min > t.m_Tm.tm_min)
        return true;
    if (m_Tm.tm_min < t.m_Tm.tm_min)
        return false;
    if (m_Tm.tm_sec > t.m_Tm.tm_sec)
        return true;
    if (m_Tm.tm_sec < t.m_Tm.tm_sec)
        return false;

    return false;
}

bool Time::operator<(const Time& t) const
{
    if (m_Tm.tm_year < t.m_Tm.tm_year)
        return true;
    if (m_Tm.tm_year > t.m_Tm.tm_year)
        return false;
    if (m_Tm.tm_mon < t.m_Tm.tm_mon)
        return true;
    if (m_Tm.tm_mon > t.m_Tm.tm_mon)
        return false;
    if (m_Tm.tm_mday < t.m_Tm.tm_mday)
        return true;
    if (m_Tm.tm_mday > t.m_Tm.tm_mday)
        return false;
    if (m_Tm.tm_hour < t.m_Tm.tm_hour)
        return true;
    if (m_Tm.tm_hour > t.m_Tm.tm_hour)
        return false;
    if (m_Tm.tm_min < t.m_Tm.tm_min)
        return true;
    if (m_Tm.tm_min > t.m_Tm.tm_min)
        return false;
    if (m_Tm.tm_sec < t.m_Tm.tm_sec)
        return true;
    if (m_Tm.tm_sec > t.m_Tm.tm_sec)
        return false;

    return false;
}

bool Time::operator>=(const Time& t) const
{
    if (m_Tm.tm_year >= t.m_Tm.tm_year)
        return true;
    if (m_Tm.tm_year < t.m_Tm.tm_year)
        return false;
    if (m_Tm.tm_mon >= t.m_Tm.tm_mon)
        return true;
    if (m_Tm.tm_mon < t.m_Tm.tm_mon)
        return false;
    if (m_Tm.tm_mday >= t.m_Tm.tm_mday)
        return true;
    if (m_Tm.tm_mday < t.m_Tm.tm_mday)
        return false;
    if (m_Tm.tm_hour >= t.m_Tm.tm_hour)
        return true;
    if (m_Tm.tm_hour < t.m_Tm.tm_hour)
        return false;
    if (m_Tm.tm_min >= t.m_Tm.tm_min)
        return true;
    if (m_Tm.tm_min < t.m_Tm.tm_min)
        return false;
    if (m_Tm.tm_sec >= t.m_Tm.tm_sec)
        return true;
    if (m_Tm.tm_sec < t.m_Tm.tm_sec)
        return false;

    return false;
}

bool Time::operator<=(const Time& t) const
{
    if (m_Tm.tm_year <= t.m_Tm.tm_year)
        return true;
    if (m_Tm.tm_year > t.m_Tm.tm_year)
        return false;
    if (m_Tm.tm_mon <= t.m_Tm.tm_mon)
        return true;
    if (m_Tm.tm_mon > t.m_Tm.tm_mon)
        return false;
    if (m_Tm.tm_mday <= t.m_Tm.tm_mday)
        return true;
    if (m_Tm.tm_mday > t.m_Tm.tm_mday)
        return false;
    if (m_Tm.tm_hour <= t.m_Tm.tm_hour)
        return true;
    if (m_Tm.tm_hour > t.m_Tm.tm_hour)
        return false;
    if (m_Tm.tm_min <= t.m_Tm.tm_min)
        return true;
    if (m_Tm.tm_min > t.m_Tm.tm_min)
        return false;
    if (m_Tm.tm_sec <= t.m_Tm.tm_sec)
        return true;
    if (m_Tm.tm_sec > t.m_Tm.tm_sec)
        return false;

    return false;
}

TimeMS Time::getTime()
{
    struct timeval tval;

    memset(&tval, 0x00, sizeof(tval));

    if (gettimeofday(&tval, NULL)) {
        //_DBG("failed to get time");
        return 1;
    }

    long long mSec1 = tval.tv_sec * 1000LL;
    long long mSec2 = tval.tv_usec / 1000LL;

    TimeMS timestamp = mSec1 + mSec2;
    //_DBG("timestamp = %lld , mSec1 = %lld, mSec2 = %lld", timestamp, mSec1, mSec2);
    return timestamp;
}

//struct timeval  
//{  
//__time_t tv_sec;        /* Seconds. */  
//__suseconds_t tv_usec;  /* Microseconds. */  
//};

struct tm Time::GetTime(TimeMS timestamp)
{
    struct timeval tv;
    struct tm st;

    memset(&tv, 0x00, sizeof(tv));
    memset(&st, 0x00, sizeof(st));

    tv.tv_sec = timestamp / 1000;
    tv.tv_usec = (timestamp % 1000) * 1000;

    localtime_r(&(tv.tv_sec), &st);

    return st;
}

struct tm Time::GetFormattedTime(const char *t)
{
	int temp_month=0;
    struct tm st;
    memset(&st, 0x00, sizeof(st));

    sscanf(t, "%d-%d-%dT%d:%d:%d.%dZ", (int *)&(st.tm_year), (int *)&(temp_month), (int *)&(st.tm_mday), (int *)&(st.tm_hour),
            (int *)&(st.tm_min), (int *)&(st.tm_sec), (int *)&(st.tm_wday));

	st.tm_mon = temp_month+1;
    return st;
}

void Time::UnixtimeToLocaltime(time_t t, int timeZone, char *time, int size)
{
    struct tm *p;
    int iYear = 0;
    int iMonth = 0;
    int iDay = 0;
    int iHour = 0;
    int iMinute = 0;
    char cDay[10] = { 0 };

    p = gmtime(&t);

    if (time) {
        memset(time, 0x0, size);
        strftime(time, size, "%m/%d/%Y %a %H:%M", p);
        sscanf(time, "%02d/%02d/%4d %05s %02d:%02d", (int *) &(iMonth), (int *) &(iDay),
                (int *) &(iYear), (char *) cDay, (int *) &(iHour), (int *) &(iMinute));

        if (iHour <= 12) {
            if (iHour + timeZone <= 12) {
                snprintf((char *) time, size, "%02d/%02d/%04d %.3s %02d:%02d AM", iMonth, iDay,
                        iYear, cDay, iHour + timeZone, iMinute);
            } else {
                snprintf((char *) time, size, "%02d/%02d/%04d %.3s %02d:%02d PM", iMonth, iDay,
                        iYear, cDay, iHour + timeZone - 12, iMinute);
            }
        } else {
            if (iHour + timeZone < 24) {
                snprintf((char *) time, size, "%02d/%02d/%04d %.3s %02d:%02d PM", iMonth, iDay,
                        iYear, cDay, (iHour + timeZone - 12), iMinute);
            } else {
                snprintf((char *) time, size, "%02d/%02d/%04d %.3s %02d:%02d AM", iMonth, iDay,
                        iYear, cDay, (iHour + timeZone - 24), iMinute);
            }
        }
    }
}

string Time::AsString()
{
	//_FUNCBEGIN;
    char time[128] = { 0, };

    snprintf(time, 127, "%d-%d-%dT%d:%d:%d.%dZ", m_Tm.tm_year, m_Tm.tm_mon+1, m_Tm.tm_mday,
            m_Tm.tm_hour, m_Tm.tm_min, m_Tm.tm_sec, m_Tm.tm_wday);

	//_FUNCEND;
    return (string(time));
}

