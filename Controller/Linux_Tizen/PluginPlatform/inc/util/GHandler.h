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

#ifndef __G_HANDLER_H__
#define __G_HANDLER_H__

#include <glib.h>
#include "Handler.h"
#include "Mutex.h"
#include <vector>

class GHandler: public Handler
{
private:
#ifdef USE_THREAD_POOL
	class GHandlerThread : public ThreadWorker
#else
	class GHandlerThread: public Thread
#endif
	{
	private:
		GHandler *mParent;
	public:
		GHandlerThread(GHandler *parent);
		void run();
	};

	class GHandlerItem
	{
	public:
		GHandlerItem();
		gint64 time;
		bool mAutoFree;
		int mGroupId;
		ThreadWorker *post;
		HandleMessage *message;
	};

	GMainLoop *mLoop;
	GMainContext *mContext;
	GHandler::GHandlerThread *mThread;
	std::vector<GHandlerItem> mQueue;
	IoTCore::Base::Mutex mSync;

	bool mDestroy;
	bool mHandlerRunning;
	GSource *mInternalHandler;
	static bool __sort_queue(GHandlerItem a, GHandlerItem b);

public:
	GHandler(bool main);
	GHandler(bool main, HandlerCallback *callback);
	~GHandler();

	bool handleMessage(HandleMessage *msg);

//       bool post(ThreadWorker *worker);
	bool post(ThreadWorker *worker, bool bAutoFree = false, int nGroupId = -1);
	bool postHead(ThreadWorker *worker);
	bool postTimeout(ThreadWorker *worker, TimeMS msec);
	bool postTimeoutHead(ThreadWorker *worker, TimeMS msec);
	void removeCallback(ThreadWorker *worker);
	void removeCallback(int nGroupId);
	void removeCallbackAll();

	bool sendMessage(HandleMessage *msg);
	bool sendMessageHead(HandleMessage *msg);
	bool sendMessageTimeout(HandleMessage *msg, TimeMS msec);
	bool sendMessageTimeoutHead(HandleMessage *msg, TimeMS msec);
	void removeMessage(int what);
	void removeMessageAll();
	HandleMessage* obtainMessage();
	HandleMessage* obtainMessage(int what);
	HandleMessage* obtainMessage(int what, void* object);
	HandleMessage* obtainMessage(int what, int param1, int param2,
			void* object);

	void selfDestroy();
private:
	void __start_handler();
	static gboolean __process_gloop(gpointer data);
	static gboolean __delay_gloop(gpointer data);

};

#endif
