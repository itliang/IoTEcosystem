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

#ifndef __TIME_H__
#define __TIME_H__

#include "DataType.h"
#include <string>


/// Number of nanoseconds in one second.
///
/// Interval for it is from 0 to 999,999,999.
const long kNanoSecondsPerSecond = 1000000000;

/// Number of microseconds in one second.
///
/// Interval for it is from 0 to 999,999.
const long kMicroSecondsPerSecond = 1000000;

/// Number milliseconds in one second.
///
/// Interval for it is from 0 to 999.
const long kMilliSecondsPerSecond = 1000;

class Time
{
public:
	Time(void);
	Time(const struct tm& t);
	~Time();

	void SetTime(const struct tm& t);
	void SetTime(const time_t t);

	static TimeMS getTime(void);
	static struct tm GetTime(TimeMS timestamp);
	static struct tm GetFormattedTime(const char *t);
	static void UnixtimeToLocaltime(time_t t, int timeZone, char *time, int size);
	std::string AsString();

	/// Operator to test equality of time.
	bool operator==(const Time& t) const;

	/// Operator to test equality of time.
	bool operator!=(const Time& t) const;

	/// Operator to test if time is later.
	bool operator>(const Time& t) const;

	/// Operator to test if time is earlier.
	bool operator<(const Time& t) const;

	/// Operator to test if time is later.
	bool operator>=(const Time& t) const;

	/// Operator to test if time is earlier.
	bool operator<=(const Time& t) const;

private:
	struct tm m_Tm;
	//TimeMS m_Tms;
};

#endif
