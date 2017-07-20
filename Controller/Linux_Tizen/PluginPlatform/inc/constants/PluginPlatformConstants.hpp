#include <string>



    /**
     * DEFAULT_PLUGIN_ACTIVITY_NAME
     */
      #define DEFAULT_PLUGIN_ACTIVITY_PREFIX     "com.samsung.android.plugin.";
    /**
     * Activity from where samsung connect will restart if crash occur due to plugin
     */
     #define PREVIOUS_ACTIVITY;
    /**
     * Sets this true when this is targeted to the Test server
     */
     #define LOCAL_TEST     true;
    /**
     * Sets this false for release build, to discard systrace log // TODO will be added from gradle by reza.r
     */

     #define ENABLE_TRACE     true;
    /**
     * Sets this true when this is targeted to the Test server
     */
     #define ALWAYS_DOWNLOAD     true;//BuildConfig.DEBUG;
    /**
     * Sets the flag to handle qc specific behavior of plugin platform
     */
     #define onQC     true;

    /**
     * Sets the flag to print class loading logs
     */
     #define CLASS_RES_LOGGING     true;//BuildConfig.DEBUG;
    /**
     * Test server url. This should be hide, need to find another way
     */
     #define TEST_DIRECTORY_PATH     "/sdcard/plugins/";
    /**
     * Directory for QA Mode
     */
     #define TEST_DIRECTORY_NAME     "gotoiot.plugin.test";
    /**
     * Default dex path prefix for a downloaded plugin
     */
     #define VMFPATH_PREFIX     "";//File.separator + "vmf" + File.separator + "apk";
    /**
     * Default optimizeddex path prefix for a loaded plugin, .so file does not use optimized dex path
     */
     #define OVMFPATH_PREFIX     "" ;//File.separator + "ovmf";

     #define PLUGIN_EXT     ".apk";

     #define RELEASE     1;
     #define INFO     2;
     #define DEBUG     3;

     #define PP_NULL_MODE     -1;
     #define PP_RELEASE_MODE     0;
     #define PP_DEBUG_MODE     1;

    /**
     * Debug level for the plugin platform, will be connected to the loggin system
     */

     #define DEBUG_LEVEL   DEBUG;
    /**
     * Plugin Platform Operation Mode
     * Production/Release Mode     0, QA/Debug Mode     1, Default Mode     -1
     */
     #define PLATFORM_OPERATION_MODE     PP_NULL_MODE;
