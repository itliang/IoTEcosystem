#include <time.h>
#include <openssl/md5.h>
#include "Util.h"
#include "JDHttpRequest.h"

JDHttpRequest::JDHttpRequest()
{
	mBaseUri = "https://smartopen.jd.com/routerjson";
	mOAuthUri = "https://smartopen.jd.com/oauth/token";
	mRedirctUri = "http://www.example.com/oauth2callback";
    mMethodDeviceList = "jingdong.smart.api.device.list";
    mMethodDeviceInfo = "jingdong.smart.api.snapshot.get";
    mMethodDeviceControl = "jingdong.smart.api.control";
    mVerison = "2.0";

    mDevList = new std::list<Device *>;
    mIsSignIn = false;

    mTimer = evtimer_new(g_event_base, timeOutCallback, (void *)this);
}

JDHttpRequest::~JDHttpRequest()
{
    if (mTimer) {
        evtimer_del(mTimer);
        mTimer = nullptr;
    }

    if (mDevList) {
        std::list<Device *>::iterator it;
        for (it = mDevList->begin(); it != mDevList->end(); it++) {
            Device *dev = (Device *)(*it);

            if (dev) {
                delete dev;
                dev = nullptr;
            }
        }

        delete mDevList;
        mDevList = nullptr;
    }
}

void JDHttpRequest::timeOutCallback(int fd, short event, void *userdata)
{
    printf("it's time to refresh the access token\n");
    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)userdata;

    RSCode ret = pJDHttpRequest->refreshAccessToken();
    if (ret != RS_OK) {
        fprintf(stderr, "failed to refresh token\n");
    }
}

size_t JDHttpRequest::signInCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    int bytesSize = size * nmemb;

    if (!ptr || bytesSize <= 0) {
        fprintf(stderr, "invalid body is received\n");
        return 0;
    }
    
    if (!userdata) {
        fprintf(stderr, "userdata is invalid\n");
        return 0;
    }

    printf("\nrecv: %s\nlen: %d\n", ptr, bytesSize);

    char *bodyMsg = (char *)calloc(1, bytesSize+1);
    if (!bodyMsg) {
        fprintf(stderr, "failed to calloc memory\n");
        return 0;
    }

    memcpy(bodyMsg, ptr, bytesSize);

    Json::Value root;
    Json::Reader *pJsonParser = new Json::Reader(Json::Features::strictMode());

    if (!pJsonParser->parse(bodyMsg, root)) {
        fprintf(stderr, "failed to parse json, because: %s\n", pJsonParser->getFormattedErrorMessages().c_str());
        free(pJsonParser);
        free(bodyMsg);
        return 0;
    }

    if (root.isMember("error_response")) {
        Json::Value error_response = root["error_response"];

        if (error_response["en_desc"].isString()) {
            std::string en_desc = error_response["en_desc"].asString();
            fprintf(stderr, "failed to sign in, because: %s\n", en_desc.c_str());
        }

        free(pJsonParser);
        free(bodyMsg);
        return 0;
    }

    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)userdata;

    if (root["access_token"].isString()) {
        pJDHttpRequest->mAccessToken = root["access_token"].asString();
    }

    if (root["refresh_token"].isString()) {
        pJDHttpRequest->mRefreshToken = root["refresh_token"].asString();
    }

    long expired_time = (long)root["expires_in"].asDouble();

    if (pJDHttpRequest->mAccessToken.empty() ||
        pJDHttpRequest->mRefreshToken.empty()) {
        fprintf(stderr, "invalid token\n");
        free(pJsonParser);
        free(bodyMsg);
        return 0;
    }

	printf("access token: %s, refresh token: %s, expired time:%ld\n", 
		pJDHttpRequest->mAccessToken.c_str(), 
		pJDHttpRequest->mRefreshToken.c_str(),
        expired_time);

	pJDHttpRequest->mIsSignIn = true;

    // start a timer to refresh token
    pJDHttpRequest->mTimeval.tv_sec = expired_time;
    pJDHttpRequest->mTimeval.tv_usec = 0;

    evtimer_add(pJDHttpRequest->mTimer, &pJDHttpRequest->mTimeval);

    free(pJsonParser);
    free(bodyMsg);
	return bytesSize;
}

