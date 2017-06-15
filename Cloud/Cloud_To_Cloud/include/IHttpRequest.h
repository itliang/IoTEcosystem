#ifndef IHTTP_REQUEST_H_
#define IHTTP_REQUEST_H_

#include "Device.h"
#include "HttpClient.h"

class IHttpRequest
{
public:
    IHttpRequest()
    {
    	mHttpClient = HttpClient::getInstance();
    }

    ~IHttpRequest()
    {
    	mHttpClient->destroy();
    }

    virtual RSCode signIn(const ParamsMap &params, 
    					Auth_Type type = AUTH_CODE) = 0;
    
	virtual RSCode bindGateway() = 0;
    
    virtual std::list<Device *>* getDeviceList() = 0;

    virtual RSCode setDeviceInfo(Device *dev) = 0;

    virtual RSCode controlDevice(Device *dev) = 0;

protected:
	bool mIsSignIn;
	HttpClient *mHttpClient;
};

#endif /* IHTTP_REQUEST_H_ */