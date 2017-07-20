
#ifndef _PLUG_TASK_H_
#define _PLUG_TASK_H_
#include <list>
//#include <Ecore.h>
#include <glib.h>
#include <iostream>
#include "SuccessCode.hpp"
#include "ErrorCode.hpp"
#include "TaskStateCode.h"
#include "StateCode.hpp"

#include "PluginInfo.hpp"
#include "PluginTaskCallback.h"
#include "Log.h"

class PluginTaskResponseCallback;
using namespace std;

class PluginTask : public PluginTaskCallback
{
	
public :
	
		PluginTask(PluginInfo *info, PluginTaskCallback *pCB, PluginTaskResponseCallback *response);
		~PluginTask(){}
		void startTask();
		void addCallback(PluginTaskCallback *callback){mCallbackList->push_back(callback);}
		void clearCallbacks(){}
		bool isContains(PluginInfo *plugininfo);
		
		void notifySuccess(PluginInfo *pluginInfo, SuccessCode code) ;
     	void notifyProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status);
     	void notifyFailure(PluginInfo *pluginInfo, ErrorCode error);
		virtual void processing() = 0;
		
protected :
		static const int MAX_TASK_RETRY_COUNT = 30;
		TaskStateCode mState;
		int mTryCount ;
		GThread *pThread = NULL;
		PluginInfo *mPluginInfo = NULL;
		list<PluginTaskCallback*> *mCallbackList = NULL;
		PluginTaskResponseCallback *mResponseCallback = NULL;

private :	
		gpointer gThreadFunc(gpointer data);
		//static void DoInBackgroundThreadCB(void *data, Ecore_Thread *thread);
		//static void CancelThreadCB(void *data, Ecore_Thread *thread);
		//static void EndThreadCB(void *data, Ecore_Thread *thread);
	 	//static void ThreadNotifyCB(void *data, Ecore_Thread *thread, void *msgData);
};

#endif
