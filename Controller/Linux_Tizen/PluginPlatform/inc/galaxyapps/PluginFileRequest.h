/*
 * PluginDownloadHandler.h
 *
 *  Created on: Jun 13, 2017
 *      Author: zoulianghui
 */
#include "util/GHandler.h"
#include "util/Handler.h"
#include "util/Mutex.h"
#include <stdio.h>
#include "util/Thread.h"
#include <string>
#include <queue>
#include <efl_extension.h>
#include <curl/curl.h>
#include "util/MRFileDownloader.h"
#include "GalaxyAppsRequest.h"
#include "info/PluginInfo.hpp"
#include "constants/SuccessCode.hpp"
#include "callback/PluginTaskCallback.h"

using namespace std;

#ifndef GALAXYAPPS_HANDLER_PLUGINDOWNLOADHANDLER_H_
#define GALAXYAPPS_HANDLER_PLUGINDOWNLOADHANDLER_H_

#define BUFFER_SIZE 1024

class PluginFileRequest :public HandlerCallback,public ThreadWorker,public IFileDownloadListener{

private:
	GHandler* mHandler;
	bool mRunning;
	queue<int> downloadQueueList;
	map<int,queue<int> > callBackMap;
	PluginInfo * mPluginInfo;

	PluginTaskCallback *mCallback = NULL;

public:
	PluginFileRequest();
	virtual ~PluginFileRequest();
	void request(PluginInfo *filter,PluginTaskCallback *downloadPluginCallback );
	bool handleMessage(HandleMessage *msg);
	void run(void);
	static PluginFileRequest* getInstance();
	HandleMessage* obtainMessage(void);
	bool Start(void);
	void Stop(void);
	void setRunning(bool val);
	bool isRunning();

private:
	void checkNDownloadFromQ();
	void executeDownload();
	//IFileDownloadListener
	virtual void OnFileDownloadStateChanged(std::string requestId,FILE_DOWNLOAD_STATE state);
	void notifySuccess(queue<int> callbacks,SuccessCode code,PluginInfo *pluginInfo);

protected:
	MRFileDownloader mFileDownloader;
	

	//Ecore_Thread *mDownloadThread;
};

#endif /* GALAXYAPPS_HANDLER_PLUGINDOWNLOADHANDLER_H_ */