size_t JDHttpRequest::setDeviceInfoCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    int bytesSize = size * nmemb;

    if (!ptr || bytesSize <= 0) {
        fprintf(stderr, "invalid body is received\n");
        return 0;
    }
    
    if (!userdata) {
        fprintf(stderr, "userdata is invalid\n");
        return 0;
    }

    printf("\nrecv: %s\nlen: %d\n", ptr, bytesSize);

    char *bodyMsg = (char *)calloc(1, bytesSize+1);
    if (!bodyMsg) {
        fprintf(stderr, "failed to calloc memory\n");
        return 0;
    }

    memcpy(bodyMsg, ptr, bytesSize);

    Json::Value root;
    Json::Reader *pJsonParser = new Json::Reader(Json::Features::strictMode());

    if (!pJsonParser->parse(bodyMsg, root)) {
        fprintf(stderr, "failed to parse json, because: %s\n", pJsonParser->getFormattedErrorMessages().c_str());
        free(pJsonParser);
        free(bodyMsg);
        return 0;
    }

    std::string strResult;
    Json::Value resp = root["jingdong_smart_api_snapshot_get_response"];

    if (resp["result"].isString()) {
        strResult = resp["result"].asString();
    }

    Json::Value result;

    if (!strResult.empty() && pJsonParser->parse(strResult, result)) {
        std::string code;
        if (resp["result"].isString()) {
            code = result["code"].asString();
        }

        if (code.empty() || code != "200") {
            fprintf(stderr, "failed to get device snapshot\n");
            free(pJsonParser);
            free(bodyMsg);
            return 0;
        }

        Device *ocDevice = (Device *)userdata;
        Json::Value streams = result["data"]["streams"];

        for (int i = 0; i < streams.size(); i++) {
            Json::Value stream = streams[i];
            std::string streamId, status;

            if (stream["stream_id"].isString()) {
                streamId = stream["stream_id"].asString();
            }

            if (stream["current_value"].isString()) {
                status = stream["current_value"].asString();

                //avoid empty info for app side
                if (status.empty()) {
                    status = "0";
                }
            }

            if (!streamId.empty()) {
                list<SupportedCommand *> *cmds = ocDevice->getSupportedCommands();
                std::list<SupportedCommand *>::iterator it;
                bool isNew = true;

                for (it = cmds->begin(); it != cmds->end(); it++) {
                    SupportedCommand *cmd = (SupportedCommand *)(*it);
                    if (cmd->getID() == streamId) {
                        cmd->setStatus(status);
                        isNew = false;
                        printf("update device snapshot: [%s][%s]\n", streamId.c_str(), status.c_str());
                    }
                }

                if (isNew) {
                    SupportedCommand *cmd = new SupportedCommand(streamId, status);
                    cmds->push_back(cmd);
                    printf("add device snapshot: [%s][%s]\n", streamId.c_str(), status.c_str());
                }
            }
        }
    }

    free(pJsonParser);
    free(bodyMsg);
    return bytesSize;
}

