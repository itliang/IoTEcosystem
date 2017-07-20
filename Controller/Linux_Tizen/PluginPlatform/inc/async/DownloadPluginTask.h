
#ifndef _DOWNLOAD_PLUG_TASK_H_
#define _DOWNLOAD_PLUG_TASK_H_


#include "async/PluginTask.h"

class DownloadPluginTask : public PluginTask{

public:
	DownloadPluginTask(PluginInfo *info, PluginTaskCallback *pCB,PluginTaskResponseCallback *response):PluginTask(info,pCB,response){}

private:
	void processing() ;
	void onSuccess(PluginInfo *pluginInfo, SuccessCode code);
   	void onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status);
    void onFailure(PluginInfo *pluginInfo, ErrorCode error);
    void request(PluginSchedulerRequestType type);
};

#endif
