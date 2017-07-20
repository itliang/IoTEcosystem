/*
 * PluginDBManager.h
 *
 *  Created on: Jun 14, 2017
 *      Author: liujuan
 */

#ifndef DATABASE_PLUGINDBMANAGER_H_
#define DATABASE_PLUGINDBMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <info/PluginInfo.hpp>
#include <database/constant/PluginStatusCode.h>

using namespace std;

class PluginDBManager
{
public:
	virtual ~PluginDBManager(void);
	static PluginDBManager *getInstance(void);

	bool createPlugin(PluginInfo* pluginInfo);
	bool updatePlugin(PluginInfo* pluginInfo);
	void markPreviousPluginToRemove(PluginInfo* pluginInfo);
	void clearNotUsedPlugins();
	void clearAllPlugins();
	void removePlugin(PluginInfo* pluginInfo);
	PluginInfo* getPlugin(PluginInfo* pluginInfo);
	list<PluginInfo*>* getAllPlugins();
	list<PluginInfo*>* getPluginsWithStatus(PluginStatusCode status);

private:
	PluginDBManager(void);

private:
	static PluginDBManager *pInstance;
};


#endif /* DATABASE_PLUGINDBMANAGER_H_ */