size_t JDHttpRequest::getDeviceListCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    int bytesSize = size * nmemb;

    if (!ptr || bytesSize <= 0) {
        fprintf(stderr, "invalid body is received\n");
        return 0;
    }
    
    if (!userdata) {
        fprintf(stderr, "userdata is invalid\n");
        return 0;
    }

    printf("\nrecv: %s\nlen: %d\n", ptr, bytesSize);

    char *bodyMsg = (char *)calloc(1, bytesSize+1);
    if (!bodyMsg) {
        fprintf(stderr, "failed to calloc memory\n");
        return 0;
    }

    memcpy(bodyMsg, ptr, bytesSize);

    Json::Value root;
    Json::Reader *pJsonParser = new Json::Reader(Json::Features::strictMode());

    if (!pJsonParser->parse(bodyMsg, root)) {
        fprintf(stderr, "failed to parse json, because: %s\n", pJsonParser->getFormattedErrorMessages().c_str());
        free(pJsonParser);
        free(bodyMsg);
        return 0;
    }

    std::string strResult;
    Json::Value resp = root["jingdong_smart_api_device_list_response"];

    if (resp["result"].isString()) {
        strResult = resp["result"].asString();
    }

    Json::Value result;
    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)userdata;

    if (!strResult.empty() && pJsonParser->parse(strResult, result)) {
        std::string code;
        if (resp["result"].isString()) {
            code = result["code"].asString();
        }

        if (code.empty() || code != "200") {
            fprintf(stderr, "failed to get device list\n");
            free(pJsonParser);
            free(bodyMsg);
            return 0;
        }

        Json::Value data = result["data"];

        for (int i = 0; i < data.size(); i++) {
            int count = 0;

            if (data[i]["count"].isInt()) {
                count = data[i]["count"].asInt();
            }
            
            Json::Value dev_list = data[i]["list"];

            for (int j = 0; j < count; j++) {
                Json::Value dev = dev_list[j];
                std::string deviceName, id, productUUID, status;

                if (dev["device_name"].isString()) {
                    deviceName = dev["device_name"].asString();
                }

                if (dev["id"].isString()) {
                    id = dev["id"].asString();
                }

                if (dev["product_uuid"].isString()) {
                    productUUID = dev["product_uuid"].asString();
                }

                if (dev["status"].isString()) {
                    status = dev["status"].asString();
                }

                if (!id.empty() && !productUUID.empty() && !status.empty()) {
                    Device *ocDevice = new Device(id, deviceName);
                    ocDevice->setProductUUID(productUUID);
                    ocDevice->setStatus(status);
                    pJDHttpRequest->mDevList->push_back(ocDevice);
                    // printf("added device[%s, %s, %s]\n", id.c_str(), deviceName.c_str(), status.c_str());
                }
            }
        }
    }

    free(pJsonParser);
    free(bodyMsg);
    return bytesSize;
}

size_t JDHttpRequest::controlDeviceCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    int bytesSize = size * nmemb;

    if (!ptr || bytesSize <= 0) {
        fprintf(stderr, "invalid body is received\n");
        return 0;
    }
    
    if (!userdata) {
        fprintf(stderr, "userdata is invalid\n");
        return 0;
    }

    printf("\nrecv: %s\nlen: %d\n", ptr, bytesSize);

    char *bodyMsg = (char *)calloc(1, bytesSize+1);
    if (!bodyMsg) {
        fprintf(stderr, "failed to calloc memory\n");
        return 0;
    }

    memcpy(bodyMsg, ptr, bytesSize);

    Json::Value root;
    Json::Reader *pJsonParser = new Json::Reader(Json::Features::strictMode());

    if (!pJsonParser->parse(bodyMsg, root)) {
        fprintf(stderr, "failed to parse json, because: %s\n", pJsonParser->getFormattedErrorMessages().c_str());
        free(pJsonParser);
        free(bodyMsg);
        return 0;
    }

    std::string strResult;
    Json::Value resp = root["jingdong_smart_api_control_response"];

    if (resp["result"].isString()) {
        strResult = resp["result"].asString();
    }

    Json::Value result;

    if (pJsonParser->parse(strResult, result)) {
        std::string code;

        if (result["code"].isString()) {
            code = result["code"].asString();
        }

        if (code.empty() || code != "200") {
            fprintf(stderr, "failed to control device\n");
            free(pJsonParser);
            free(bodyMsg);
            return 0;
        }
    }

    free(pJsonParser);
    free(bodyMsg);
    return bytesSize;
}

void JDHttpRequest::getSignature(char **signature, char **timestamp, const char *method, const char *param)
{
    //get timestamp
    char format_time[64] = {0};
    time_t tt = time(NULL);
    tm* t = localtime(&tt);
    
    snprintf(format_time, 63, "%d-%02d-%02d %02d:%02d:%02d", 
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        t->tm_hour,
        t->tm_min,
        t->tm_sec
    );

    printf("timestamp: %s\n", format_time);

    //append all parameters in order
    char buffer[2048] = {0};
    snprintf(buffer, 2047, "%s%saccess_token%sapp_key%smethod%stimestamp%sv%s%s", 
        mAppSecret.c_str(),
        param,
        mAccessToken.c_str(),
        mAppKey.c_str(),
        method,
        format_time,
        mVerison.c_str(),
        mAppSecret.c_str());

    printf("sign before md5: %s\n", buffer);

    //set encryption
    MD5_CTX md5_ctx;
    unsigned char signature_hash[16] = {0};
    std::string tmp(buffer);

    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, tmp.c_str(), tmp.length());
    MD5_Final(signature_hash, &md5_ctx);

    char digest[33] = {0};

    for (int i = 0; i < 16; i++)
        sprintf(&digest[i*2], "%02X", (unsigned int)signature_hash[i]);

    printf("[uppered]md5 digest: %s\n", digest);

    *signature = g_strdup(digest);
    *timestamp = g_strdup(format_time);
}

