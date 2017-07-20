/*
 * common.h
 *
 *  Created on: Jun 19, 2017
 *      Author: liujuan
 */

#ifndef UTIL_COMMON_H_
#define UTIL_COMMON_H_

#include <string>

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif

#ifndef SAFE_COPY
#define SAFE_COPY(x)	(x)!=NULL? strdup(x) : ""
#endif

using namespace std;

class Common{
public:
	Common(void);
	~Common(void);

	static string toLower(string res);
};

#endif /* UTIL_COMMON_H_ */
