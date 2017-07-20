#ifndef _PLUG_SERVICE_INFO_H_
#define _PLUG_SERVICE_INFO_H_

#include <string>
#include <list>
#include "database/constant/PluginInfoTable.h"
#include "database/constant/PluginStatusCode.h"
#include "database/constant/PluginDataStatusCode.h"
using namespace std;

class PluginServiceInfo {
public:
	static PluginServiceInfo *getInstance();
	string getDataRootPath(){return dataRootPath;}

private:
	

   string dataRootPath = null;

   HostInfo *mHostInfo = null;
   int mPluginPlatformVersion = 0;
 //  DisplayMetrics mDisplayMetrics;
 //  Configuration mConfiguration;
};

#endif

