#ifndef JD_HTTP_REQUEST_H_
#define JD_HTTP_REQUEST_H_

#include <json/json.h>
#include "IHttpRequest.h"

class JDHttpRequest : public IHttpRequest
{
public:
    JDHttpRequest();

    ~JDHttpRequest();

    RSCode signIn(const ParamsMap &params, 
    			Auth_Type type = AUTH_CODE);

    RSCode bindGateway() { return RS_OK; }

    std::list<Device *>* getDeviceList();

    RSCode setDeviceInfo(Device *dev);

    RSCode controlDevice(Device *dev);

private:
    void getSignature(char **signature, 
                    char **timestamp, 
                    const char *method, 
                    const char *param);

    RSCode refreshAccessToken();

private:
    static void timeOutCallback(int fd, 
                                short event, 
                                void *userdata);

	static size_t signInCallback(char *ptr, 
                        size_t size, 
                        size_t nmemb, 
                        void *userdata);

	static size_t getDeviceListCallback(char *ptr, 
                            size_t size, 
                            size_t nmemb, 
                            void *userdata);

    static size_t setDeviceInfoCallback(char *ptr, 
                                        size_t size, 
                                        size_t nmemb, 
                                        void *userdata);

    static size_t controlDeviceCallback(char *ptr, 
                                        size_t size, 
                                        size_t nmemb, 
                                        void *userdata);
private:
	std::string mBaseUri;
	std::string mOAuthUri;
	std::string mRedirctUri;
    std::string mMethodDeviceList;
    std::string mMethodDeviceInfo;
    std::string mMethodDeviceControl;
    std::string mVerison;

	std::string mAppKey;
	std::string mAppSecret;
	std::string mAccessToken;
    std::string mRefreshToken;
    std::list<Device *> *mDevList;

    struct event *mTimer;
    struct timeval mTimeval;
};

#endif /* JD_HTTP_REQUEST_H_ */