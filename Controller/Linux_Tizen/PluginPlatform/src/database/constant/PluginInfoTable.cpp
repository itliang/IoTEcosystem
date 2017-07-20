/*
 * PluginInfoTable.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#include <database/constant/PluginInfoTable.h>

const std::string TABLE_NAME = "PLUGIN_INFO";

const std::string COLUMN_ID = "id";
const std::string COLUMN_VERSION_CODE = "version_code";
const std::string COLUMN_VERSION_NAME = "version_name";
const std::string COLUMN_NAME = "name";
const std::string COLUMN_FILESIZE = "filesize";
const std::string COLUMN_DEVICE_TYPE = "device_type";
const std::string COLUMN_DEVICE_SUB_TYPE = "device_sub_type";
const std::string COLUMN_MODEL_ID = "model_id";
const std::string COLUMN_PLUGIN_PLATFORM_VERSION = "plugin_platform_version";
const std::string COLUMN_DEVICE_MANUFACTURER =  "device_manufacturer";
const std::string COLUMN_DEPENDENCIES = "dependencies";
const std::string COLUMN_PLUGIN_TYPE = "plugin_type";
const std::string COLUMN_PLUGIN_STATUS = "plugin_status";
const std::string COLUMN_DATA_STATUS = "data_status";
const std::string COLUMN_LAST_CHECK_UPDATE = "last_check_update";
const std::string COLUMN_TIMESTAMP = "timestamp";

//const int STATUS_PLUGIN_VALID = 0;
//const int STATUS_PLUGIN_IS_UPDATE_AVAILABLE = 1;
//const int STATUS_PLUGIN_TO_BE_DELETED = 2;
//const int STATUS_PLUGIN_OUTDATED = 3;

//const int STATUS_DATA_DOWNLOADED = 0;
//const int STATUS_DATA_INSTALLED = 1;

const int PLUGIN_TYPE_DEFAULT= 0;
const int PLUGIN_TYPE_WEB= 1;
const int PLUGIN_TYPE_LOCAL_TEST= 10;
const int PLUGIN_TYPE_LOCAL_WEB_TEST= 11;

const std::string CREATETABLE =
		"CREATE TABLE IF NOT EXISTS " + TABLE_NAME + "(" +
		                    COLUMN_ID + " TEXT NOT NULL, " +
		                    COLUMN_VERSION_CODE + " TEXT NOT NULL, " +
		                    COLUMN_VERSION_NAME + " TEXT, " +
		                    COLUMN_NAME + " TEXT NOT NULL, " +
		                    COLUMN_FILESIZE + " INTEGER DEFAULT 0, " +
		                    COLUMN_DEVICE_TYPE + " TEXT, " +
		                    COLUMN_DEVICE_SUB_TYPE + " TEXT, " +
		                    COLUMN_MODEL_ID + " TEXT, " +
		                    COLUMN_PLUGIN_PLATFORM_VERSION + " TEXT, " +
		                    COLUMN_DEVICE_MANUFACTURER + " TEXT, " +
		                    COLUMN_DEPENDENCIES + " TEXT, " +
		                    COLUMN_PLUGIN_TYPE + " INTEGER DEFAULT 0, " +
		                    COLUMN_PLUGIN_STATUS + " INTEGER DEFAULT 0, " +
		                    COLUMN_DATA_STATUS + " INTEGER DEFAULT 0, " +
		                    COLUMN_LAST_CHECK_UPDATE + " DATETIME DEFAULT CURRENT_TIMESTAMP, " +
		                    COLUMN_TIMESTAMP + " DATETIME DEFAULT CURRENT_TIMESTAMP," +
		                    "PRIMARY KEY (["+COLUMN_ID+"], ["+COLUMN_VERSION_CODE+"]));";

const std::string DROPTABLE_V1 = "DROP TABLE IF EXISTS UI_PLUGIN_INFO;";
const std::string DROPTABLE = "DROP TABLE IF EXISTS " + TABLE_NAME + ";";
