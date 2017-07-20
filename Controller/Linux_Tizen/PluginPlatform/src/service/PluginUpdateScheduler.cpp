#include <service/PluginUpdateScheduler.h>
#include "util/Log.h"
#include "info/ConfigInfo.hpp"


static PluginUpdateScheduler* _pInstance = NULL;

static long lastUpdateTime = 0;

unsigned char _func(void *data)
{
   //elm_flip_go(data, ELM_FLIP_CUBE_RIGHT);
	WDEBUG("func");
   return 0;
}

PluginUpdateScheduler::PluginUpdateScheduler(){
	isRunning = false;
	mTaskTimer = NULL;
	//ecore_timer_add(2.0,_func,NULL);
}

PluginUpdateScheduler* PluginUpdateScheduler::getInstance(){
	if(!_pInstance)
		_pInstance =  new PluginUpdateScheduler();
	return _pInstance;
}


void PluginUpdateScheduler::execute(bool isForce,PluginTaskCallback *callback){
	WDEBUG("execute");


	mRequestCallback = callback;
	if(!isRunning){
		if (!mTaskTimer)
		{
			WDEBUG("create timer");
			mTaskTimer =ecore_timer_add(10.0,sendRequest,NULL);
			if(mTaskTimer == NULL)
				WERROR("create timer failed");
		}
		else
		{
			ecore_timer_reset(mTaskTimer);
		}
#if 0
		long prevTime = getLastUpdateTime();
		if(prevTime < 0){
			ecore_timer_reset(mTaskTimer);
		}else{
			if(isForce){
				ecore_timer_reset(mTaskTimer);
			}else{
				//long delayTime = prevTime + ConfigInfo::getSchedulerTime()-
			}
		}
#endif
		isRunning = true;
	}else
		WDEBUG("plugin scheduler is already running");

}

long PluginUpdateScheduler::getLastUpdateTime(){
	return -1;
}

void PluginUpdateScheduler::setLastUpdateTime(long updateTime){

}

void PluginUpdateScheduler::deleteTimer()
{
	if(mTaskTimer)
	{
		ecore_timer_del(mTaskTimer);
		mTaskTimer = NULL;
	}
}

Eina_Bool PluginUpdateScheduler::sendRequest(void *data){
	WDEBUG("==sendRequest");
	_pInstance->setLastUpdateTime(1);
	_pInstance->mRequestCallback->request(REQUEST_TYPE_PLUGIN_UPDATE);
	_pInstance->mRequestCallback->request(REQUEST_TYPE_SERVER_UPDATE);
	return false;
}
