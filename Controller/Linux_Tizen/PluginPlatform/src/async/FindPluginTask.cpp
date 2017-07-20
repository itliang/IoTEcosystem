#include "FindPluginInfoTask.h"
#include "PluginInfoRequest.h"
#include "DownloadPluginCallBack.h"
#include "PluginTaskResponseCallback.h"
#include "Log.h"

void FindPluginInfoTask::processing()
{
	mState = RUNNING;
    mTryCount++;
	PluginInfoRequest *pRequestHandler = PluginInfoRequest::getInstance();	
	if(mTryCount > 1){
		pRequestHandler->changeGalaxyAppsServer();
	}
	pRequestHandler->request(mPluginInfo,MSG::START_FIND,this);
	
}

void FindPluginInfoTask::onSuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	WDEBUG("FindPluginInfoTask::onSuccess");
	mState = FINISHED;
    mResponseCallback->onSuccessTask(this, pluginInfo, code);
}

void FindPluginInfoTask::onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{
	WDEBUG("FindPluginInfoTask::onProgress");
}

void FindPluginInfoTask::onFailure(PluginInfo *pluginInfo, ErrorCode error)
{
	WDEBUG("FindPluginInfoTask::onFailure");
	if(error == OPERATION_ERROR){
		if(mTryCount <= MAX_TASK_RETRY_COUNT){
			processing();
		}else{
			mState = FINISHED;
			mResponseCallback->onFailureTask(this,pluginInfo,error);
		}
	}else{
		mState = FINISHED;
		mResponseCallback->onFailureTask(this,pluginInfo,error);
	}
}


void FindPluginInfoTask::request(PluginSchedulerRequestType type){
	// no need to use in this file
}

