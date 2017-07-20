/*
 * FindPluginCallBack.h
 *
 *  Created on: Jun 14, 2017
 *      Author: liliang
 */

#ifndef CALLBACK_FINDPLUGINCALLBACK_H_
#define CALLBACK_FINDPLUGINCALLBACK_H_
#include <list>
#include "constants/SuccessCode.hpp"
#include "constants/ErrorCode.hpp"

using namespace std;
class FindPluginCallBack{

public:
	virtual void onFindSuccess(list<PluginInfo *> pluginInfoList, SuccessCode code)= 0;
	virtual void onFindFailure(list<PluginInfo *> pluginInfoList, ErrorCode error) = 0;	
};



#endif /* CALLBACK_FINDPLUGINCALLBACK_H_ */
