#ifndef __PLUGIN_TASK_CALLBACK_H_
#define __PLUGIN_TASK_CALLBACK_H_

#include "PluginInfo.hpp"
#include "SuccessCode.hpp"
#include "ErrorCode.hpp"
#include "StateCode.hpp"

typedef enum {
	REQUEST_TYPE_PLUGIN_UPDATE,
	REQUEST_TYPE_SERVER_UPDATE
}PluginSchedulerRequestType;

class PluginTaskCallback {

public:
    virtual void onSuccess(PluginInfo *pluginInfo, SuccessCode code) = 0;
    virtual void onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status) = 0;
    virtual void onFailure(PluginInfo *pluginInfo, ErrorCode error) = 0;
    //virtual void request(PluginSchedulerRequestType type) = 0;
};
#endif

