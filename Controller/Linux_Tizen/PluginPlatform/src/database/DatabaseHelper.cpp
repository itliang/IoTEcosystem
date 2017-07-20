/*
 * DatabaseHelper.cpp
 *
 *  Created on: Jun 19, 2017
 *      Author: liujuan
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "sqlite3.h"
#include "util/Log.h"
#include <app_common.h>
#include <database/constant/PluginInfoTable.h>
#include <database/PluginDBHelper.h>

#define DB_FILE_NAME "vmf.db"

DatabaseHelper::DatabaseHelper(void)
{

}
DatabaseHelper::~DatabaseHelper(void)
{

}

ErrorCode DatabaseHelper::openDb(sqlite3 ** database)
{
	FUNC_BEGIN();
	ErrorCode result = _ERR_NONE;
    int count = 0;
	if(*database == NULL) {
		int sqlResult = sqlite3_open(this->getDbFile(),  database);

		while ((sqlResult == SQLITE_BUSY || sqlResult == SQLITE_LOCKED) && count++ <= MAX_DB_RETRY_COUNT)
		{
	        WERROR("failed to sqlite3_open(): [%d], (%s), retry count=%d", sqlResult, sqlite3_errmsg(*database), count);
			usleep(DB_SLEEP_TIME);
			sqlResult = sqlite3_open(this->getDbFile(),  database);
		}

		if(sqlResult != SQLITE_OK) {
			WERROR("sqlite3_open fail : sqlResult = %d", sqlResult);
			result = _ERR_GENERIC_FAIL;

			if (*database) {
				sqlite3_close(*database);
				*database = NULL;
			}
		}
	}
	return result;
}

const char* DatabaseHelper::getDbFile()
{
	FUNC_BEGIN();
	static string dbFile;
	if (dbFile.empty() == true)
	{
		string path;
		char* dataPath = app_get_data_path();
		if (dataPath)
		{
			path = dataPath;
			free(dataPath);
		}
		dbFile.append(path);
//		dbFile.append("/");
		dbFile.append(DB_FILE_NAME);
	}
	WDEBUG("db file path is: %s",dbFile.c_str());
	return dbFile.c_str();
}
