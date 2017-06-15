// IFindPluginCB.aidl
package com.example.controller.PluginPlatform.callback;

// Declare any non-default types here with import statements

interface IFindPluginCB {

    oneway void onSuccess(in List<PluginInfo> pluginInfoList, in Constants successCode);

    oneway void onFailure(in List<PluginInfo> pluginInfoList, in Constants errorCode);
}
