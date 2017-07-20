#include "service/PluginBgService.hpp"
#include "async/FindPluginInfoTask.h"
#include "async/DownloadPluginTask.h"
#include "galaxyapps/PluginFileRequest.h"
#include "util/Log.h"

PluginBgService::~PluginBgService()
{
	delete mPluginTaskManager;
}


PluginBgService::PluginBgService()
{
	mPluginTaskManager = new PluginTaskManager();
	//WDEBUG("execute update scheduler");
	//mPluginUpdateScheduler = PluginUpdateScheduler::getInstance();
	//mPluginUpdateScheduler->execute(false,this);

}

void PluginBgService::findPluginInfo(PluginInfo *info)
{
	mPluginTaskManager->command(info,FIND_COMMAND,this);
}

void PluginBgService::downLoadPlugin(PluginInfo *info)
{
	mPluginTaskManager->command(info,DOWNLOAD_COMMAND,this);
}

void PluginBgService::installPlugin(PluginInfo *info)
{
	mPluginTaskManager->command(info,INSTALL_COMMAND,this);
}

void PluginBgService::deletePlugin(PluginInfo *info)
{

}

void PluginBgService::onSuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	WDEBUG("PluginBgService::onSuccess");

	//IPC ...

	//TEST CODE
	//downLoadPlugin(pluginInfo);
}

void PluginBgService::onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{
	WDEBUG("PluginBgService::onProgress");
}

void PluginBgService::onFailure(PluginInfo *pluginInfo, ErrorCode error)
{
	WDEBUG("PluginBgService::onFailure");
}

void PluginBgService::request(PluginSchedulerRequestType type){
	WDEBUG("request type is %d",type);
	if(type == REQUEST_TYPE_PLUGIN_UPDATE){
		//TBD mPluginTaskManager->clearNotUsedPlugins();
		mPluginTaskManager->updateAllPlugins(this);
	}else if(type == REQUEST_TYPE_SERVER_UPDATE){
		//TBD PluginServiceInfo.updateCNServerURL
	}
}

