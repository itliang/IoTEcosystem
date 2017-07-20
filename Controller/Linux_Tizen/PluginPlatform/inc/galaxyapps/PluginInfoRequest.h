/*
 * PluginDownloadHandler.h
 *
 *  Created on: Jun 13, 2017
 *      Author: zoulianghui
 */

#ifndef GALAXYAPPS_HANDLER_PLUGININFOREQUESTHANDLER_H_
#define GALAXYAPPS_HANDLER_PLUGININFOREQUESTHANDLER_H_

#include "GHandler.h"
#include "Handler.h"
#include "Mutex.h"
#include <stdio.h>
#include "Thread.h"
#include <string>
#include <queue>
#include <curl/curl.h>
#include "MRFileDownloader.h"
#include "GalaxyAppsRequest.h"
#include "PluginInfo.hpp"
#include "HostInfo.hpp"
#include "PluginTaskCallback.h"

using namespace std;



class PluginInfoRequest {

private:
	http_session_h mSession;
	http_transaction_h mTransaction;
	PluginInfo *mPluginInfo = NULL;
	HostInfo *mHostInfo = NULL;
	string mAppid;
	string GLOBAL_HTTP_URL ;
	string GLOBAL_HTTPS_URL;
	string CHINA_HTTP_DEFAULT_URL;
	string mUri;

	HttpRequest *mHttpRequest;

public:
	PluginInfoRequest();
	virtual ~PluginInfoRequest();
	void request(PluginInfo *info,int msg, PluginTaskCallback *callback);
	void changeGalaxyAppsServer();
	static PluginInfoRequest* getInstance();

	int mRequestType ;
	PluginTaskCallback *mCallback;

private:
	void registerHttpCB();
	void sendHttpRequest(char *uri);
	void executeFind();
	void executeGetDownloadURL();
	void executeUpdate();
	void makeFindUrl(string *findUrl);
	void makeUpdateUrl(string *updateUrl);
	void makeGetAddressUrl(string *getUrl);
};

#endif /* GALAXYAPPS_HANDLER_PLUGININFOREQUESTHANDLER_H_ */
