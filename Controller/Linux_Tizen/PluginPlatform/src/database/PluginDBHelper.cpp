/*
 * PluginDBHelper.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: liujuan
 */

#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include <fcntl.h>
#include <app_common.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <database/constant/PluginInfoTable.h>
#include <database/PluginDBHelper.h>
#include <util/Common.h>
#include "util/Log.h"
#include "util/Time.h"
#include "database/DatabaseHelper.h"

#define ASSIGN_CHARS_WITH_NULL_CHECK(src, dest) \
        do	\
        {	\
            const char *tmp = (src);	\
            (dest) = (tmp ? string((const char *)tmp) : "");	\
        } while (0)

#define BIND_STRING_WITH_NULL_CHECK(DB, statement, column, str)	\
        do \
        { \
            NULL == (str) ? (DB)->bindNull((statement), (column)) \
                    : (DB)->bindString((statement), (column), (str)); \
        } while (0)

#define BIND_INT64(DB, statement, column, int64)	\
        do \
        { \
            (DB)->bindInt64((statement), (column), (int64)); \
        } while (0)

#define BIND_BOOL(DB, statement, column, int)	\
        do \
        { \
            (DB)->bindBool((statement), (column), (int)); \
        } while (0)

using namespace boost::property_tree;

PluginDBHelper::PluginDBHelper(void)
{
	pDbHandler = NULL;
	pDatabaseHelper = NULL;
}

PluginDBHelper::~PluginDBHelper(void)
{

}

PluginDBHelper* PluginDBHelper::pInstance = new PluginDBHelper;

PluginDBHelper *PluginDBHelper::getInstance(void)
{
	return pInstance;
}

