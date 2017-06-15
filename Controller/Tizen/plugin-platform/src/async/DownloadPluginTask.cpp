/*
 * DownloadPluginTask.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: liliang
 */

#include 	<iostream>
#include	"DownloadPluginTask.h"


DownloadPluginTask::DownloadPluginTask(PluginInfo *info)
{
	pPluginInfo = info;
}

void DownloadPluginTask::startDownload(){
	pThread = ecore_thread_feedback_run(downloadThreadCB, downloadThreadNotifyCB, downloadEndThreadCB, downloadCancelThreadCB, this, EINA_FALSE);

}


void DownloadPluginTask::downloadThreadCB(void *data, Ecore_Thread *thread){
	DownloadPluginTask *pThis = (DownloadPluginTask *)data;
	//pThis->downloadPlugin();

}

void DownloadPluginTask::downloadCancelThreadCB(void *data, Ecore_Thread *thread){

}

void DownloadPluginTask::downloadEndThreadCB(void *data, Ecore_Thread *thread){

}
	
void DownloadPluginTask::downloadThreadNotifyCB(void *data, Ecore_Thread *thread, void *msgData){

}

/**
     * Starts downloading the plugin with a given plugin information.
     * When the download is finished, it invokes the onSuccess of callback implementation.
     * @param pluginInfo the plugin information corresponding to the one to be downloaded from the server {@see com.samsung.android.oneapp.pluginmgmt.info.PluginInfo}
     * @param callback the implementation of DownloadPluginCallback interface {@see com.samsung.android.oneapp.pluginmgmt.callback.DownloadPluginCallback}
     * @param period the period to report the download progress as a callback, (in milliseconds)
     */
void DownloadPluginTask::downloadPlugin(final DownloadPluginCallback callback, final long period){   
    //PPLog.d(TAG, "downloadPlugin", pluginInfo->getName() + ", " + pluginInfo->getDeviceType());
    if ((pPluginInfo->getVersionName() != null) && (pPluginInfo->getUrl() == null || pPluginInfo->getUrl().length()==0) || (pPluginInfo->getId() != null && pluginInfo->getId().length() == 0)) {
        findPlugin(pPluginInfo, new FindPluginCallBack() {
            @Override
            public void onSuccess(list<PluginInfo *> pluginInfoList, SuccessCode code) {
                if(code==SuccessCode.PLUGIN_ALREADY_DOWNLOADED){
					callback.onSuccess(pluginInfo, code);
				}
                else{
                    for (PluginInfo *pi : pluginInfoList) {
                        downloadPlugin(pi, callback, period);
                    }
				}
            }

            @Override
            public void onFailure(List<PluginInfo> pluginInfoList, ErrorCode error) {
                callback.onFailure(pluginInfo, error);
            }
        }, false);
    }
	else if (pluginInfo->getUrl() != null && pluginInfo->getUrl().startsWith("http")) {
        if(pluginInfo->getUrl().length()<=0){
            callback.onFailure(pluginInfo, ErrorCode.INVALID_PARAM);
            return;
        }
        RequestFile.getInstance(mContext).setPeriod(period);
        if (downloadPluginCallbacks.containsKey(pluginInfo.getKey())) {
            PPLog.i(TAG, "downloadPlugin", "duplicate download request, merged into the previous download request");
            downloadPluginCallbacks.get(pluginInfo.getKey()).add(callback);
        } else {//when there is no previous download request for the same plugin
            LinkedList<DownloadPluginCallback> list = new LinkedList<DownloadPluginCallback>();
            list.add(callback);
            downloadPluginCallbacks.put(pluginInfo.getKey(), list);
            downloadList.add(pluginInfo);
            downloadPlugin(pluginInfo);
        }
    } else {
        if (pluginInfo.getUrl() == null || pluginInfo.getUrl().length()==0) {
            callback.onFailure(pluginInfo, ErrorCode.INVALID_PARAM);
            return;
        }
		#if 0
        PPLog.d(TAG, "downloadPlugin", pluginInfo.getUrl());
        File file = new File(PluginPlatformConstants.TEST_DIRECTORY_PATH + pluginInfo.getUrl());
        if (PluginPlatformConstants.LOCAL_TEST && file.exists()) {
            try {
                copyPlugin(pluginInfo, file);
                UIPluginDBManager pluginDbManager = UIPluginDBManager.getInstance(mContext);
                if(pluginDbManager!=null){
                    if(!pluginDbManager.contains(pluginInfo, UIPluginInfo.STATUS_INSTALLED)) {
                        pluginDbManager.storeUIPluginInfo(pluginInfo);
                        pluginDbManager.updateCurrentStatus(pluginInfo, UIPluginInfo.STATUS_DOWNLOADED);
                    }
                }
                PPLog.d(TAG, "downloadPlugin", pluginInfo.getDeviceType());
                callback.onSuccess(pluginInfo, SuccessCode.PLUGIN_DOWNLOADED);
            } catch (IOException e) {
                if (e.getMessage().contains("There is not enough space on the disk")) {
                    callback.onFailure(pluginInfo, ErrorCode.NOT_ENOUGH_SPACE);
                }
            }
        } else {
            callback.onFailure(pluginInfo, ErrorCode.PLUGIN_NOT_AVAILABLE);
        }
		#endif
    }
    //if (PluginPlatformConstants.ENABLE_TRACE) {
    //    Trace.endSection();
    //}
}

