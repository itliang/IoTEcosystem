/*
 * UIPluginInfo.h
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#ifndef DB_CONSTANT_UIPLUGININFO_H_
#define DB_CONSTANT_UIPLUGININFO_H_

#include <string>

extern const long serialVersionUID;

extern const std::string TABLE_NAME;

extern const std::string COLUMN_NO;
extern const std::string COLUMN_ID;
//extern const std::string COLUMN_VERSION_CODE;
extern const std::string COLUMN_VERSION_CODE;
extern const std::string COLUMN_VERSION_NAME;
extern const std::string COLUMN_NAME;
extern const std::string COLUMN_FILESIZE;
//    extern const std::string COLUMN_INSTALLED;
extern const std::string COLUMN_DEVICE_TYPE;
extern const std::string COLUMN_DEVICE_SUB_TYPE;
extern const std::string COLUMN_MODEL_ID;
extern const std::string COLUMN_PLUGIN_PLATFORM_VERSION;
extern const std::string COLUMN_DEVICE_MANUFACTURER;
extern const std::string COLUMN_DEPENDENCIES;
//    extern const std::string COLUMN_TO_BE_DELETED;
extern const std::string COLUMN_CURRENT_STATUS;
extern const std::string COLUMN_LAST_CHECK_UPDATE;
extern const std::string COLUMN_TIMESTAMP;

extern const int STATUS_DOWNLOADED;
extern const int STATUS_INSTALLED;
extern const int STATUS_IS_UPDATE_AVAILABLE;
extern const int STATUS_TO_BE_DELETED;
extern const int STATUS_OUTDATED;

extern const std::string CREATE;

extern const std::string DROP;

#endif /* DB_CONSTANT_UIPLUGININFO_H_ */
