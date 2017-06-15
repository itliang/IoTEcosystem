// IPluginBgService.aidl
package com.example.controller.PluginPlatform;
package com.example.controller.PluginPlatform.callback;

// Declare any non-default types here with import statements

interface IPluginBgService {

    oneway void findPlugin(in PluginInfo filter, in IFindPluginCB callback);

    oneway void downloadPlugin(in PluginInfo pluginInfo, in IDownloadPluginCB callback, long period);

    oneway void installPlugin(in PluginInfo pluginInfo, in IInstallPluginCB callback);

    oneway void checkUpdate(in List<PluginInfo> pluginInfos, in ICheckUpdateCB callback);

    boolean deletePlugin(in PluginInfo pluginInfo);

    oneway void clearDownloadQueue();
    oneway void cancelDownloadRequest();

    //oneway void setServiceListener(in IPluginBgServiceListener listener);

    List<PluginInfo> getAllPlugins();
}
