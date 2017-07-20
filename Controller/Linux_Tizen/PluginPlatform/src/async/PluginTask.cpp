#include "PluginTask.h"

PluginTask::PluginTask(PluginInfo *info, PluginTaskCallback *pCB, PluginTaskResponseCallback *response)
{
	//MAX_TASK_RETRY_COUNT = 30;
	mTryCount = 0;
	mPluginInfo = info;
	mResponseCallback = response;

	if(mCallbackList == NULL){
		mCallbackList = new list<PluginTaskCallback *>();
	}
	mCallbackList->push_back(pCB);	
}

PluginTask::~PluginTask(){
	
	g_thread_join(pThread);

	for (list<PluginTaskCallback*>::iterator it = mCallbackList->begin(); it != mCallbackList->end();)  
    {  
        mCallbackList->erase(it++);  
    }
	delete mCallbackList;

}

void PluginTask::startTask()
{
	GError *error = null;
	pThread = g_thread_create(gThreadFunc, this, TRUE, &error);
	
	
	//pThread = ecore_thread_feedback_run(DoInBackgroundThreadCB, ThreadNotifyCB, EndThreadCB, CancelThreadCB, this, EINA_FALSE);
}

gpointer PluginTask::gThreadFunc(gpointer data){
	PluginTask *pThis = (PluginTask *)data;
	pThis->processing();
	return FALSE;
}


//void PluginTask::DoInBackgroundThreadCB(void *data, Ecore_Thread *thread)
//{
//	PluginTask *pThis = (PluginTask *)data;
//	pThis->processing();
//}

bool PluginTask::isContains(PluginInfo *plugininfo)
{
	if(mPluginInfo == NULL)
		return false;

	if(plugininfo->getKey() == mPluginInfo->getKey() && mState == TaskStateCode::FINISHED){
		return true;
	}

	return false;
}


void PluginTask::notifySuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	for(auto p :*mCallbackList)
	{
		PluginTaskCallback *callback = (PluginTaskCallback *)p;
		callback->onSuccess(pluginInfo,code);
	}
}

void PluginTask::notifyProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{
	for(auto p :*mCallbackList)
	{
		PluginTaskCallback *callback = (PluginTaskCallback *)p;
		callback->onProgress(pluginInfo,stateCode,current_status);
	}
}

void PluginTask::notifyFailure(PluginInfo *pluginInfo, ErrorCode error)
{
	for(auto p :*mCallbackList)
	{
		PluginTaskCallback *callback = (PluginTaskCallback *)p;
		callback->onFailure(pluginInfo,error);
	}
}

/*
void PluginTask::CancelThreadCB(void *data, Ecore_Thread *thread)
{
	WDEBUG("PluginTask::CancelThreadCB");
}

void PluginTask::EndThreadCB(void *data, Ecore_Thread *thread)
{
	WDEBUG("PluginTask::EndThreadCB");
}
	
void PluginTask::ThreadNotifyCB(void *data, Ecore_Thread *thread, void *msgData)
{
	WDEBUG("PluginTask::ThreadNotifyCB");
}
*/
