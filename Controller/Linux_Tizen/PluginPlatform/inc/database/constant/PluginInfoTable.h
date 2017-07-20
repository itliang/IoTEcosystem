/*
 * PluginInfoTable.h
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#ifndef DATABASE_CONSTANT_PLUGININFOTABLE_H_
#define DATABASE_CONSTANT_PLUGININFOTABLE_H_

#include <string>

extern const long serialVersionUID;

extern const std::string TABLE_NAME;

extern const std::string COLUMN_ID;
extern const std::string COLUMN_VERSION_CODE;
extern const std::string COLUMN_VERSION_NAME;
extern const std::string COLUMN_NAME;
extern const std::string COLUMN_FILESIZE;
extern const std::string COLUMN_DEVICE_TYPE;
extern const std::string COLUMN_DEVICE_SUB_TYPE;
extern const std::string COLUMN_MODEL_ID;
extern const std::string COLUMN_PLUGIN_PLATFORM_VERSION;
extern const std::string COLUMN_DEVICE_MANUFACTURER;
extern const std::string COLUMN_DEPENDENCIES;
extern const std::string COLUMN_PLUGIN_TYPE;
extern const std::string COLUMN_PLUGIN_STATUS;
extern const std::string COLUMN_DATA_STATUS;
extern const std::string COLUMN_LAST_CHECK_UPDATE;
extern const std::string COLUMN_TIMESTAMP;

//extern const int STATUS_PLUGIN_VALID;
//extern const int STATUS_PLUGIN_IS_UPDATE_AVAILABLE;
//extern const int STATUS_PLUGIN_TO_BE_DELETED;
//extern const int STATUS_PLUGIN_OUTDATED;

//extern const int STATUS_DATA_DOWNLOADED;
//extern const int STATUS_DATA_INSTALLED;

extern const int PLUGIN_TYPE_DEFAULT;
extern const int PLUGIN_TYPE_WEB;
extern const int PLUGIN_TYPE_LOCAL_TEST;
extern const int PLUGIN_TYPE_LOCAL_WEB_TEST;

extern const std::string CREATETABLE;

extern const std::string DROPTABLE_V1;
extern const std::string DROPTABLE;

#endif /* DATABASE_CONSTANT_PLUGININFOTABLE_H_ */
