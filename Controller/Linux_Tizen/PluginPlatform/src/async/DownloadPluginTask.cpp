#include "async/DownloadPluginTask.h"
#include "galaxyapps/PluginInfoRequest.h"
#include "util/Log.h"
#include "callback/PluginTaskResponseCallback.h"

void DownloadPluginTask::processing() 
{
	mState = RUNNING;
    mTryCount++;
	PluginInfoRequest *pRequestHandler = PluginInfoRequest::getInstance();	
	if(mTryCount > 1){
		pRequestHandler->changeGalaxyAppsServer();
	}
	pRequestHandler->request(mPluginInfo,MSG::START_DOWNLOAD_URI,this);
}

void DownloadPluginTask::onSuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	WDEBUG("DownloadPluginTask::onSuccess,id:%s|uri:%s",pluginInfo->getId().c_str(),pluginInfo->getUrl().c_str());
	mState = FINISHED;

//check NULL .....
	if(pluginInfo->getSignature()== ""){
		WDEBUG("DownloadPluginTask::onSuccess,Signature is empty!");
		mResponseCallback->onFailureTask(this,pluginInfo,NOT_VALID_SIGNATURE);
		return;

	}else{
#if 0		
		PluginServiceInfo *serviceInfo = PluginServiceInfo.getInstance();
		string path = serviceInfo->getDataRootPath() + pluginInfo->getVMFPath();
		string apkPath;

#endif
	}

	
	mResponseCallback->onSuccessTask(this,pluginInfo,code);
}	

void DownloadPluginTask::onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{
	WDEBUG("DownloadPluginTask::onProgress,id:%s|uri:%s",pluginInfo->getId().c_str(),pluginInfo->getUrl().c_str());
	PluginFileRequest *pFileRequest = PluginFileRequest::getInstance();
	pFileRequest->request(pluginInfo,this);
}

void DownloadPluginTask::onFailure(PluginInfo *pluginInfo, ErrorCode error)
{
	WDEBUG("DownloadPluginTask::onFailure,id:%s|uri:%s",pluginInfo->getId().c_str(),pluginInfo->getUrl().c_str());
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


void DownloadPluginTask::request(PluginSchedulerRequestType type){
	// no need to use in this file
}
