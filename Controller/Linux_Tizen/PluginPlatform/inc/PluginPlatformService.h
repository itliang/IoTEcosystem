#ifndef _PLUGIN_PLATFORM_SERVICE_H_
#define _PLUGIN_PLATFORM_SERVICE_H_

#include <string>
#include "PluginInfo.hpp"
#include "PluginTaskCallback.h"
#include "PluginTaskManager.h"
#include "PluginUpdateScheduler.h"

class PluginPlatformService : public PluginTaskCallback 
{
public:
	
	PluginPlatformService();
	virtual ~PluginPlatformService();

	
	void findPluginInfo(PluginInfo *info);
	void downLoadPlugin(PluginInfo *info);
	void installPlugin(PluginInfo *info);
	void deletePlugin(PluginInfo *info);
	
private:
	
	void onSuccess(PluginInfo *pluginInfo, SuccessCode code);
	void onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status);
    void onFailure(PluginInfo *pluginInfo, ErrorCode error);
	
	PluginTaskManager *mPluginTaskManager;
//	PluginUpdateScheduler *mPluginUpdateScheduler;
//	void request(PluginSchedulerRequestType type);

};
#endif 