ErrorCode PluginDBHelper::closeDb(void)
{
	FUNC_BEGIN();
	ErrorCode result = _ERR_NONE;
	int sqlResult = SQLITE_OK;
	int count = 0;
	if (pDbHandler == NULL) {
		return _ERR_GENERIC_FAIL;
	}

	sqlResult = sqlite3_close(pDbHandler);

	while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
	{
		WERROR("failed to sqlite3_close(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(pDbHandler), count);
		usleep(DB_SLEEP_TIME);
		sqlResult = sqlite3_close(pDbHandler);
	}

	if (sqlResult != SQLITE_OK) {
		WERROR("failed to sqlite3_close(): [%d], (%s)", sqlResult, sqlite3_errmsg(pDbHandler));
		if (pDbHandler) {
			sqlite3_close(pDbHandler);
			pDbHandler = NULL;
		}
		result = _ERR_GENERIC_FAIL;
	}
	pDbHandler = NULL;
	return result;
}

ErrorCode PluginDBHelper::beginTransaction()
{
	FUNC_BEGIN();
	char *errmsg = NULL;
    ErrorCode result = _ERR_NONE;
    int sqlResult = SQLITE_OK;

    if(pDbHandler == NULL) {
    	if (!pDatabaseHelper) {
        	pDatabaseHelper = new DatabaseHelper();
    	}
        result = pDatabaseHelper->openDb(&pDbHandler);
        if(result != _ERR_NONE) {
            return result;
        }
        result = this->createTable();
        if(result != _ERR_NONE) {
			return result;
		}
    }

	sqlResult = sqlite3_exec(pDbHandler, "BEGIN", NULL, 0, &errmsg);
	if(sqlResult != SQLITE_OK){
		WERROR("sqlite3_open fail : sqlResult: %d , errmsg: %s", sqlResult, errmsg);
		sqlite3_free(errmsg);
		return _ERR_GENERIC_FAIL;
	}

    return result;
}

ErrorCode PluginDBHelper::commitTransaction()
{
	FUNC_BEGIN();
	char *errmsg = NULL;
    int sqlResult = SQLITE_OK;

	sqlResult = sqlite3_exec(pDbHandler, "COMMIT", NULL, 0, &errmsg);
	if(sqlResult != SQLITE_OK){
		WERROR("sqlite3_open fail : sqlResult: %d , errmsg: %s", sqlResult, errmsg);
		sqlite3_free(errmsg);
		return _ERR_GENERIC_FAIL;
	}

    return _ERR_NONE;
}

ErrorCode PluginDBHelper::execute(const string &query)
{
	FUNC_BEGIN();
    ErrorCode result = _ERR_NONE;
    sqlite3_stmt *pStatement = prepareStatement(query);
    if (pStatement == NULL) {
    	return _ERR_GENERIC_FAIL;
    }

    result = stepStatement(pStatement);

    if(result != _ERR_NONE && result != _ERR_ALREADY_DONE) {
        WERROR("failed to stepStatement errorCode = %d", result);
    }
    result = finalizeStatement(pStatement);

    if(result != _ERR_NONE) {
        WERROR("failed to finalizeStatement errorCode = %d", result);
    }
    return result;
}

sqlite3_stmt * PluginDBHelper::prepareStatement(const string &query)
{
	FUNC_BEGIN();
    sqlite3_stmt *pStatement = NULL;
    int sqlResult = SQLITE_OK;
    ErrorCode result = _ERR_NONE;
    int count = 0;

    if(query.size() == 0) {
        return NULL;
    }

    if(pDbHandler == NULL) {
    	if (!pDatabaseHelper) {
			pDatabaseHelper = new DatabaseHelper();
		}
		result = pDatabaseHelper->openDb(&pDbHandler);
		if(result != _ERR_NONE) {
			return NULL;
		}
		result = this->createTable();
		if(result != _ERR_NONE) {
			return NULL;
		}
    }

    WDEBUG("query = %s", query.c_str());
    sqlResult = sqlite3_prepare_v2(pDbHandler, query.c_str(), query.size(), &pStatement, NULL);

    while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
    {
        WERROR("failed to sqlite3_prepare_v2(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(pDbHandler), count);
        usleep(DB_SLEEP_TIME);
        sqlResult = sqlite3_prepare_v2(pDbHandler, query.c_str(), query.size(), &pStatement, NULL);
    }

    if (sqlResult != SQLITE_OK) {
        WERROR("failed to sqlite3_prepare_v2(): [%d], (%s)", sqlResult, sqlite3_errmsg(pDbHandler));
        if(pStatement){
			sqlite3_finalize(pStatement);
			pStatement = NULL;
		}
        return NULL;
    }
    return pStatement;
}

ErrorCode PluginDBHelper::stepStatement(sqlite3_stmt *pStatement)
{
	FUNC_BEGIN();
    ErrorCode result = _ERR_NONE;
    int sqlResult = SQLITE_OK;
    int count = 0;
    if (pStatement == NULL) {
    	return _ERR_GENERIC_FAIL;
    }

    sqlResult = sqlite3_step(pStatement);

    while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
    {
        WERROR("failed to sqlite3_step(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(pDbHandler), count);
        usleep(DB_SLEEP_TIME);
        sqlResult = sqlite3_step(pStatement);
    }

    if(sqlResult != SQLITE_DONE && sqlResult != SQLITE_ROW) {
        WERROR("failed to sqlite3_step(): [%d], (%s)", sqlResult, sqlite3_errmsg(pDbHandler));
        result = _ERR_GENERIC_FAIL;

    } else if(sqlResult == SQLITE_DONE) {
        result = _ERR_ALREADY_DONE;
        WDEBUG("stepStatement SQLITE_DONE");
    }
    return result;
}

ErrorCode PluginDBHelper::finalizeStatement(sqlite3_stmt *pStatement)
{
	FUNC_BEGIN();
    ErrorCode result = _ERR_NONE;
    int sqlResult = SQLITE_OK;
    int count = 0;
    if (pStatement == NULL) {
    	return _ERR_GENERIC_FAIL;
    }

    sqlResult = sqlite3_finalize(pStatement);

    while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
    {
        WERROR("failed to sqlite3_finalize(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(pDbHandler), count);
        usleep(DB_SLEEP_TIME);
        sqlResult = sqlite3_finalize(pStatement);
    }

    if(sqlResult != SQLITE_OK) {
        WERROR("failed to sqlite3_finalize(): [%d], (%s)", sqlResult, sqlite3_errmsg(pDbHandler));
        result = _ERR_GENERIC_FAIL;
    }
    pStatement = NULL;
    return result;
}

ErrorCode  PluginDBHelper::stepAndFinalizeStatement(sqlite3_stmt *pStatement)
{
	FUNC_BEGIN();
    ErrorCode result = _ERR_NONE;
    if (pStatement == NULL) {
    	return _ERR_GENERIC_FAIL;
    }

    result = stepStatement(pStatement);

    if(result != _ERR_NONE && result != _ERR_ALREADY_DONE) {
        WERROR("failed to stepStatement errorCode = %d", result);
    }
    result = finalizeStatement(pStatement);

    if(result != _ERR_NONE) {
        WERROR("failed to finalizeStatement errorCode = %d", result);
    }
    return result;
}

ErrorCode PluginDBHelper::resetStatement(sqlite3_stmt *pStatement)
{
	FUNC_BEGIN();
    ErrorCode result = _ERR_NONE;
    int sqlResult = SQLITE_OK;
    int count = 0;
    if (pStatement == NULL) {
    	return _ERR_GENERIC_FAIL;
    }

    sqlResult = sqlite3_reset(pStatement);

    while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
    {
        WERROR("failed to sqlite3_reset(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(pDbHandler), count);
        usleep(DB_SLEEP_TIME);
        sqlResult = sqlite3_reset(pStatement);
    }

    if(sqlResult != SQLITE_OK) {
        WERROR("failed to sqlite3_reset(): [%d], (%s)", sqlResult, sqlite3_errmsg(pDbHandler));
        result = _ERR_GENERIC_FAIL;
    }
    return result;
}

ErrorCode PluginDBHelper::clearBindings(sqlite3_stmt *pStatement)
{
	FUNC_BEGIN();
    ErrorCode result = _ERR_NONE;
    int sqlResult = SQLITE_OK;
    int count = 0;
    if (pStatement == NULL) {
    	return _ERR_GENERIC_FAIL;
    }

    sqlResult = sqlite3_clear_bindings(pStatement);

    while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
    {
        WERROR("failed to sqlite3_clear_bindings(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(pDbHandler), count);
        usleep(DB_SLEEP_TIME);
        sqlResult = sqlite3_clear_bindings(pStatement);
    }

    if(sqlResult != SQLITE_OK) {
        WERROR("failed to sqlite3_clear_bindings(): [%d], (%s)", sqlResult, sqlite3_errmsg(pDbHandler));
        result = _ERR_GENERIC_FAIL;
    }
    return result;
}

list<PluginInfo*>* PluginDBHelper::findAllPlugins()
{
	FUNC_BEGIN();
	list<PluginInfo*> *pluginInfoList = new list<PluginInfo*>();

	string query_execute = "select * from " + TABLE_NAME + " ORDER BY " + COLUMN_VERSION_CODE + " DESC ";

	sqlite3_stmt *pStatement = this->prepareStatement(query_execute);

	while (this->stepStatement(pStatement) == _ERR_NONE) {
		PluginInfo *pItem = new PluginInfo();
		pItem->setId(SAFE_COPY(this->getColumnCString(pStatement, 0)));
		pItem->setVersionCode(SAFE_COPY(this->getColumnCString(pStatement, 1)));
		pItem->setVersionName(SAFE_COPY(this->getColumnCString(pStatement, 2)));
		pItem->setName(SAFE_COPY(this->getColumnCString(pStatement, 3)));
		pItem->setFileSize(this->getColumnInt(pStatement, 4));
		pItem->setDeviceType(SAFE_COPY(this->getColumnCString(pStatement, 5)));
		pItem->setDeviceSubType(SAFE_COPY(this->getColumnCString(pStatement, 6)));
		pItem->setModelID(SAFE_COPY(this->getColumnCString(pStatement, 7)));
		pItem->setPlatformVersion(SAFE_COPY(this->getColumnCString(pStatement, 8)));
		pItem->setDeviceManufacturer(SAFE_COPY(this->getColumnCString(pStatement, 9)));
		pItem->setDependencies(SAFE_COPY(this->getColumnCString(pStatement, 10)));
		pItem->setPluginType(this->getColumnInt(pStatement, 11));
		pItem->setPluginStatus(this->getColumnInt(pStatement, 12));
		pItem->setPluginDataStatus(this->getColumnInt(pStatement, 13));
		pItem->setLastCheckUpdate(this->getColumnInt64(pStatement, 14));
		pItem->setTimeStamp(this->getColumnInt64(pStatement, 15));

		pluginInfoList->push_back(pItem);
	}
	this->finalizeStatement(pStatement);
	return pluginInfoList;
}

list<PluginInfo*>* PluginDBHelper::findPluginsWithStatus(PluginStatusCode code)
{
	FUNC_BEGIN();
	list<PluginInfo*>* pluginInfoList = new list<PluginInfo*>();

	string query_execute = "select * from " + TABLE_NAME + " where " + COLUMN_PLUGIN_STATUS + " =? " + " ORDER BY " + COLUMN_VERSION_CODE + " DESC ";

	sqlite3_stmt *pStatement = this->prepareStatement(query_execute);
	this->bindInt(pStatement, 1, code);

	while (this->stepStatement(pStatement) == _ERR_NONE) {
		PluginInfo *pItem = new PluginInfo();
		pItem->setId(SAFE_COPY(this->getColumnCString(pStatement, 1)));
		pItem->setVersionCode(SAFE_COPY(this->getColumnCString(pStatement, 2)));
		pItem->setVersionName(SAFE_COPY(this->getColumnCString(pStatement, 3)));
		pItem->setName(SAFE_COPY(this->getColumnCString(pStatement, 4)));
		pItem->setFileSize(this->getColumnInt(pStatement, 5));
		pItem->setDeviceType(SAFE_COPY(this->getColumnCString(pStatement, 6)));
		pItem->setDeviceSubType(SAFE_COPY(this->getColumnCString(pStatement, 7)));
		pItem->setModelID(SAFE_COPY(this->getColumnCString(pStatement, 8)));
		pItem->setPlatformVersion(SAFE_COPY(this->getColumnCString(pStatement, 9)));
		pItem->setDeviceManufacturer(SAFE_COPY(this->getColumnCString(pStatement, 10)));
		pItem->setDependencies(SAFE_COPY(this->getColumnCString(pStatement, 11)));
		pItem->setPluginType(this->getColumnInt(pStatement, 12));
		pItem->setPluginStatus(this->getColumnInt(pStatement, 13));
		pItem->setPluginDataStatus(this->getColumnInt(pStatement, 14));
		pItem->setLastCheckUpdate(this->getColumnInt64(pStatement, 15));
		pItem->setTimeStamp(this->getColumnInt64(pStatement, 16));

		pluginInfoList->push_back(pItem);
	}
	this->finalizeStatement(pStatement);
	return pluginInfoList;
}

void PluginDBHelper::clearNotUsedPlugins()
{
	FUNC_BEGIN();
	string query_execute = "delete from " + TABLE_NAME + " where " + COLUMN_PLUGIN_STATUS + " =? ";

	sqlite3_stmt *pStatement = this->prepareStatement(query_execute);

	this->bindInt(pStatement, 1, STATUS_TO_BE_DELETED);

	this->stepAndFinalizeStatement(pStatement);
}

void PluginDBHelper::clearAllPlugins()
{
	FUNC_BEGIN();
	string query_execute = "delete from " + TABLE_NAME;

	sqlite3_stmt *pStatement = this->prepareStatement(query_execute);
	this->stepAndFinalizeStatement(pStatement);
}

void PluginDBHelper::markPreviousPluginToRemove(string appID, string version_code)
{
	FUNC_BEGIN();
	if(version_code != "") {
		string query_execute = "UPDATE " + TABLE_NAME + " set plugin_status=?, timestamp=? where " + COLUMN_ID + " =? and " + COLUMN_VERSION_CODE + " =? ";

		sqlite3_stmt *pStatement = this->prepareStatement(query_execute);

		this->bindInt(pStatement, 1, STATUS_TO_BE_DELETED);
		this->bindDouble(pStatement,2,Time::getTime());
		BIND_STRING_WITH_NULL_CHECK(this, pStatement, 3, appID.c_str());
		BIND_STRING_WITH_NULL_CHECK(this, pStatement, 4, version_code.c_str());

		this->stepAndFinalizeStatement(pStatement);
	}
}

PluginInfo* PluginDBHelper::findPluginWithType(string deviceType, string deviceSubType)
{
	FUNC_BEGIN();
	PluginInfo* pPluginInfo = NULL;
	sqlite3_stmt *pStatement;

	if(deviceSubType != "") {
		string query_execute = "select * from " + TABLE_NAME + " where " + COLUMN_DEVICE_TYPE + " =? and " + COLUMN_DEVICE_SUB_TYPE + " =? and "
				+ COLUMN_DATA_STATUS + " =? " + " ORDER BY " + COLUMN_VERSION_CODE + " DESC LIMIT 1";

		pStatement = this->prepareStatement(query_execute);
		this->bindString(pStatement, 1, (char *) deviceType.c_str());
		this->bindString(pStatement, 2, (char *) deviceSubType.c_str());
		this->bindInt(pStatement,3,STATUS_DATA_INSTALLED);

	} else {
		string query_execute = "select * from " + TABLE_NAME + " where " + COLUMN_DEVICE_TYPE + " =? and "
				+ COLUMN_DATA_STATUS + " =? " + " ORDER BY " + COLUMN_VERSION_CODE + " DESC LIMIT 1";
		pStatement = this->prepareStatement(query_execute);
		this->bindString(pStatement, 1, (char *) deviceType.c_str());
		this->bindInt(pStatement,2,STATUS_DATA_INSTALLED);
	}

	while (this->stepStatement(pStatement) == _ERR_NONE) {
		pPluginInfo = new PluginInfo();
		pPluginInfo->setId(SAFE_COPY(this->getColumnCString(pStatement, 0)));
		pPluginInfo->setVersionCode(SAFE_COPY(this->getColumnCString(pStatement, 1)));
		pPluginInfo->setVersionName(SAFE_COPY(this->getColumnCString(pStatement, 2)));
		pPluginInfo->setName(SAFE_COPY(this->getColumnCString(pStatement, 3)));
		pPluginInfo->setFileSize(this->getColumnInt(pStatement, 4));
		pPluginInfo->setDeviceType(SAFE_COPY(this->getColumnCString(pStatement, 5)));
		pPluginInfo->setDeviceSubType(SAFE_COPY(this->getColumnCString(pStatement, 6)));
		pPluginInfo->setModelID(SAFE_COPY(this->getColumnCString(pStatement, 7)));
		pPluginInfo->setPlatformVersion(SAFE_COPY(this->getColumnCString(pStatement, 8)));
		pPluginInfo->setDeviceManufacturer(SAFE_COPY(this->getColumnCString(pStatement, 9)));
		pPluginInfo->setDependencies(SAFE_COPY(this->getColumnCString(pStatement, 10)));
		pPluginInfo->setPluginType(this->getColumnInt(pStatement, 11));
		pPluginInfo->setPluginStatus(this->getColumnInt(pStatement, 12));
		pPluginInfo->setPluginDataStatus(this->getColumnInt(pStatement, 13));
		pPluginInfo->setLastCheckUpdate(this->getColumnInt64(pStatement, 14));
		pPluginInfo->setTimeStamp(this->getColumnInt64(pStatement, 15));
		break;
	}
	this->finalizeStatement(pStatement);
	return pPluginInfo;
}

PluginInfo* PluginDBHelper::findPluginWithID(string appID, string version_code)
{
	FUNC_BEGIN();
	PluginInfo* pPluginInfo = NULL;
	sqlite3_stmt *pStatement;

	if(version_code != "") {
		string query_execute = "select * from " + TABLE_NAME + " where " + COLUMN_ID + " =? and " +
				COLUMN_VERSION_CODE + " =? " + " ORDER BY " + COLUMN_VERSION_CODE + " DESC LIMIT 1";

		pStatement = this->prepareStatement(query_execute);
		this->bindString(pStatement, 1, (char *) appID.c_str());
		this->bindString(pStatement, 2, (char *) version_code.c_str());

	} else {
		string query_execute = "select * from " + TABLE_NAME + " where " + COLUMN_ID + " =? and " +
				COLUMN_DATA_STATUS + " =? " + " ORDER BY " + COLUMN_VERSION_CODE + " DESC LIMIT 1";

		pStatement = this->prepareStatement(query_execute);
		this->bindString(pStatement, 1, (char *) appID.c_str());
		this->bindInt(pStatement,2,STATUS_DATA_INSTALLED);
	}

	while (this->stepStatement(pStatement) == _ERR_NONE) {
		pPluginInfo = new PluginInfo();
		pPluginInfo->setId(SAFE_COPY(this->getColumnCString(pStatement, 0)));
		pPluginInfo->setVersionCode(SAFE_COPY(this->getColumnCString(pStatement, 1)));
		pPluginInfo->setVersionName(SAFE_COPY(this->getColumnCString(pStatement, 2)));
		pPluginInfo->setName(SAFE_COPY(this->getColumnCString(pStatement, 3)));
		pPluginInfo->setFileSize(this->getColumnInt(pStatement, 4));
		pPluginInfo->setDeviceType(SAFE_COPY(this->getColumnCString(pStatement, 5)));
		pPluginInfo->setDeviceSubType(SAFE_COPY(this->getColumnCString(pStatement, 6)));
		pPluginInfo->setModelID(SAFE_COPY(this->getColumnCString(pStatement, 7)));
		pPluginInfo->setPlatformVersion(SAFE_COPY(this->getColumnCString(pStatement, 8)));
		pPluginInfo->setDeviceManufacturer(SAFE_COPY(this->getColumnCString(pStatement, 9)));
		pPluginInfo->setDependencies(SAFE_COPY(this->getColumnCString(pStatement, 10)));
		pPluginInfo->setPluginType(this->getColumnInt(pStatement, 11));
		pPluginInfo->setPluginStatus(this->getColumnInt(pStatement, 12));
		pPluginInfo->setPluginDataStatus(this->getColumnInt(pStatement, 13));
		pPluginInfo->setLastCheckUpdate(this->getColumnInt64(pStatement, 14));
		pPluginInfo->setTimeStamp(this->getColumnInt64(pStatement, 15));
		break;
	}
	this->finalizeStatement(pStatement);
	return pPluginInfo;
}

bool PluginDBHelper::createPlugin(PluginInfo* pluginInfo)
{
	FUNC_BEGIN();
	if (pluginInfo == NULL) {
		WDEBUG("pluginInfo is null");
		return false;
	}
	string query_execute = "INSERT INTO " + TABLE_NAME +" (id, version_code, version_name, name, filesize, device_type, "
			"device_sub_type, model_id, plugin_platform_version, device_manufacturer, dependencies, "
			"plugin_type, plugin_status, data_status, last_check_update, timestamp) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

	sqlite3_stmt *pStatement = this->prepareStatement(query_execute);

	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 1, pluginInfo->getId().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 2, pluginInfo->getVersionCode().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 3, pluginInfo->getVersionName().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 4, pluginInfo->getName().c_str());
	this->bindInt(pStatement, 5, pluginInfo->getFileSize());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 6, Common::toLower(pluginInfo->getDeviceType()).c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 7, Common::toLower(pluginInfo->getDeviceSubType()).c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 8, pluginInfo->getModelID().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 9, pluginInfo->getPlatformVersion().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 10, pluginInfo->getDeviceManufacturer().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 11, pluginInfo->getDependencies().c_str());
	this->bindInt(pStatement, 12, pluginInfo->getPluginType());
	this->bindInt(pStatement, 13, pluginInfo->getPluginStatus());
	this->bindInt(pStatement, 14, pluginInfo->getPluginDataStatus());
	this->bindDouble(pStatement, 15, Time::getTime());
	this->bindDouble(pStatement,16,Time::getTime());

	ErrorCode ret = this->stepAndFinalizeStatement(pStatement);
	if (ret != _ERR_NONE) {
		return false;
	}
	return true;
}

bool PluginDBHelper::updatePlugin(PluginInfo* pluginInfo)
{
	FUNC_BEGIN();

	if (pluginInfo == NULL) {
		WDEBUG("pluginInfo is null");
		return false;
	}

	string query_execute = "UPDATE " + TABLE_NAME + " set id=?, version_code=?, version_name=?, name=?, filesize=?, "
			"device_type=?, device_sub_type=?, model_id=?, plugin_platform_version=?, device_manufacturer=?, dependencies=?, "
			"plugin_type=?, plugin_status=?, data_status=?, last_check_update=?, timestamp=? where "
			+ COLUMN_ID + " =? and " + COLUMN_VERSION_CODE + " =? ";

	sqlite3_stmt *pStatement = this->prepareStatement(query_execute);

	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 1, pluginInfo->getId().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 2, pluginInfo->getVersionCode().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 3, pluginInfo->getVersionName().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 4, pluginInfo->getName().c_str());
	this->bindInt(pStatement, 5, pluginInfo->getFileSize());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 6, Common::toLower(pluginInfo->getDeviceType()).c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 7, Common::toLower(pluginInfo->getDeviceSubType()).c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 8, pluginInfo->getModelID().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 9, pluginInfo->getPlatformVersion().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 10, pluginInfo->getDeviceManufacturer().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 11, pluginInfo->getDependencies().c_str());
	this->bindInt(pStatement, 12, pluginInfo->getPluginType());
	this->bindInt(pStatement, 13, pluginInfo->getPluginStatus());
	this->bindInt(pStatement, 14, pluginInfo->getPluginDataStatus());
	this->bindInt64(pStatement, 15, pluginInfo->getLastCheckUpdate());
	this->bindDouble(pStatement,16,Time::getTime());

	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 17, pluginInfo->getId().c_str());
	BIND_STRING_WITH_NULL_CHECK(this, pStatement, 18, pluginInfo->getVersionCode().c_str());

	this->stepAndFinalizeStatement(pStatement);
	return true;
}

void PluginDBHelper::removePlugin(string appID, string version_code)
{
	FUNC_BEGIN();
	if(version_code != "") {
		string query_execute = "delete from " + TABLE_NAME + " where " + COLUMN_ID + " =? and " + COLUMN_VERSION_CODE + " =? ";

		sqlite3_stmt *pStatement = this->prepareStatement(query_execute);

		BIND_STRING_WITH_NULL_CHECK(this, pStatement, 1, appID.c_str());
		BIND_STRING_WITH_NULL_CHECK(this, pStatement, 2, version_code.c_str());

		this->stepAndFinalizeStatement(pStatement);
	}
}


//===================================================PRIVATE FUNCTION==============================================

ErrorCode PluginDBHelper::createTable(void)
{
	FUNC_BEGIN();
	ErrorCode ret = this->execute(CREATETABLE);
	return ret;
}

bool PluginDBHelper::isDbExisted(const string &dbPath)
{
	FUNC_BEGIN();
    int fd = open(dbPath.c_str(), O_RDONLY);
    bool result = (fd == -1) ? false : true;
    if (result) {
        close(fd);
    }
    WDEBUG("isDbExisted(%s): %d", dbPath.c_str(), result);
    return result;
}

void PluginDBHelper::bindInt(sqlite3_stmt *pStatement, int index, int value)
{
    if (pStatement == NULL) {
    	return;
    }
    WDEBUG("index =%d, value =%d", index, value);
    if (pStatement == NULL) {
    	return;
    }
    sqlite3_bind_int(pStatement, index, value);
}


void PluginDBHelper::bindInt64(sqlite3_stmt *pStatement, int index, long value)
{
    if (pStatement == NULL) {
    	return;
    }
    sqlite3_bind_int64(pStatement, index, value);
}

void PluginDBHelper::bindDouble(sqlite3_stmt *pStatement, int index, double value)
{
    if (pStatement == NULL) {
    	return;
    }
    sqlite3_bind_double(pStatement, index, value);
}

void PluginDBHelper::bindString(sqlite3_stmt *pStatement, int index, const string &value)
{
    if (pStatement == NULL) {
    	return;
    }
    int valueLength = value.size();

    if (valueLength) {
        WDEBUG("index =%d, value =%s, valueLength=%d", index, value.c_str(), valueLength);
        sqlite3_bind_text(pStatement, index, value.c_str(), valueLength, SQLITE_TRANSIENT);
    } else {
        WDEBUG("index =%d, value = NULL", index);
        bindNull(pStatement, index);
    }
}

void PluginDBHelper::bindNull(sqlite3_stmt *pStatement, int index)
{
    if (pStatement == NULL) {
    	return;
    }

    sqlite3_bind_null(pStatement, index);
}

int PluginDBHelper::getColumnInt(sqlite3_stmt *pStatement, int index)
{
    if (pStatement == NULL) {
    	return -1;
    }

    int value = (int)sqlite3_column_int(pStatement, index);

    WDEBUG("index =%d, value =%d", index, value);
    return value;
}

long PluginDBHelper::getColumnInt64(sqlite3_stmt *pStatement, int index)
{
    if (pStatement == NULL) {
    	return -1;
    }

    long value = (long)sqlite3_column_int64(pStatement, index);

    WDEBUG("index =%d, value =%d", index, value);
    return value;
}

bool PluginDBHelper::getColumnBool(sqlite3_stmt *pStatement, int index)
{
    if (pStatement == NULL) {
    	return false;
    }

    bool value = (bool)sqlite3_column_int(pStatement, index);

    WDEBUG("index =%d, value =%d", index, value);
    return value;
}

const char *PluginDBHelper::getColumnCString(sqlite3_stmt *pStatement, int index)
{
    if (pStatement == NULL) {
    	return "";
    }

    char *value = (char *)sqlite3_column_text(pStatement, index);

    WDEBUG("index =%d, value =%s", index, value);
    return value;
}
