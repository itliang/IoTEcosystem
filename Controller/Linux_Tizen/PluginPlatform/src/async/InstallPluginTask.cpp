#include "async/InstallPluginTask.h"
#include "galaxyapps/PluginInfoRequest.h"
#include "util/FileUtil.h"

#define DIR_SRC "/opt/usr/media/ios.apk"
#define DIR_DES "/opt/usr/media/ios"


void InstallPluginTask::processing()
{
	mState = RUNNING;
    mTryCount++;

    char *path =  DIR_DES;
    char *apkPath = DIR_SRC;

    return_e ret=FileUtil::unZipData(apkPath,path);

}


void InstallPluginTask::onSuccess(PluginInfo *pluginInfo, SuccessCode code)
{
	mState = FINISHED;
    //TBD
}

void InstallPluginTask::onProgress(PluginInfo *pluginInfo, StateCode stateCode, int current_status)
{
	// not used
}

void InstallPluginTask::onFailure(PluginInfo *pluginInfo, ErrorCode error)
{

}

void InstallPluginTask::request(PluginSchedulerRequestType type){
	// no need to use in this file
}
