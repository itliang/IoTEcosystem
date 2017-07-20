#ifndef __GALAXY_APPS_REQUEST_H__
#define __GALAXY_APPS_REQUEST_H__

#include <memory>
#include <map>
#include <string>

#include "callback/PluginTaskCallback.h"
using namespace std;



class PluginUpdateScheduler{

public:
	PluginUpdateScheduler();

	static PluginUpdateScheduler* getInstance();
	PluginTaskCallback *mRequestCallback;
	void execute(bool isForce,PluginTaskCallback *callback);

private:

	long getLastUpdateTime();
	void setLastUpdateTime(long updateTime);
	bool isRunning;
	//Ecore_Timer *mTaskTimer;
	//static Eina_Bool sendRequest(void *data);
	void deleteTimer();

};

#endif
