#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>


#ifdef __cplusplus
}
#endif

#include "PluginPlatformService.h"

int main(int argc, char *argv[])
{
	g_thread_init(NULL);
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_print("g_main_loop_run\n");
	
	g_main_loop_run(loop);	
	//To add my logic here ?
	PluginPlatformService *pluginPFSvc = new PluginPlatformService();	
	PluginInfo *info = new PluginInfo();
	info->setId("com.samsung.ios.plugin.stplugin");
	pluginPFSvc->findPluginInfo(info);

	g_print("g_main_loop_unref\n");
    g_main_loop_unref(loop);
    return 0;

}





