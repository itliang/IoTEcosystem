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

#include "Timer.h"
#include "Log.h"

Timer::Timer()
{
	mTimer    = 0;
	mEventListener = NULL;
	mTimerInterval = -1;
}

Timer::~Timer()
{
	Stop();
}

void Timer::Start()
{
	if(mTimerInterval <= 0){
		THROW_RUNTIME_EXCEPTION("timer interval is 0");
	}

	if(mTimer==0){
		mTimer = g_timeout_add(mTimerInterval, timerCB, this);
	}else{
		Restart();
	}
}

gboolean Timer::timerCB(gpointer user_data)
{
	FUNC_BEGIN();
	Timer* pThis = static_cast<Timer*>(user_data);
	if(pThis->mEventListener != NULL){
		if(pThis->mEventListener->OnTimerTick() == true){
			return TRUE;
		}else{
			pThis->mEcoreTimer = NULL;
			return FALSE;
		}
	}else{
		pThis->mTimer = 0;
		return FALSE;
	}
}

void Timer::Stop()
{
	FUNC_BEGIN();
	if(mTimer > 0){
		g_source_remove(mTimer);
		mTimer = 0;
	}
	FUNC_END();
}

void Timer::Restart()
{
	FUNC_BEGIN();
	
	FUNC_END();
}

void Timer::Pause()
{
	FUNC_BEGIN();
	
	FUNC_END();
}

void Timer::Resume()
{
	FUNC_BEGIN();
	
	FUNC_END();
}

bool Timer::IsStarted()
{
	return mTimer > 0 ? true : false;
}

