// IDownloadPluginCB.aidl
package com.example.controller.PluginPlatform.callback;

// Declare any non-default types here with import statements

interface IDownloadPluginCB {

    oneway void onSuccess(in PluginInfo pluginInfo, in Constants successCode);

    oneway void onProgress(long downloaded); //downloaded file size

    oneway void onFailure(in PluginInfo pluginInfo, in Constants errorCode);
}
