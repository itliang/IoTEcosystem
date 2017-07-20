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

#ifndef TIMERUTIL_H_
#define TIMERUTIL_H_
#include <glib.h>

class TimerEventListener{
public:
	TimerEventListener(){};
	virtual ~TimerEventListener(){};

	virtual bool OnTimerTick() = 0;
};

class Timer{
public:
	Timer();
	virtual ~Timer();

	void SetEventListener(TimerEventListener* listener){
		mEventListener = listener;
	}

	void UnSetEventListener(){
		mEventListener = NULL;
	}

	void Start();
	void Stop();
	void Restart();
	void Pause();
	void Resume();
	bool IsStarted();

	void SetInterval(float interval){
		mTimerInterval = interval;
	}

	float GetCurrentInterval(){
		return mTimerInterval;
	}

private:
	static gboolean timerCB(gpointer user_data);

private:
	TimerEventListener* mEventListener;
	guint mTimer;
	guint mTimerInterval;
};



#endif /* TIMERUTIL_H_ */
