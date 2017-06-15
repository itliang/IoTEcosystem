/*
 * UIPluginInfo.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#include "db/constant/UIPluginInfo.h"

const long serialVersionUID = 201L;

const std::string TABLE_NAME = "UI_PLUGIN_INFO";

const std::string COLUMN_NO = "no";
const std::string COLUMN_ID = "id";
//const std::string COLUMN_VERSION_CODE = L"version";
const std::string COLUMN_VERSION_CODE = "version_code";
const std::string COLUMN_VERSION_NAME = "version_name";
const std::string COLUMN_NAME = "name";
const std::string COLUMN_FILESIZE = "filesize";
//    const std::string COLUMN_INSTALLED = "installed";
const std::string COLUMN_DEVICE_TYPE = "device_type";
const std::string COLUMN_DEVICE_SUB_TYPE = "device_sub_type";
const std::string COLUMN_MODEL_ID = "model_id";
const std::string COLUMN_PLUGIN_PLATFORM_VERSION = "plugin_platform_version";
const std::string COLUMN_DEVICE_MANUFACTURER =  "device_manufacturer";
const std::string COLUMN_DEPENDENCIES = "dependencies";
//    const std::string COLUMN_TO_BE_DELETED =  L"to_be_deleted";
const std::string COLUMN_CURRENT_STATUS = "current_status";
const std::string COLUMN_LAST_CHECK_UPDATE = "last_check_update";
const std::string COLUMN_TIMESTAMP;

const int STATUS_DOWNLOADED = 0;
const int STATUS_INSTALLED = 1;
const int STATUS_IS_UPDATE_AVAILABLE = 2;
const int STATUS_TO_BE_DELETED = 3;
const int STATUS_OUTDATED = 4;

const std::string CREATE =
        "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + "(" +
                COLUMN_NO + " INTEGER primary key autoincrement, " +
                COLUMN_ID + " TEXT NOT NULL, " +
                COLUMN_VERSION_CODE + " TEXT NOT NULL, " +
                COLUMN_VERSION_NAME + " TEXT NOT NULL, " +
                COLUMN_NAME + " TEXT NOT NULL, " +
                COLUMN_FILESIZE + " INTEGER NOT NULL, " +
//                    COLUMN_INSTALLED + " INTEGER NOT NULL, " +
                COLUMN_DEVICE_TYPE + " TEXT, " +
                COLUMN_DEVICE_SUB_TYPE + " TEXT, " +
                COLUMN_MODEL_ID + " TEXT, " +
                COLUMN_PLUGIN_PLATFORM_VERSION + " TEXT, " +
                COLUMN_DEVICE_MANUFACTURER + " TEXT, " +
                COLUMN_DEPENDENCIES + " TEXT, " +
//                    COLUMN_TO_BE_DELETED + " INTEGER, " +
                COLUMN_CURRENT_STATUS + " INTEGER, " +
                COLUMN_LAST_CHECK_UPDATE + " INTEGER, " +
                COLUMN_TIMESTAMP + " DATETIME DEFAULT CURRENT_TIMESTAMP);";

const std::string DROP = "DROP TABLE IF EXISTS " + TABLE_NAME + ";";


