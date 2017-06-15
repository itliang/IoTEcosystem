// ICheckUpdateCB.aidl
package com.example.controller.PluginPlatform.callback;

// Declare any non-default types here with import statements

interface ICheckUpdateCB {

    oneway void onCheckUpdated(in List<PluginInfo> pluginInfoList);

    oneway void onFailure(in List<PluginInfo> pluginInfoList, in Constants errorCode);

}
