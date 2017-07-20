/*
 * PluginInfoRequest.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: li liang
 */

#include "PluginInfoRequest.h"
#include "Log.h"
#include <memory.h>
#include <sstream>
#include "CHttpClient.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "HashHelper.h"


static PluginInfoRequest* _pInstance = NULL;

PluginInfoRequest* PluginInfoRequest::getInstance(){
	if(!_pInstance)
		_pInstance =  new PluginInfoRequest();
	return _pInstance;
}

PluginInfoRequest::PluginInfoRequest() 
{
	
	GLOBAL_HTTP_URL = "http://vas.samsungapps.com/";
	GLOBAL_HTTPS_URL = "https://vas.samsungapps.com/";
    CHINA_HTTP_DEFAULT_URL = "http://cn-ms.samsungapps.com/";

	mHttpRequest = new HttpRequest();
}

PluginInfoRequest::~PluginInfoRequest()
{
	delete mHttpRequest;
}

void PluginInfoRequest::request(PluginInfo *info,int msg,PluginTaskCallback *callback)
{

	WDEBUG("PluginInfoRequest::request :%d",msg);

	mRequestType = msg;
	mCallback = callback;
	
	mAppid = info->getId();
	mPluginInfo = info;
	
	switch (msg){
	case MSG::START_FIND:
		executeFind();
		break;
	case MSG::START_DOWNLOAD_URI:
		executeGetDownloadURL();
		break;
	case MSG::START_UPDATE:
		executeUpdate();
		break;
	default:
		WDEBUG("PluginInfoRequest::no operation !!!!!!!!!");
		break;
	}
}

void PluginInfoRequest::changeGalaxyAppsServer()
{
	WDEBUG("PluginInfoRequest::executeFind");
	//TBD
}

void PluginInfoRequest::executeFind()
{

	WDEBUG("PluginInfoRequest::executeFind");
	mUri = GLOBAL_HTTPS_URL 
		  + "earth/product/getScpAppList.as?deviceId=SCPLUGIN-IOS&mcc=000&mnc=00&csc=SKC&sdkVer=24&appId="
		  + mAppid.c_str()
		  + "&pluginPlatformVer=160286&pd=1";

	
	mHttpRequest->setPluginInfo(mPluginInfo);
	mHttpRequest->setUri(mUri);
	mHttpRequest->setRequestType(mRequestType);
	mHttpRequest->setResponseCallback(mCallback);
	mHttpRequest->request();

}

void PluginInfoRequest::executeUpdate()
{
	WDEBUG("PluginInfoRequest::executeUpdate");
	mUri = GLOBAL_HTTPS_URL 
		  + "stub/stubUpdateCheckEx.as?deviceId=SCPLUGIN-IOS&mcc=000&mnc=00&csc=SKC&sdkVer=24&appId="
		  + mAppid.c_str()
		  + "&pluginPlatformVer=160286&pd=1";

	
	mHttpRequest->setPluginInfo(mPluginInfo);
	mHttpRequest->setUri(mUri);
	mHttpRequest->setRequestType(mRequestType);
	mHttpRequest->setResponseCallback(mCallback);
	mHttpRequest->request();
}

void PluginInfoRequest::executeGetDownloadURL()
{
	//get time	
	time_t rawTime;	
	struct tm* timeInfo;	
	char szTemp[30]={0};	
	time(&rawTime);	
	timeInfo = gmtime(&rawTime);
	strftime(szTemp,sizeof(szTemp),"%Y%m%d%H%M%S",timeInfo);
	char *hashValue = getHashValue((char *)szTemp,"com.samsung.ios.plugin.stplugin");
	WDEBUG("hash value is :%s",hashValue);

	mUri = GLOBAL_HTTPS_URL+ "stub/stubDownloadEx.as?appId=" 
	+ mAppid
	+ "&encImei=JBFUEv61j6MbBEwLrVl5aQ%253D%253D%250A&deviceId=SCPLUGIN-IOS&mcc=000&mnc=00&csc=ETC&sdkVer=24&pluginPlatformVer="
	+ mPluginInfo->getPlatformVersion()
	+ "&hashValue="
	+ hashValue+ "&pd=1&srcType=SCP&reqTime="
	+ szTemp;

	WDEBUG("Uri value is %s",mUri.c_str());

	mHttpRequest->setPluginInfo(mPluginInfo);
	mHttpRequest->setUri(mUri);
	mHttpRequest->setRequestType(mRequestType);
	mHttpRequest->setResponseCallback(mCallback);
	mHttpRequest->request();
	
	
	
}

void PluginInfoRequest::makeFindUrl(string *url){

}

void PluginInfoRequest::makeUpdateUrl(string *url){

}

void PluginInfoRequest::makeGetAddressUrl(string *url){

}

