// IInstallPluginCB.aidl
package com.example.controller.PluginPlatform.callback;

// Declare any non-default types here with import statements

interface IInstallPluginCB {

    oneway void onSuccess(in PluginInfo pluginInfo, in Constants successCode);

    oneway void onProgress(in StateCode state, in PluginInfo pluginInfo);

    oneway void onFailure(in PluginInfo pluginInfo, in Constants errorCode);
}
