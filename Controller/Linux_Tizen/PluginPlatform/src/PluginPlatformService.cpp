#include "PluginPlatformService.h"
//#include "async/FindPluginInfoTask.h"
//#include "async/DownloadPluginTask.h"
//#include "galaxyapps/PluginFileRequest.h"
#include "Log.h"

PluginPlatformService::~PluginPlatformService()
{
	delete mPluginTaskManager;
}


PluginPlatformService::PluginPlatformService()
{
	mPluginTaskManager = new PluginTaskManager();
	//WDEBUG("execute update scheduler");
	//mPluginUpdateScheduler = PluginUpdateScheduler::getInstance();
	//mPluginUpdateScheduler->execute(false,this);

}

void PluginPlatformService::findPluginInfo(PluginInfo *info)
{
	mPluginTaskManager->command(info,FIND_COMMAND,this);
}

void PluginPlatformService::downLoadPlugin(PluginInfo *info)
{
	mPluginTaskManager->command(info,DOWNLOAD_COMMAND,this);
}

void PluginPlatformService::installPlugin(PluginInfo *info)
{
	mPluginTaskManager->command(info,INSTALL_COMMAND,this);
}

void PluginPlatformService::deletePlugin(PluginInfo *info)
{

}


void PluginPlatformService::onSuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	WDEBUG("PluginPlatformService::onSuccess");

	//IPC ...

	//TEST CODE
	//downLoadPlugin(pluginInfo);
}

void PluginPlatformService::onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{
	WDEBUG("PluginPlatformService::onProgress");
}

void PluginPlatformService::onFailure(PluginInfo *pluginInfo, ErrorCode error)
{
	WDEBUG("PluginPlatformService::onFailure");
}

/*
void PluginBgService::request(PluginSchedulerRequestType type){
	WDEBUG("request type is %d",type);
	if(type == REQUEST_TYPE_PLUGIN_UPDATE){
		//TBD mPluginTaskManager->clearNotUsedPlugins();
		mPluginTaskManager->updateAllPlugins(this);
	}else if(type == REQUEST_TYPE_SERVER_UPDATE){
		//TBD PluginServiceInfo.updateCNServerURL
	}
}
*/

