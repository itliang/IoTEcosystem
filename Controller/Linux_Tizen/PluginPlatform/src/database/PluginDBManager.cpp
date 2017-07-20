/*
 * PluginDBManager.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: liujuan
 */

#include <database/constant/PluginInfoTable.h>
#include <database/PluginDBHelper.h>
#include <database/PluginDBManager.h>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <util/Log.h>
#include <util/Common.h>

using namespace std;

PluginDBManager::PluginDBManager(void)
{
}

PluginDBManager::~PluginDBManager(void)
{

}

PluginDBManager* PluginDBManager::pInstance = new PluginDBManager;

PluginDBManager *PluginDBManager::getInstance(void)
{
	return pInstance;
}

bool PluginDBManager::createPlugin(PluginInfo* pluginInfo) {
	return PluginDBHelper::getInstance()->createPlugin(pluginInfo);
	SAFE_DELETE(pluginInfo);
}

bool PluginDBManager::updatePlugin(PluginInfo* pluginInfo) {
	return PluginDBHelper::getInstance()->updatePlugin(pluginInfo);
	SAFE_DELETE(pluginInfo);
}

void PluginDBManager::markPreviousPluginToRemove(PluginInfo* pluginInfo) {
	if (pluginInfo == NULL) {
		WDEBUG("pluginInfo is null");
		return;
	}
	PluginDBHelper::getInstance()->markPreviousPluginToRemove(pluginInfo->getId(), pluginInfo->getVersionCode());
	SAFE_DELETE(pluginInfo);
}

void PluginDBManager::clearNotUsedPlugins() {
	PluginDBHelper::getInstance()->clearNotUsedPlugins();
}

void PluginDBManager::clearAllPlugins() {
	PluginDBHelper::getInstance()->clearAllPlugins();
}

void PluginDBManager::removePlugin(PluginInfo* pluginInfo) {
	if (pluginInfo == NULL) {
		WDEBUG("pluginInfo is null");
		return;
	}
	PluginDBHelper::getInstance()->removePlugin(pluginInfo->getId(), pluginInfo->getVersionCode());
	SAFE_DELETE(pluginInfo);
}

PluginInfo* PluginDBManager::getPlugin(PluginInfo* pluginInfo) {
	if (pluginInfo == NULL) {
		WDEBUG("pluginInfo is null");
		return NULL;
	}
	PluginInfo *result = NULL;
	if (pluginInfo->getId() != "") {
		result = PluginDBHelper::getInstance()->findPluginWithID(pluginInfo->getId(), pluginInfo->getVersionCode());
	}
	else if (pluginInfo->getDeviceType() != "") {
		result = PluginDBHelper::getInstance()->findPluginWithType(pluginInfo->getDeviceType(), pluginInfo->getDeviceSubType());
	}
	//SAFE_DELETE(pluginInfo);
	return result;
}

list<PluginInfo*>* PluginDBManager::getAllPlugins() {
	return PluginDBHelper::getInstance()->findAllPlugins();
}

list<PluginInfo*>* PluginDBManager::getPluginsWithStatus(PluginStatusCode status) {
	return PluginDBHelper::getInstance()->findPluginsWithStatus(status);
}

