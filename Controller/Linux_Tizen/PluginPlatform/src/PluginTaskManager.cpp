#include "Log.h"
#include "PluginTaskManager.h"
#include "PluginInfoRequest.h"
#include "PluginDataStatusCode.h"

static PluginTaskManager* _pInstance = NULL;
PluginTaskManager::PluginTaskManager()
{
	mPluginTaskEntry = new list<PluginTask *>();
}

PluginTaskManager::~PluginTaskManager()
{
	delete mPluginTaskEntry;
}

PluginTaskManager* PluginTaskManager::getInstance()
{
	if(!_pInstance)
		_pInstance =  new PluginTaskManager();
	return _pInstance;
}

void PluginTaskManager::init()
{
	mPluginDBManager = PluginDBManager::getInstance();
}

void PluginTaskManager::onSuccessTask(PluginTask *task, PluginInfo *pluginInfo, SuccessCode code)
{
	WDEBUG("PluginTaskManager::onSuccessTask, plugin id :%s",pluginInfo->getId().c_str());

	//update database
	SuccessCode resultCode = code;
	PluginInfo *foundPluginfo = mPluginDBManager->getPlugin(pluginInfo);

	if(dynamic_cast<FindPluginInfoTask *>(task)){

		if(foundPluginfo == NULL){
			WDEBUG("PluginTaskManager::FindPluginInfoTask, Add new pluginInfo");
			resultCode = PLUGIN_METADATA_FOUND;
			
		}else{
			WDEBUG("PluginTaskManager::FindPluginInfoTask, Update TimeStamp PluginInfo");
			foundPluginfo->setLastCheckUpdate(0.0);
			pluginInfo->setPluginStatus(foundPluginfo->getPluginStatus());
			pluginInfo->setPluginDataStatus(foundPluginfo->getPluginDataStatus());
			mPluginDBManager->updatePlugin(foundPluginfo);

			if (STATUS_DATA_DOWNLOADED == foundPluginfo->getPluginDataStatus()) {
                resultCode = PLUGIN_ALREADY_DOWNLOADED;
            } else if (STATUS_DATA_INSTALLED == foundPluginfo->getPluginDataStatus()) {
                resultCode = PLUGIN_ALREADY_INSTALLED;
            } else {
                WDEBUG("onSuccessTask,FindPluginTask, invalid Plugin Status");
            }
		}
	}else if(dynamic_cast<DownloadPluginTask *>(task)){
		

		if(foundPluginfo == NULL){
			WDEBUG("PluginTaskManager::DownloadPluginTask,Download New PluginInfo");
			pluginInfo->setPluginStatus(STATUS_VALID);
			pluginInfo->setPluginDataStatus(STATUS_DATA_DOWNLOADED);
			mPluginDBManager->createPlugin(pluginInfo);
			resultCode = PLUGIN_DOWNLOADED;
		}else{
			pluginInfo->setPluginStatus(pluginInfo->getPluginStatus());
            pluginInfo->setPluginDataStatus(pluginInfo->getPluginDataStatus());
            if (STATUS_DATA_DOWNLOADED == pluginInfo->getPluginDataStatus()){
                resultCode = PLUGIN_ALREADY_DOWNLOADED;
            } else if (STATUS_DATA_INSTALLED == pluginInfo->getPluginDataStatus()){
                resultCode = PLUGIN_ALREADY_INSTALLED;
            }
		}
	}


	//remove callback.
	mPluginTaskEntry->remove(task);
	task->notifySuccess(pluginInfo,code);
}

void PluginTaskManager::onFailureTask(PluginTask *task, PluginInfo *pluginInfo, ErrorCode error)
{
	WDEBUG("PluginTaskManager::onFailureTask");
	if(dynamic_cast<DownloadPluginTask *>(task)){
		removePlugin(pluginInfo);
	}else if(dynamic_cast<InstallPluginTask *>(task)){
		removePlugin(pluginInfo);
	}else if(dynamic_cast<UpdatePluginTask *>(task)){
		if(error == APPLICATION_NOT_AVAILABLE){
			removePlugin(pluginInfo);
		}
	}

	//remove callback.
	mPluginTaskEntry->remove(task);
	task->notifyFailure(pluginInfo,error);
}

void PluginTaskManager::removePlugin(PluginInfo *pluginInfo) 
{
	if(pluginInfo == NULL){
		WDEBUG("PluginTaskManager:removePlugin is NULL , Check Your code  !!!!!!!!!!!!!!!!!!!!!!!");
		return;
	}
	
   //TBD , remove file .......
   
   // Utils.deleteDirectory(pluginDexPath);
   // Utils.deleteDirectory(pluginLibPath);
    mPluginDBManager->removePlugin(pluginInfo);
}

