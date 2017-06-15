/*
 * FindPluginCallBack.h
 *
 *  Created on: Jun 14, 2017
 *      Author: liliang
 */

#ifndef CALLBACK_FINDPLUGINCALLBACK_H_
#define CALLBACK_FINDPLUGINCALLBACK_H_
#include <list>
#include "SuccessCode.hpp"
#include "ErrorCode.hpp"

using namespace std;
class FindPluginCallBack{

public:
	virtual void onSuccess(list<PluginInfo *> pluginInfoList, SuccessCode code)= 0;
	virtual void onFailure(list<PluginInfo *> pluginInfoList, ErrorCode error) = 0;	
}



#endif /* CALLBACK_FINDPLUGINCALLBACK_H_ */
