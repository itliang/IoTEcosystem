#ifndef PLUGIN_TASK_RESPONSE_CALLBACK_H_
#define PLUGIN_TASK_RESPONSE_CALLBACK_H_

#include "PluginInfo.hpp"
#include "SuccessCode.hpp"
#include "ErrorCode.hpp"
#include "PluginTask.h"

class PluginTaskResponseCallback {

public:
    virtual void onSuccessTask(PluginTask *task, PluginInfo *pluginInfo, SuccessCode code) = 0;
    virtual void onFailureTask(PluginTask *task, PluginInfo *pluginInfo, ErrorCode error) = 0;
   // void findPluginLibrary(@NonNull PluginInfo pluginInfo, @NonNull final IPluginCallback callback);
};
#endif