RSCode JDHttpRequest::refreshAccessToken()
{
    if (!mIsSignIn) {
        fprintf(stderr, "must sign in at first\n");
        return RS_SIGN_IN_ERROR;
    }

    char buffer[512] = {0};
    snprintf(buffer, 511, "%s?grant_type=refresh_token&client_id=%s&refresh_token=%s&client_secret=%s&state=app", 
        mOAuthUri.c_str(), 
        mAppKey.c_str(),
        mRefreshToken.c_str(),
        mAppSecret.c_str());

    std::string uri(buffer);
    std::string postData;
    HeadersMap headers;

    int ret = mHttpClient->post(uri, postData, headers, signInCallback, (void *)this);
    if (ret != 0) {
        fprintf(stderr, "network's error\n");
        return RS_NETWORK_ERROR;
    }

    return RS_OK;    
}

RSCode JDHttpRequest::signIn(const ParamsMap &params, Auth_Type type)
{
	if (params.size() != 3) {
		fprintf(stderr, "the count of parameters is wrong\n");
		return RS_INVALID_PARAMETER;
	}

	std::string authCode, appKey, appSecret;
	std::map<std::string, std::string>::const_iterator iter;

	iter = params.find("AuthCode");
    if (params.end() != iter) {   
        authCode = iter->second;
    }

	iter = params.find("AppKey");
    if (params.end() != iter) {   
        appKey = iter->second;
    }

	iter = params.find("AppSecret");
    if (params.end() != iter) {   
        appSecret = iter->second;
    }

    if (0 == authCode.length() ||
        0 == appKey.length() ||
        0 == appSecret.length()) {
        fprintf(stderr, "invalid parameters\n");
        return RS_INVALID_PARAMETER;
    }

	char buffer[512] = {0};
    snprintf(buffer, 511, "%s?grant_type=authorization_code&client_id=%s&redirect_uri=%s&code=%s&client_secret=%s&state=app", 
        mOAuthUri.c_str(), 
    	appKey.c_str(),
    	mRedirctUri.c_str(),
        authCode.c_str(),
        appSecret.c_str());

    std::string uri(buffer);
    std::string postData;
    HeadersMap headers;

    int ret = mHttpClient->post(uri, postData, headers, signInCallback, (void *)this);
    if (ret != 0) {
		fprintf(stderr, "network's error\n");
		return RS_NETWORK_ERROR;
    }

    mAppKey = appKey;
    mAppSecret = appSecret;

    return RS_OK;
}

std::list<Device *>* JDHttpRequest::getDeviceList()
{
	if (!mIsSignIn) {
		fprintf(stderr, "must sign in at first\n");
		return nullptr;
	}

    char *signature = nullptr;
    char *timestamp = nullptr;
    const char *data = "360buy_param_json{}";

    getSignature(&signature, &timestamp, mMethodDeviceList.c_str(), data);
    if (!signature || !timestamp) {
        fprintf(stderr, "failed to get signature\n");
        return nullptr;
    }

    ParamsMap uriParams;
    uriParams.insert(std::make_pair("sign", std::string(signature)));
    uriParams.insert(std::make_pair("timestamp", std::string(timestamp)));
    uriParams.insert(std::make_pair("v", mVerison));
    uriParams.insert(std::make_pair("app_key", mAppKey));
    uriParams.insert(std::make_pair("method", mMethodDeviceList));
    uriParams.insert(std::make_pair("access_token", mAccessToken));

    g_free(signature);
    g_free(timestamp);

    std::string postData(data);
    HeadersMap headers;
    postData.insert(17, "=");

    int ret = mHttpClient->post(mBaseUri, uriParams, postData, headers, getDeviceListCallback, (void *)this);
    if (ret != 0) {
		fprintf(stderr, "network's error\n");
		return nullptr;
    }

    //set device snapshot in advanced
    if (mDevList) {
        std::list<Device *>::iterator it;
        for (it = mDevList->begin(); it != mDevList->end(); it++) {
            Device *dev = (Device *)(*it);
            printf("set the snapshot of device[%s]\n", dev->getID().c_str());

            setDeviceInfo(dev);
        }
    }

    return mDevList;
}

