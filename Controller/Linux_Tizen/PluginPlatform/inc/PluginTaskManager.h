#ifndef __PLUGIN_TASK_MANAGER_H__
#define __PLUGIN_TASK_MANAGER_H__
#include <list>
#include "PluginDBManager.h"
#include "PluginInfo.hpp"
#include "TaskCommand.h"
#include "PluginTask.h"
#include "FindPluginInfoTask.h"
#include "DownloadPluginTask.h"
#include "InstallPluginTask.h"
#include "UpdatePluginTask.h"
#include "PluginTaskResponseCallback.h"


using namespace std;
class PluginTaskManager : public PluginTaskResponseCallback{

public:
	PluginTaskManager();
	~PluginTaskManager();
	static PluginTaskManager* getInstance();

	void init();

	void command(PluginInfo *pluginInfo,  TaskCommand command, PluginTaskCallback *callback) ;
	void onSuccessTask(PluginTask *task, PluginInfo *pluginInfo, SuccessCode code);
    void onFailureTask(PluginTask *task, PluginInfo *pluginInfo, ErrorCode error);
	void removePlugin(PluginInfo *pluginInfo);
	void updateAllPlugins(PluginTaskCallback *callback);
private:

	

	list<PluginTask *> *mPluginTaskEntry = NULL;
	PluginDBManager *mPluginDBManager = NULL;
	
};
#endif

