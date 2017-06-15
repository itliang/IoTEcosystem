/*
 * DownloadPluginCallBack.h
 *
 *  Created on: Jun 14, 2017
 *      Author: liliang
 */

#ifndef CALLBACK_DOWNLOADPLUGINCALLBACK_H_
#define CALLBACK_DOWNLOADPLUGINCALLBACK_H_

#include "PluginInfo.hpp"
#include "SuccessCode.hpp"
#include "ErrorCode.hpp"

public DownloadPluginCallback {

public:
    /**
     * This is invoked when the download of the given plugin is successfully finished.
     * <p><ul>
     * <li>SuccessCode.PLUGIN_ALREADY_DOWNLOADED: When the plugin is already downloaded
     * <li>SuccessCode.PLUGIN_DOWNLOADED: When the plugin is successfully downloaded
     * </ul></p>
     * @param pluginInfo the information of the successfully downloaded plugin
     * @param code the success code indicating different success situations of downloadPlugin.
     */
    virtual void onSuccess(PluginInfo pluginInfo, SuccessCode code)= 0;

    /**
     * This is invoked every 'period' milliseconds, which is given at the download request, to let the application know the download progress.
     * @param downloaded the downloaded file size.
     */
    virtual void onProgress(long downloaded) = 0;

    /**
     * This is invoked when the download of the given plugin is failed. This returns the error code corresponding to the failure type.
     * <p><ul>
     * <li>ErrorCode.INVALID_PARAM: When the list is empty or null
     * <li>ErrorCode.BGSERVICE_NOT_BOUND: When the plugin background service is not bound
     * <li>ErrorCode.NOT_ENOUGH_SPACE: When there is not enough space to download the plugin
     * <li>ErrorCode.PLUGIN_NOT_AVAILABLE: When the plugin is not available at the given url
     * </ul></p>
     * @param pluginInfo the information of the failed plugin download
     * @param errorCode the error code corresponding to the download failure
     */
    virtual void onFailure(PluginInfo pluginInfo, ErrorCode errorCode) = 0;
}



#endif /* CALLBACK_DOWNLOADPLUGINCALLBACK_H_ */
