#ifndef _BG_SERVICE_H_
#define _BG_SERVICE_H_

#include <string>
#include "info/PluginInfo.hpp"
#include "callback/PluginTaskCallback.h"
#include "service/PluginTaskManager.h"
#include "service/PluginUpdateScheduler.h"

class PluginBgService : public PluginTaskCallback 
{
public:
	
	PluginBgService();
	virtual ~PluginBgService();
	void findPluginInfo(PluginInfo *info);
	void downLoadPlugin(PluginInfo *info);
	void installPlugin(PluginInfo *info);
	void deletePlugin(PluginInfo *info);
private:
	
	void onSuccess(PluginInfo *pluginInfo, SuccessCode code);
    void onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status);
    void onFailure(PluginInfo *pluginInfo, ErrorCode error);
	PluginTaskManager *mPluginTaskManager;
	PluginUpdateScheduler *mPluginUpdateScheduler;
	void request(PluginSchedulerRequestType type);

};
#endif 

