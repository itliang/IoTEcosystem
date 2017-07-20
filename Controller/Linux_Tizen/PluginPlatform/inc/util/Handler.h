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

#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <util/Thread.h>
#include <util/Time.h>

class Handler;
class HandleMessage
{
public:
    std::string sender;
	void *object; // object to work.
	int param1; // param1
	int param2; // param2
	int what; // user defined message code
	Handler *reply; // send reply message
};

class HandlerCallback
{
public:
	virtual bool handleMessage(HandleMessage *msg) = 0;
};

class Handler: virtual public HandlerCallback
{
public:
	virtual ~Handler();

protected:
	HandlerCallback* mCallback;
	bool mMain;

	Handler(bool main); // main is true, UI thread if possible
	Handler(bool main, HandlerCallback *callback); // main is true, UI thread if possible
	virtual bool handleMessage(HandleMessage *msg) = 0;
public:
	bool getIsMain();

	//virtual bool post(ThreadWorker *worker) = 0;
	virtual bool post(ThreadWorker *worker, bool bAutoFree, int nGroupId) = 0;
	virtual bool postHead(ThreadWorker *worker) = 0;
	virtual bool postTimeout(ThreadWorker *worker, TimeMS msec) = 0;
	virtual bool postTimeoutHead(ThreadWorker *worker, TimeMS msec) = 0;
	virtual void removeCallback(ThreadWorker *worker) = 0;
	virtual void removeCallback(int nGroupId) = 0;
	virtual void removeCallbackAll() = 0;

	virtual bool sendMessage(HandleMessage *msg) = 0;
	virtual bool sendMessageHead(HandleMessage *msg) = 0;
	virtual bool sendMessageTimeout(HandleMessage *msg, TimeMS msec) = 0;
	virtual bool sendMessageTimeoutHead(HandleMessage *msg, TimeMS msec) = 0;
	virtual void removeMessage(int what) = 0;
	virtual void removeMessageAll() = 0;

	virtual HandleMessage* obtainMessage() = 0;
	virtual HandleMessage* obtainMessage(int what) = 0;
	virtual HandleMessage* obtainMessage(int what, void* object) = 0;
	virtual HandleMessage* obtainMessage(int what, int param1, int param2,
			void* object) =0;
};

#endif
