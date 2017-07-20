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

#include "util/Log.h"
#include "util/Mutex.h"

Mutex::Mutex()
{
	pthread_mutex_init(&mutex, NULL);
}

IoTCore::Base::Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex);
}

void IoTCore::Base::Mutex::lock()
{
	int ret = 0;

	ret = pthread_mutex_lock(&mutex);
	if (ret != 0)
	{
		//_ERR("Mutex error occur [%d],[%s]", ret, strerror(ret));
	}
}

void IoTCore::Base::Mutex::trylock()
{
	int ret = 0;
	ret = pthread_mutex_trylock(&mutex);
	if (ret != 0)
	{

	}
}

void IoTCore::Base::Mutex::unlock()
{
	int ret = 0;

	ret = pthread_mutex_unlock(&mutex);
	if (ret != 0)
	{
		//_ERR("Mutex error occur [%d],[%s]", ret, strerror(ret));
	}
}
