/*
 * PluginDBHelper.h
 *
 *  Created on: Jun 14, 2017
 *      Author: liujuan
 */

#ifndef DATABASE_PLUGINDBHELPER_H_
#define DATABASE_PLUGINDBHELPER_H_

#include <string>
#include <map>
#include <list>
#include "sqlite3.h"
#include "constants/ErrorCode.hpp"
#include "database/constant/PluginStatusCode.h"
#include "database/DatabaseHelper.h"
#include "info/PluginInfo.hpp"

using namespace std;

class PluginDBHelper
{
public:
	virtual ~PluginDBHelper(void);
	static PluginDBHelper *getInstance(void);

	ErrorCode closeDb(void);
    ErrorCode beginTransaction();
    ErrorCode commitTransaction();

    ErrorCode execute(const string &query);
	sqlite3_stmt *prepareStatement(const string &query);
	ErrorCode stepStatement(sqlite3_stmt *pStatement);
	ErrorCode finalizeStatement(sqlite3_stmt *pStatement);
	ErrorCode stepAndFinalizeStatement(sqlite3_stmt *pStatement);
	ErrorCode resetStatement(sqlite3_stmt *pStatement);
	ErrorCode clearBindings(sqlite3_stmt *pStatement);

	list<PluginInfo*>* findAllPlugins();
	list<PluginInfo*>* findPluginsWithStatus(PluginStatusCode code);
	void clearNotUsedPlugins();
	void clearAllPlugins();
	void markPreviousPluginToRemove(string appID, string version_code);
	PluginInfo* findPluginWithType(string deviceType, string deviceSubType);
	PluginInfo* findPluginWithID(string appID, string version_code);
	bool createPlugin(PluginInfo* pluginInfo);
	bool updatePlugin(PluginInfo* pluginInfo);
	void removePlugin(string appID, string version_code);

private:
	PluginDBHelper(void);
	ErrorCode createTable(void);
    bool isDbExisted(const string &dbName);
    void bindInt(sqlite3_stmt *pStatement, int index, int value);
    void bindInt64(sqlite3_stmt *pStatement, int index, long value);
    void bindDouble(sqlite3_stmt *pStatement, int index, double value);
    void bindString(sqlite3_stmt *pStatement, int index, const string &value);
    void bindNull(sqlite3_stmt *pStatement, int index);
    int getColumnInt(sqlite3_stmt *pStatement, int index);
    long getColumnInt64(sqlite3_stmt *pStatement, int index);
    bool getColumnBool(sqlite3_stmt *pStatement, int index);
    const char *getColumnCString(sqlite3_stmt *pStatement, int index);

private:
	static PluginDBHelper *pInstance;
	DatabaseHelper *pDatabaseHelper;
	sqlite3 *pDbHandler;
};

#endif /* DATABASE_PLUGINDBHELPER_H_ */
