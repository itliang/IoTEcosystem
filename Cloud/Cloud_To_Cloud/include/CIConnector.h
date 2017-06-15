#ifndef CI_CONNECTOR_H_
#define CI_CONNECTOR_H_

#include <condition_variable>
#include "BaseResource.h"

#include "ocstack.h"
#include "ocpayload.h"
#include "RDClient.h"

using namespace OC;

class CIConnector
{
public:
    CIConnector();

    ~CIConnector();

	RSCode signUp(std::string &authProvider, std::string &authCode);

	RSCode signIn();

	RSCode signOut();

	RSCode registerDeviceInfo(Device *dev);

	RSCode publishResource(ResourceHandles resourceHandles);

	RSCode deleteResourceFromRD(ResourceHandles resourceHandles);

private:
	void handleLoginoutCb(const HeaderOptions &,
	                      const OCRepresentation &rep, const int ecode);

	void publishResourceCb(const OCRepresentation &, const int &eCode);

	void deleteResourceCb(const int &eCode);

private:
	std::string mCIServer;
	OCAccountManager::Ptr mAccountMgr;
	condition_variable mCallbackLock;

	std::string mUUID;
	std::string mAccessToken;
};

#endif /* CI_CONNECTOR_H_ */