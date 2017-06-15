#ifndef DEVICE_MANAGER_H_
#define DEVICE_MANAGER_H_

#include "ResourceSet.h"
#include "IHttpRequest.h"
#include "CIConnector.h"

class DeviceManager
{
public:
    static DeviceManager *getInstance();

    static void destroy();

    void init();

    RSCode signIn(std::string &authProvider, 
                    std::string &authCode);

    RSCode signOut();

    RSCode registerResources(std::string authCode = "");

    void unregisterResources();

private:
    DeviceManager();

    ~DeviceManager();

    RSCode createResources(std::list<Device *> *devList);

private:
	IHttpRequest *mHttpRequest;
	CIConnector *mCIConnector;

    //outlet
    USPResource *mUSPResource;
    PowerResource *mPowerResource;
    
    //rvc
    RvcResource *mRvcResource;
    PowerResource *mRvcPowerResource;
    MovementResource *mMovementResource;
    ModeResource *mModeResource;
    BatteryChargeResource *mBatteryChargeResource;

    //oven
    OvenResource *mOvenResource;
    KidLockResource *mKidLockResource;
    OvenOperationResource *mOvenOperationResource;

    ResourceHandles mResourceHandles;

	static DeviceManager *s_instance;
};

#endif /* DEVICE_MANAGER_H_ */