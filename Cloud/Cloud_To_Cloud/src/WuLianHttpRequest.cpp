#include <time.h>
#include <openssl/md5.h>
#include "WuLianHttpRequest.h"

WuLianHttpRequest::WuLianHttpRequest()
{
	mBaseUri = "https://v2.wuliancloud.com:52182";
    mCmdInit = "cloudinit";
    mCmdBindDevice = "bindDevice";
    mCmdGetDeviceByUser = "getDeviceByUser";
    mCmdCommDeviceCtr = "forward12";

    mIsSignIn = false;

    //gateway information
    mUserId = "iot";
    mGWDeviceId = "50294D102871";
    mGWDevicePassword = "102871";
    mIsDeviceBind = false;
}

size_t WuLianHttpRequest::signInCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
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

    WuLianHttpRequest *pWuLianHttpRequest = (WuLianHttpRequest *)userdata;
	pWuLianHttpRequest->mAccessToken = root["token"].asString();
	printf("access token: %s\n", pWuLianHttpRequest->mAccessToken.c_str());
	pWuLianHttpRequest->mIsSignIn = true;

    free(pJsonParser);
    free(bodyMsg);
    return bytesSize;
}

size_t WuLianHttpRequest::getDeviceListCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
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

    std::list<Device *> *devList = (std::list<Device *> *)userdata;

    int num = root["devices"].size();
    printf("total of devices: %d\n", num);

    for (int i = 0; i < num; i++) {
        Json::Value device = root["devices"][i];

        int childs_num = device["device_childs"].size();
        printf("total of device childs: %d\n", childs_num);

        for (int j = 0; j < childs_num; j++) {
            Json::Value sub_device = device["device_childs"][j];

            std::string device_id = sub_device["device_childId"].asString();
            printf("device_id: %s\n", device_id.c_str());

            std::string device_type = sub_device["deviceType"].asString();
            printf("device_type: %s\n", device_type.c_str());

            int device_status = sub_device["status"].asInt();
            printf("device_status: %d\n", device_status);

            // BaseResource *res = new BaseResource();
            // res->setName(device_id);
            // res->setType(device_type);
        }
    }

    free(pJsonParser);
    free(bodyMsg);
    return bytesSize;
}

RSCode WuLianHttpRequest::signIn(const ParamsMap &params, Auth_Type type)
{	
	if (params.size() != 2) {
		fprintf(stderr, "invalid parameters\n");
		return RS_INVALID_PARAMETER;
	}

    std::string username, userpwd;
    std::map<std::string, std::string>::const_iterator iter;

    iter = params.find("partnerId");
    if (params.end() != iter) {   
        username = iter->second;
    }

    iter = params.find("partnerPasswd");
    if (params.end() != iter) {   
        userpwd = iter->second;
    }

	char buffer[512] = {0};
    snprintf(buffer, 511, "%s/%s", mBaseUri.c_str(), "AMS/cloud");

    char body[512] = {0};
    snprintf(body, 511, "{\"partnerId\": \"%s\", \"partnerPasswd\": \"%s\"}", 
            username.c_str(), userpwd.c_str());

    HeadersMap headers;
    headers.insert(std::pair<std::string, std::string>("cmd", mCmdInit));

    std::string uri(buffer);
    std::string postData(body);

    int ret = mHttpClient->post(uri, postData, headers, signInCallback, (void *)this);
    if (ret != 0) {
		fprintf(stderr, "network's error\n");
		return RS_NETWORK_ERROR;
    }

    return RS_OK;
}

RSCode WuLianHttpRequest::bindGateway()
{
    if (!mIsSignIn) {
        fprintf(stderr, "must sign in at first\n");
        return RS_SIGN_IN_ERROR;
    }

    char buffer[512] = {0};
    snprintf(buffer, 511, "%s/%s", mBaseUri.c_str(), "AMS/cloud");

    char body[512] = {0};
    snprintf(body, 511, "{\"userId\": \"%s\", \"deviceId\": \"%s\", \"devicePasswd\": \"%s\"}", 
            mUserId.c_str(), mGWDeviceId.c_str(), mGWDevicePassword.c_str());
    
    HeadersMap headers;
    headers.insert(std::pair<std::string, std::string>("token", mAccessToken));
    headers.insert(std::pair<std::string, std::string>("cmd", mCmdBindDevice));

    std::string uri(buffer);
    std::string postData(body);

    int ret = mHttpClient->post(uri, postData, headers, NULL, NULL);
    if (ret != 0) {
        fprintf(stderr, "network's error\n");
        return RS_NETWORK_ERROR;
    }

    mIsDeviceBind = true;

    return RS_OK;
}

std::list<Device *>* WuLianHttpRequest::getDeviceList()
{
	if (!mIsDeviceBind) {
		fprintf(stderr, "must bind device at first\n");
		return nullptr;
	}

    char buffer[512] = {0};
    snprintf(buffer, 511, "%s/%s", mBaseUri.c_str(), "AMS/cloud");

    char body[512] = {0};
    snprintf(body, 511, "{\"userId\": \"%s\"}", mUserId.c_str());

    HeadersMap headers;
    headers.insert(std::pair<std::string, std::string>("token", mAccessToken));
    headers.insert(std::pair<std::string, std::string>("cmd", mCmdGetDeviceByUser));

    std::string uri(buffer);
    std::string postData(body);

    int ret = mHttpClient->post(uri, postData, headers, getDeviceListCallback, (void *)this);
    if (ret != 0) {
        fprintf(stderr, "network's error\n");
        return nullptr;
    }

    return nullptr;
}

RSCode WuLianHttpRequest::controlDevice(Device *dev)
{
    if (!mIsDeviceBind) {
        fprintf(stderr, "must bind device at first\n");
        return RS_BIND_DEVICE_ERROR;
    }

    char buffer[512] = {0};
    snprintf(buffer, 511, "%s/%s", mBaseUri.c_str(), "OMS/cloud");

    char body[512] = {0};
    // snprintf(body, 511, "{\"cmd\": \"%s\", \"gwID\": \"%s\", \"devID\": \"%s\", " \
    //                     "\"type\": \"%s\", \"ep\": \"%s\", \"epType\": \"%s\", \"epData\": \"%s\"}", 
    //                     cmd.c_str(), mGWDeviceId.c_str(), 12, );

    HeadersMap headers;
    headers.insert(std::pair<std::string, std::string>("token", mAccessToken));
    headers.insert(std::pair<std::string, std::string>("userid", mUserId));
    headers.insert(std::pair<std::string, std::string>("cmd", mCmdCommDeviceCtr));

    std::string uri(buffer);
    std::string postData(body);

    // int ret = mHttpClient->post(uri, postData, headers, getDeviceListCallback, nullptr);
    // if (ret != 0) {
    //     fprintf(stderr, "network's error\n");
    //     return RS_NETWORK_ERROR;
    // }

    return RS_OK;
}