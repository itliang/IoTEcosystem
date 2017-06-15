#include "CIConnector.h"
// #include "CAManager.h"

CIConnector::CIConnector()
{
    mCIServer     = "coap+tcp://10.25.34.132:5683";
    mAccountMgr   = OCPlatform::constructAccountManagerObject(mCIServer, CT_ADAPTER_TCP);
}

CIConnector::~CIConnector()
{

}

void CIConnector::handleLoginoutCb(const HeaderOptions &,
                      const OCRepresentation &rep, const int ecode)
{
    if (ecode == 4) {
        std::string accesstoken = rep.getValueToString("accesstoken");
        std::string uuid = rep.getValueToString("uid");

        if (accesstoken.length() > 0 &&
            uuid.length() > 0) {
            mUUID = uuid;
            mAccessToken = accesstoken;
        }
    }

    mCallbackLock.notify_all();
}

void CIConnector::publishResourceCb(const OCRepresentation &, const int &eCode)
{
    printf("publish resource to RD response received, code: %d\n", eCode);
}

void CIConnector::deleteResourceCb(const int &eCode)
{
    printf("delete resource from RD response received, code: %d\n", eCode);
}

RSCode CIConnector::signUp(std::string &authProvider, std::string &authCode)
{
    mutex blocker;
    unique_lock<mutex> lock(blocker);

    PostCallback cb = std::bind(&CIConnector::handleLoginoutCb, 
                                this, 
                                std::placeholders::_1, 
                                std::placeholders::_2, 
                                std::placeholders::_3);

    mAccountMgr->signUp(authProvider, authCode, cb);

    mCallbackLock.wait(lock);

    if (mAccessToken.length() > 0 &&
        mUUID.length() > 0) {
        printf("sign up successfully\n");
        return RS_OK;
    }

    return RS_SIGN_UP_ERROR;
}

RSCode CIConnector::signIn()
{
    if (mAccessToken.length() == 0 ||
        mUUID.length() == 0) {
        printf("must sign up at first\n");
        return RS_SIGN_UP_ERROR;
    }

    mutex blocker;
    unique_lock<mutex> lock(blocker);

    printf("uuid: %s\n", mUUID.c_str());
    printf("access token: %s\n", mAccessToken.c_str());

    PostCallback cb = std::bind(&CIConnector::handleLoginoutCb, 
                                this, std::placeholders::_1, 
                                std::placeholders::_2, 
                                std::placeholders::_3);

    int ret = mAccountMgr->signIn(mUUID, mAccessToken, cb);
    if (ret != OC_STACK_OK) {
        printf("failed to sign in\n");
        return RS_SIGN_IN_ERROR;
    }

    mCallbackLock.wait(lock);

    return RS_OK;
}

RSCode CIConnector::signOut()
{
    if (mAccessToken.length() == 0) {
        printf("no existed access token\n");
        return RS_SIGN_UP_ERROR;
    }

    mutex blocker;
    unique_lock<mutex> lock(blocker);

    printf("access token: %s\n", mAccessToken.c_str());

    PostCallback cb = std::bind(&CIConnector::handleLoginoutCb, 
                                this, std::placeholders::_1, 
                                std::placeholders::_2, 
                                std::placeholders::_3);

    int ret = mAccountMgr->signOut(mAccessToken, cb);
    if (ret != OC_STACK_OK) {
        printf("failed to sign out\n");
        return RS_SIGN_OUT_ERROR;
    }

    mCallbackLock.wait(lock);

    return RS_OK;
}

RSCode CIConnector::registerDeviceInfo(Device *dev)
{
    OCDeviceInfo devInfo;
    OCStringLL deviceType;

    deviceType.value = (char *)dev->getType().c_str();
    deviceType.next = NULL;

    devInfo.deviceName = (char *)dev->getName().c_str();
    devInfo.types = &deviceType;
    devInfo.specVersion = NULL;
    devInfo.dataModelVersions = NULL;

    int ret = OCPlatform::registerDeviceInfo(devInfo);
    if (ret != OC_STACK_OK) {
        printf("failed to register device info to cloud\n");
        return RS_DEVICE_INFO_REGISTER_ERROR;
    }

    return RS_OK;    
}

RSCode CIConnector::publishResource(ResourceHandles resourceHandles)
{
    PublishResourceCallback cb = std::bind(&CIConnector::publishResourceCb, 
                                            this, 
                                            std::placeholders::_1, 
                                            std::placeholders::_2);

    OCConnectivityType connectivityType = static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4);

    int ret = RDClient::Instance().publishResourceToRD(mCIServer, 
                                            connectivityType, 
                                            resourceHandles,
                                            cb);
    if (ret != OC_STACK_OK) {
        printf("failed to publish resource to cloud\n");
        return RS_PUBLISH_RESOURCE_TO_RD_ERROR;
    }

    return RS_OK;    
}

RSCode CIConnector::deleteResourceFromRD(ResourceHandles resourceHandles)
{
    DeleteResourceCallback cb = std::bind(&CIConnector::deleteResourceCb, 
                                            this, 
                                            std::placeholders::_1);

    OCConnectivityType connectivityType = static_cast<OCConnectivityType>(CT_ADAPTER_TCP | CT_IP_USE_V4);

    int ret = RDClient::Instance().deleteResourceFromRD(mCIServer, 
                                            connectivityType, 
                                            resourceHandles,
                                            cb);
    if (ret != OC_STACK_OK) {
        printf("failed to delete resource from cloud\n");
        return RS_DELETE_RESOURCE_FROM_RD_ERROR;
    }

    return RS_OK;    
}