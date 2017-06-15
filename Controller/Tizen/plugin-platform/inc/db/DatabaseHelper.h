/*
 * DatabaseHelper.h
 *
 *  Created on: Jun 13, 2017
 *      Author: liujuan
 */

#ifndef DB_DATABASEHELPER_H_
#define DB_DATABASEHELPER_H_

#include "sqlite3.h"
#include <string>
#include "constant/UIPluginInfo.h"

class DatabaseHelper
{
public:
	DatabaseHelper(std::string DB_NAME);
	virtual ~DatabaseHelper(void);

private:
	static const int DB_VERSION = 1;
//	static const char* createTable[1] = {
//	            "111"
//	    };
//	static string dropTable[] = {
//            UIPluginInfo.DROP,
//    };

};

#endif /* DB_DATABASEHELPER_H_ */