void PluginTaskManager::command(PluginInfo *pluginInfo,  TaskCommand command, PluginTaskCallback *callback)
{
	WDEBUG("command:taskCommand is :%d",command);

	if(command == FIND_COMMAND){
		
		bool isExist = false ;
			
		for(auto p : *mPluginTaskEntry){
			
			PluginTask *task = (PluginTask *)p;
			if(task->isContains(pluginInfo)){
				WDEBUG("command:TaskCommand.FIND_COMMAND,added Request:%s" ,pluginInfo->getKey().c_str());
				task->addCallback(callback);
				isExist = true;
				break;
			}	
	
		}
	
		if(!isExist){
		
			PluginInfo *foundPlugin  = mPluginDBManager->getPlugin(pluginInfo);

			if(foundPlugin == NULL){
				WDEBUG("command:TaskCommand.FIND_COMMAND,new Task:%s" ,pluginInfo->getKey().c_str());

				PluginTask* task = new FindPluginInfoTask(pluginInfo,callback,this);
				mPluginTaskEntry->push_back(task);
				task->startTask();
			}else{
				WDEBUG("command:find plugininfo in DB!");
			}
		}else{
			//TBD

		}

	}else if(command == UPDATE_COMMAND){

	}else if(command == TaskCommand::DOWNLOAD_COMMAND){

		bool isExist = false ;
		
		for(auto p : *mPluginTaskEntry){
			PluginTask *task = (PluginTask *)p;
			if(task->isContains(pluginInfo)){
				WDEBUG("command:TaskCommand.DOWNLOAD_COMMAND,added Request:" ,pluginInfo->getKey().c_str());
				task->addCallback(callback);
				isExist = true;
				break;
			}	
		}

		if(!isExist){

			PluginInfo *foundPlugin =  mPluginDBManager->getPlugin(pluginInfo);

			if(foundPlugin == NULL){
				WDEBUG("command:TaskCommand.DOWNLOAD_COMMAND,added Request:" ,pluginInfo->getKey().c_str());

				PluginTask* task = new DownloadPluginTask(pluginInfo,callback,this);
				mPluginTaskEntry->push_back(task);
				task->startTask();
			}else{
				WDEBUG("command:find plugininfo in DB!");
			}
		}else{
			//TBD

		}

	}else if(command == INSTALL_COMMAND){
		bool isExist = false ;

		for(auto p : *mPluginTaskEntry){
			PluginTask *task = (PluginTask *)p;
			if(task->isContains(pluginInfo)){
				WDEBUG("command:TaskCommand.INSTALL_COMMAND,added Request:" ,pluginInfo->getKey().c_str());
				task->addCallback(callback);
				isExist = true;
				break;
			}
		}

		if(!isExist){

			PluginInfo *foundPlugin = NULL;//= mPluginDBManager->getPlugin(pluginInfo);

			if(foundPlugin == NULL){
				WDEBUG("command:TaskCommand.INSTALL_COMMAND,added Request:" ,pluginInfo->getKey().c_str());

				PluginTask* task = new InstallPluginTask(pluginInfo,callback,this);
				mPluginTaskEntry->push_back(task);
				task->startTask();
			}else{
				WDEBUG("command:find plugininfo in DB!");
			}
		}else{
			//TBD

		}

	}else if(command == CLEAR_COMMAND){

	}else if(command == UPDATE_COMMAND){

		bool isExist = false ;

		for(auto p : *mPluginTaskEntry){
			PluginTask *task = (PluginTask *)p;
			if(task->isContains(pluginInfo)){
				WDEBUG("command:TaskCommand.INSTALL_COMMAND,added Request:" ,pluginInfo->getKey().c_str());
				task->addCallback(callback);
				isExist = true;
				break;
			}
		}

		if(!isExist){

			PluginInfo *foundPlugin = mPluginDBManager->getPlugin(pluginInfo);
			if(foundPlugin == NULL){
				WDEBUG("command:TaskCommand.UPDATE_COMMAND,Plugin not found in DB:");
				callback->onFailure(pluginInfo,PLUGIN_NOT_FOUND);

			}else{
				WDEBUG("command:find plugininfo in DB, Update it !!!!!");
				PluginTask *task = new UpdatePluginTask(pluginInfo,callback,this);
				mPluginTaskEntry->push_back(task);
				task->startTask();
			}
		}
	}
}

void PluginTaskManager::updateAllPlugins(PluginTaskCallback *callback){

	list<PluginInfo*>* pluginInfoList = mPluginDBManager->getPluginsWithStatus(PluginStatusCode::STATUS_VALID);
	WDEBUG("update ALL Plugins");
	for(PluginInfo* pluginInfo : *pluginInfoList){
		WDEBUG("send update command");
		command(pluginInfo,TaskCommand::UPDATE_COMMAND,callback);
	}
}

