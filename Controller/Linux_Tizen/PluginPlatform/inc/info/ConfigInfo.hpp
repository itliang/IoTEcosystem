#ifndef _CONFIG_INFO_H_
#define _CONFIG_INFO_H_


#include "string"
#include "vconf.h"
#include "util/Log.h"
//#include "bincfg.h"

using namespace std;
#define CN_SERVER_BASE_URL "CN_SERVER_BASE_URL"

static int OPERATION_MODE ;
static int SCHEDULER_DELAY_TIME;
static string GLOBAL_HTTP_URL;
static string GLOBAL_HTTPS_URL;
static string CHINA_HTTP_DEFAULT_URL;
static string CHINA_HTTP_URL;
static string CHINA_HTTPS_URL;

class ConfigInfo {

private:
		//const static bool IS_DEBUGGABLE = false;//the attribute isn't exist in tizen
		const static bool IS_USER_BINARY = true;
		const static bool IS_SIGNATURE_MODE = true;

public:


		static void Init(){
			//int type = bincfg_get_binary_type();
			//if(type == BIN_TYPE_USER)
			//	IS_USER_BINARY = true;
			//else
			//	IS_USER_BINARY = false;
			OPERATION_MODE = 0;
			SCHEDULER_DELAY_TIME = 24*60*60*1000;
			GLOBAL_HTTP_URL = "http://vas.samsungapps.com/";
			GLOBAL_HTTPS_URL = "https://vas.samsungapps.com/";
			CHINA_HTTP_DEFAULT_URL = "http://cn-ms.samsungapps.com/";

			char *baseURL=vconf_get_str(CN_SERVER_BASE_URL);
			if(baseURL == NULL)
				WERROR("vconf get string failed");
			else
				WDEBUG("get base url %s",baseURL);
			string chinaBaseURL = baseURL;
			//WDEBUG("======>init get base url from vconf :%s",chinaBaseURL.c_str());
			if(chinaBaseURL.length()!=0 && !chinaBaseURL.empty()) {
	            CHINA_HTTP_URL = string("http://") + chinaBaseURL;
	            CHINA_HTTPS_URL = "https://" + chinaBaseURL;
	            if(CHINA_HTTP_URL.substr(CHINA_HTTP_URL.length()-1,1)!="/")
	                CHINA_HTTP_URL +="/";
	            if(CHINA_HTTPS_URL.substr(CHINA_HTTPS_URL.length()-1,1)!="/")
	                CHINA_HTTPS_URL +="/";
	        } else {
	            CHINA_HTTP_URL = "http://vas.samsungapps.com/";
	            CHINA_HTTPS_URL = "https://vas.samsungapps.com/";
	        }
			WDEBUG("CHINA HTTP URL IS %s",CHINA_HTTP_URL.c_str());
		}

		static void reloadCNServerBaseURL(string baseURL){
			if(baseURL.length()!=0 && !baseURL.empty()){
				CHINA_HTTP_URL = "http://" + baseURL;
				CHINA_HTTPS_URL = "https://" + baseURL;
				if(CHINA_HTTP_URL.substr(CHINA_HTTP_URL.length()-1,1)!="/")
					CHINA_HTTP_URL +="/";
				if(CHINA_HTTPS_URL.substr(CHINA_HTTPS_URL.length()-1,1)!="/")
					CHINA_HTTPS_URL +="/";
				WDEBUG("url is %s",CHINA_HTTP_URL.c_str());
				int ret = vconf_set_str(CN_SERVER_BASE_URL,baseURL.c_str());
				if(ret < 0)
					WERROR("set string error %d",ret);
			}
		}

		static bool isSignatureMode(){
			return IS_SIGNATURE_MODE;
		}

		static bool isReleaseMode(){
			return IS_USER_BINARY;
		}

		static int getSchedulerTime(){
			return SCHEDULER_DELAY_TIME;
		}

};

#endif
