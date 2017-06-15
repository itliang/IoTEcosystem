#include <Ecore.h>
#include <iostream>
#include "SuccessCode.hpp"
#include "ErrorCode.hpp"
#include "PluginInfo.hpp"


class DownloadPluginTask{

private:
	Ecore_Thread *pThread;
	PluginInfo *pPluginInfo;

public:
	DownloadPluginTask(PluginInfo *info);
	void startDownload();


	static void downloadThreadCB(void *data, Ecore_Thread *thread);
	static void downloadCancelThreadCB(void *data, Ecore_Thread *thread);
	static void downloadEndThreadCB(void *data, Ecore_Thread *thread);
 	static void downloadThreadNotifyCB(void *data, Ecore_Thread *thread, void *msgData);

private:
	void downloadPlugin();
};
