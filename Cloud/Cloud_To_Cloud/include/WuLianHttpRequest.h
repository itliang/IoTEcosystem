#ifndef WULIAN_HTTP_REQUEST_H_
#define WULIAN_HTTP_REQUEST_H_

#include <json/json.h>
#include "IHttpRequest.h"

class WuLianHttpRequest : public IHttpRequest
{
protected:

public:
    WuLianHttpRequest();

    ~WuLianHttpRequest(){};

    RSCode signIn(const ParamsMap &params, 
                Auth_Type type = AUTH_RESOURCE_OWNER_PASSWORD_CREDENTIALS);

    RSCode bindGateway();

    std::list<Device *>* getDeviceList();

    RSCode setDeviceInfo(Device *dev) {}

    RSCode controlDevice(Device *dev);

private:
	static size_t signInCallback(char *ptr, 
                        size_t size, 
                        size_t nmemb, 
                        void *userdata);

	static size_t getDeviceListCallback(char *ptr, 
                            size_t size, 
                            size_t nmemb, 
                            void *userdata);

    static size_t controlDeviceCallback(char *ptr, 
                                        size_t size, 
                                        size_t nmemb, 
                                        void *userdata);

private:
	std::string mBaseUri;
    std::string mCmdInit;
    std::string mCmdBindDevice;
    std::string mCmdGetDeviceByUser;
    std::string mCmdCommDeviceCtr;

	std::string mAccessToken;

    std::string mUserId;
    std::string mGWDeviceId;
    std::string mGWDevicePassword;
    std::list<Device *> *mDevList;

    bool mIsDeviceBind;
};

#endif /* WULIAN_HTTP_REQUEST_H_ */