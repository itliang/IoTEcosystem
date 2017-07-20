#include "async/UpdatePluginTask.h"
#include "galaxyapps/PluginInfoRequest.h"
#include "callback/DownloadPluginCallBack.h"
#include "callback/PluginTaskResponseCallback.h"
void UpdatePluginTask::processing()
{
	mState = RUNNING;
    mTryCount++;
	PluginInfoRequest *pRequestHandler = PluginInfoRequest::getInstance();	
	if(mTryCount > 1){
		pRequestHandler->changeGalaxyAppsServer();
	}
	pRequestHandler->request(mPluginInfo,MSG::START_UPDATE,this);
	
}

void UpdatePluginTask::onSuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	 mState = FINISHED;
     mResponseCallback->onSuccessTask(this, pluginInfo, code);
}

void UpdatePluginTask::onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{

}

void UpdatePluginTask::onFailure(PluginInfo *pluginInfo, ErrorCode error)
{

}

void UpdatePluginTask::request(PluginSchedulerRequestType type){
	// no need to use in this file
}