void DownloadPluginTask::findPlugin(PluginInfo *filter, final FindPluginCallBack callback, boolean checkLocal){
        //local check
        if(checkLocal) {
            list<PluginInfo *> pluginInfoList = UIPluginDBManager.getInstance(mContext).getUIPluginInfoList(filter, UIPluginInfo.STATUS_INSTALLED);
            if (pluginInfoList != null && pluginInfoList.size() > 0) {
                callback.onSuccess(pluginInfoList, SuccessCode.PLUGIN_ALREADY_INSTALLED);
                return;
            }
            if (filter.getName() == null && (filter.getDeviceType() != null && filter.getDeviceType().length() > 0)) {
                pluginInfoList = UIPluginDBManager.getInstance(mContext).getUIPluginInfoList(filter, UIPluginInfo.STATUS_IS_UPDATE_AVAILABLE);
                if (pluginInfoList != null && pluginInfoList.size() > 0) {
                    callback.onSuccess(pluginInfoList, SuccessCode.PLUGIN_UPDATE_FOUND);
                    return;
                }
                pluginInfoList = UIPluginDBManager.getInstance(mContext).getUIPluginInfoList(filter, UIPluginInfo.STATUS_OUTDATED);
                if (pluginInfoList != null && pluginInfoList.size() > 0) {
                    callback.onSuccess(pluginInfoList, SuccessCode.PLUGIN_OUTDATED);
                    return;
                }
            } else if (filter.getName() == null && (filter.getId() != null && filter.getId().length() > 0)) {
                pluginInfoList = UIPluginDBManager.getInstance(mContext).getUIPluginInfoList(filter, UIPluginInfo.STATUS_IS_UPDATE_AVAILABLE);
                if (pluginInfoList != null && pluginInfoList.size() > 0) {
                    callback.onSuccess(pluginInfoList, SuccessCode.PLUGIN_UPDATE_FOUND);
                    return;
                }
                pluginInfoList = UIPluginDBManager.getInstance(mContext).getUIPluginInfoList(filter, UIPluginInfo.STATUS_OUTDATED);
                if (pluginInfoList != null && pluginInfoList.size() > 0) {
                    callback.onSuccess(pluginInfoList, SuccessCode.PLUGIN_OUTDATED);
                    return;
                }
            }
        }

        //if (PluginPlatformConstants.ENABLE_TRACE) {
        //    Trace.beginSection("PluginPlatform:PluginBgService:findPluginRequest");
        //}
        //create differernt combination key
        PPLog.d(TAG, "findPlugin", "discover metadata of " + filter.getDeviceType()+" & "+filter.getDeviceSubType());
#if 0
		if (PluginPlatformConstants.LOCAL_TEST && testPlugins!=null && contains(testPlugins, filter)) {
            PPLog.d(TAG, "findPlugin", "discover metadata of " + filter.getDeviceType() + " / " + filter.getDeviceSubType() +" from local");
            //TODO: for referring the dependency among plugins, we need to use the innerfindplugincallback for both local and server logics
            InnerFindPluginCallback innerCallback = new InnerFindPluginCallback(callback);
            for (PluginInfo pluginInfo : testPlugins.values()) {
                if (equals(pluginInfo, filter)) {
                    innerCallback.nPlugins++;
                    pluginInfo.setReqTime(getCurrentTimeString());
                    if (pluginInfo.getDependencies().size() == 0) {
                        innerCallback.add(pluginInfo);
                    } else {
                        DependencyFindPluginCallback handler = new DependencyFindPluginCallback(pluginInfo, innerCallback);
                        for (PluginInfo lib : pluginInfo.getDependencies()) {
                            findPlugin(lib, handler, checkLocal);
                        }
                    }
                }
            }
            if(innerCallback.nPlugins==innerCallback.ret.size()&callback!=null)callback.onSuccess(innerCallback.ret, SuccessCode.PLUGIN_METADATA_FOUND);
        } else {
#endif
            findPluginFromServer(filter, callback);
#if 0
        }
        if (PluginPlatformConstants.ENABLE_TRACE) {
            Trace.endSection();
        }
#endif
    }