RSCode JDHttpRequest::setDeviceInfo(Device *dev)
{
    if (!dev) {
        fprintf(stderr, "invalid parameter\n");
        return RS_INVALID_PARAMETER;
    }

    char *signature = nullptr;
    char *timestamp = nullptr;
    char data[256] = {0};

    snprintf(data, 255, "360buy_param_json{\"id\":\"%s\", \"pull_mode\":0}", dev->getID().c_str());

    getSignature(&signature, &timestamp, mMethodDeviceInfo.c_str(), (const char *)data);
    if (!signature || !timestamp) {
        fprintf(stderr, "failed to get signature\n");
        return RS_GET_SIGNATURE_ERROR;
    }

    ParamsMap uriParams;
    uriParams.insert(std::make_pair("sign", std::string(signature)));
    uriParams.insert(std::make_pair("timestamp", std::string(timestamp)));
    uriParams.insert(std::make_pair("v", mVerison));
    uriParams.insert(std::make_pair("app_key", mAppKey));
    uriParams.insert(std::make_pair("method", mMethodDeviceInfo));
    uriParams.insert(std::make_pair("access_token", mAccessToken));

    g_free(signature);
    g_free(timestamp);

    std::string postData(data);
    HeadersMap headers;
    postData.insert(17, "=");

    int ret = mHttpClient->post(mBaseUri, uriParams, postData, headers, setDeviceInfoCallback, (void *)dev);
    if (ret != 0) {
        fprintf(stderr, "network's error\n");
        return RS_NETWORK_ERROR;
    }

    return RS_OK;
}

RSCode JDHttpRequest::controlDevice(Device *dev)
{
    if (!mIsSignIn) {
        fprintf(stderr, "must sign in at first\n");
        return RS_SIGN_IN_ERROR;
    }

    if (!dev) {
        fprintf(stderr, "invalid parameter\n");
        return RS_INVALID_PARAMETER;
    }

    std::list<SupportedCommand > cmdList = dev->getCommands();
    if (cmdList.empty()) {
        fprintf(stderr, "command list is empty\n");
        return RS_INVALID_PARAMETER;
    }

    char *signature = nullptr;
    char *timestamp = nullptr;
    char data[2048] = {0};
    char tmp[64] = {0};
    std::string commandsStr;
    std::list<SupportedCommand >::iterator it;

    commandsStr.append("[");

    for (it = cmdList.begin(); it != cmdList.end(); it++) {
        SupportedCommand cmd = (SupportedCommand)(*it);
        snprintf(tmp, 63, "{\\\"current_value\\\":\\\"%s\\\", \\\"stream_id\\\":\\\"%s\\\"}",
                cmd.getStatus().c_str(), cmd.getID().c_str());
        commandsStr.append(tmp);

        if (cmd.getID() != cmdList.back().getID()) {
            commandsStr.append(",");
        }
    }

    commandsStr.append("]");

    snprintf(data, 2047, "360buy_param_json{\"command\":\"%s\", \"id\":\"%s\"}", 
                            commandsStr.c_str(), dev->getID().c_str());
    printf("device control param: %s\n", data);

    getSignature(&signature, &timestamp, mMethodDeviceControl.c_str(), data);
    if (!signature || !timestamp) {
        fprintf(stderr, "failed to get signature\n");
        return RS_GET_SIGNATURE_ERROR;
    }

    ParamsMap uriParams;
    uriParams.insert(std::make_pair("sign", std::string(signature)));
    uriParams.insert(std::make_pair("timestamp", std::string(timestamp)));
    uriParams.insert(std::make_pair("v", mVerison));
    uriParams.insert(std::make_pair("app_key", mAppKey));
    uriParams.insert(std::make_pair("method", mMethodDeviceControl));
    uriParams.insert(std::make_pair("access_token", mAccessToken));

    g_free(signature);
    g_free(timestamp);

    std::string postData(data);
    HeadersMap headers;
    postData.insert(17, "=");

    int ret = mHttpClient->post(mBaseUri, uriParams, postData, headers, controlDeviceCallback, (void *)this);
    if (ret != 0) {
        fprintf(stderr, "network's error\n");
        return RS_NETWORK_ERROR;
    }

    return RS_OK;
}