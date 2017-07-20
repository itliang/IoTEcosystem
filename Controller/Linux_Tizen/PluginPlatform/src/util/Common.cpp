/*
 * Common.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: liujuan
 */

#include <algorithm>
#include "util/Common.h"

Common::Common(void)
{

}
Common::~Common(void)
{

}

string Common::toLower(string res){
	transform(res.begin(), res.end(), res.begin(), ::tolower);
	return res;
}
