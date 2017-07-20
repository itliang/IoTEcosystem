/*
 * DatabaseHelper.h
 *
 *  Created on: Jun 19, 2017
 *      Author: liujuan
 */

#ifndef DATABASE_DATABASEHELPER_H_
#define DATABASE_DATABASEHELPER_H_

#include "constants/ErrorCode.hpp"

#define MAX_DB_RETRY_COUNT 20
#define DB_SLEEP_TIME  50000

class DatabaseHelper{
public:
	DatabaseHelper(void);
	virtual ~DatabaseHelper(void);

	ErrorCode openDb(sqlite3 ** database);

private:
	const char* getDbFile();
};


#endif /* DATABASE_DATABASEHELPER_H_ */
