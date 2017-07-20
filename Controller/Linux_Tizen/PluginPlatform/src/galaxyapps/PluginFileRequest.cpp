/*
 * PluginDownloadHandler.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: zoulianghui
 */

#include "PluginFileRequest.h"
#include "Log.h"
//#include "util/Downloader.h"
#include <memory.h>
#include <sstream>
#include <app_common.h>

static PluginFileRequest* _pInstance = NULL;

PluginFileRequest::PluginFileRequest() {
	// TODO Auto-generated constructor stub
	mHandler = new GHandler(false,this);
	mRunning = false;
	
	
	Start();
}

PluginFileRequest::~PluginFileRequest() {
	// TODO Auto-generated destructor stub
	delete mHandler;
}

void PluginFileRequest::request(PluginInfo *filter,PluginTaskCallback *downloadPluginCallback ){
	HandleMessage* mMsg = obtainMessage();
	mMsg->what = MSG::START_DOWNLOAD;

	mCallback = downloadPluginCallback;
	mPluginInfo = filter;
	mHandler->sendMessage(mMsg);
}

void PluginFileRequest::run(){

}

bool PluginFileRequest::handleMessage(HandleMessage *msg){
	//dlog_print(DLOG_ERROR, "PluginPlatform", "handle message");
	WDEBUG("handle message");
	switch(msg->what){
	case MSG::START_DOWNLOAD:
		checkNDownloadFromQ();
		break;
	case MSG::ERROR_DOWNLOAD:
	case MSG::END_DOWNLOAD:
		//remove from callback map
		callBackMap.erase(1);//erase current plugin info
		checkNDownloadFromQ();
		break;
	}

	return true;
}

PluginFileRequest* PluginFileRequest::getInstance(){
	if(!_pInstance)
		_pInstance =  new PluginFileRequest();
	return _pInstance;
}

HandleMessage* PluginFileRequest::obtainMessage(){
	return mHandler->obtainMessage();
}

bool PluginFileRequest::Start()
{
	return mHandler->post(this);
}

void PluginFileRequest::Stop()
{
	mHandler->removeCallback(this);
}

void PluginFileRequest::checkNDownloadFromQ(){

	#if 0
	if(downloadQueueList.empty()){
		setRunning(false);
		callBackMap.clear();
		return;
	}
	#endif
	
	executeDownload();
}

void PluginFileRequest::executeDownload(){
	if(!isRunning()){
		setRunning(true);

		mFileDownloader.SetDownloadFileUrl(mPluginInfo->getUrl());

		std::string filename=mPluginInfo->getName();
		mFileDownloader.SetDownloadFileName(filename+".apk");

		std::string mDownloadPath = app_get_data_path();
		mFileDownloader.SetDownloadPath(mDownloadPath);

		mFileDownloader.SetDownloadStateChangeListener(dynamic_cast<IFileDownloadListener*>(this));
		FileDownloadError error = mFileDownloader.StartDownload();

	}
}

bool PluginFileRequest::isRunning(){
	return mRunning;
}

void PluginFileRequest::setRunning(bool val){
	mRunning = val;
}

void PluginFileRequest::OnFileDownloadStateChanged(std::string requestId,FILE_DOWNLOAD_STATE state){
   FUNC_BEGIN();
   if(state == FILE_DOWNLOAD_STATE_FAILED){
	   WDEBUG("download failed");
	   mCallback->onFailure(mPluginInfo,ErrorCode::OPERATION_FAILED);
   }else if(state == FILE_DOWNLOAD_STATE_COMPLETED){
	   WDEBUG("download finished");
	   mCallback->onSuccess(mPluginInfo,SuccessCode::PLUGIN_DOWNLOADED);
	  
   }else {
	   WDEBUG("other error %d",state);
   }
}

void PluginFileRequest::notifySuccess(queue<int> callbacks,SuccessCode code,PluginInfo *pluginInfo)
{

	
}









