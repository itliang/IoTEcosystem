// ILaunchPluginCB.aidl
package com.example.controller.PluginPlatform.callback;

// Declare any non-default types here with import statements

interface ILaunchPluginCB {

    oneway void onSuccess(in PluginInfo pluginInfo, in Constants successCode);

    oneway void onFailure(in PluginInfo pluginInfo, in Constants errorCode);
}
