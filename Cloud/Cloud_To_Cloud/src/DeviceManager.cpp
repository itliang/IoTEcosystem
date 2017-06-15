#include "Util.h"
#include "DeviceManager.h"
#include "JDHttpRequest.h"
#include "WuLianHttpRequest.h"

DeviceManager* DeviceManager::s_instance = NULL;

static char CRED_FILE[] = "../data/oic_svr_db_server.dat";

static FILE* server_fopen(const char *path, const char *mode)
{
	OC_UNUSED(path);
	return fopen(CRED_FILE, mode);
}

DeviceManager::DeviceManager()
{
	if (1 == g_auth_provider)
    	mHttpRequest = new WuLianHttpRequest();
    else
		mHttpRequest = new JDHttpRequest();

	mCIConnector = new CIConnector();

    mUSPResource = nullptr;
    mPowerResource = nullptr;

    mRvcResource = nullptr;
    mRvcPowerResource = nullptr;
    mMovementResource = nullptr;
    mModeResource = nullptr;
    mBatteryChargeResource = nullptr;

    mOvenResource = nullptr;
    mKidLockResource = nullptr;
    mOvenOperationResource = nullptr;
}

DeviceManager::~DeviceManager()
{
	if (mCIConnector) {
		delete mCIConnector;
		mCIConnector = nullptr;
	}

	if (mHttpRequest) {
		delete mHttpRequest;
		mHttpRequest = nullptr;
	}
}

DeviceManager* DeviceManager::getInstance()
{
    if (!s_instance) {
        if (!s_instance) {
            s_instance = new DeviceManager();
        }
    }

    return s_instance;
}

void DeviceManager::destroy()
{
    if (s_instance) {
        delete s_instance;
        s_instance = NULL;
    }
}

void DeviceManager::init()
{
	OCPersistentStorage ps { server_fopen, fread, fwrite, fclose, unlink };

	PlatformConfig cfg
	{
        ServiceType::InProc,
        ModeType::Both,
        "0.0.0.0", // By setting to "0.0.0.0", it binds to all available interfaces
        0,         // Uses randomly available port
        QualityOfService::LowQos,
        &ps
	};

	OCPlatform::Configure(cfg);
}

RSCode DeviceManager::signIn(std::string &authProvider, std::string &authCode)
{
	RSCode ret = RS_OK;

	ret = mCIConnector->signUp(authProvider, authCode);
	if (ret != RS_OK) {
		fprintf(stderr, "failed to sign up CI server\n");
		return ret;
	}

	ret = mCIConnector->signIn();
	if (ret != RS_OK) {
		fprintf(stderr, "failed to sign in CI server\n");
	}

	return ret;
}

RSCode DeviceManager::signOut()
{
	RSCode ret = RS_OK;

	ret = mCIConnector->signOut();
	if (ret != RS_OK) {
		fprintf(stderr, "failed to sign out CI server\n");
	}

	return ret;
}

RSCode DeviceManager::registerResources(std::string authCode)
{
	RSCode ret = RS_OK;
	ParamsMap params;

	if (1 == g_auth_provider) {
	    params.insert(std::make_pair("partnerId", WULIAN_USER_ID));
	    params.insert(std::make_pair("partnerPasswd", WULIAN_USER_PWD));
	} else {
		params.insert(std::make_pair("AuthCode", authCode));
		params.insert(std::make_pair("AppKey", JD_APP_KEY));
		params.insert(std::make_pair("AppSecret", JD_APP_SECRET));
	}

	ret = mHttpRequest->signIn(params);
	if (ret != RS_OK) {
		fprintf(stderr, "failed to sign in 3rd cloud\n");
		return ret;
	}

	ret = mHttpRequest->bindGateway();
	if (ret != RS_OK) {
		fprintf(stderr, "failed to bind gateway\n");
		return ret;
	}

	std::list<Device *> *devList = mHttpRequest->getDeviceList();
	if (!devList || devList->empty()) {
		fprintf(stderr, "failed to get device list\n");
		return RS_GET_DEVICES_ERROR;
	}
#if 0
	std::list<Device *>::iterator it;

	for (it = devList->begin(); it != devList->end(); it++) {
		Device *dev = (Device *)(*it);
		std::string product_uuid = dev->getProductUUID();

		if ("XMS7RF" == product_uuid) {
			SupportedCommand powerCmd("power", "1");
			list<SupportedCommand > cmdList;
			cmdList.push_back(powerCmd);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(10);

			//up
			cmdList.clear();
			SupportedCommand dirCmd1("Direction", "1");
			cmdList.push_back(dirCmd1);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//back
			cmdList.clear();
			SupportedCommand dirCmd2("Direction", "2");
			cmdList.push_back(dirCmd2);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//left
			cmdList.clear();
			SupportedCommand dirCmd3("Direction", "3");
			cmdList.push_back(dirCmd3);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//right
			cmdList.clear();
			SupportedCommand dirCmd4("Direction", "4");
			cmdList.push_back(dirCmd4);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//auto mode
			cmdList.clear();
			SupportedCommand workMode1("WorkMode", "1");
			cmdList.push_back(workMode1);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(10);

			//local mode
			cmdList.clear();
			SupportedCommand workMode2("WorkMode", "2");
			cmdList.push_back(workMode2);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//border mode
			cmdList.clear();
			SupportedCommand workMode3("WorkMode", "3");
			cmdList.push_back(workMode3);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//plan mode
			cmdList.clear();
			SupportedCommand workMode4("WorkMode", "4");
			cmdList.push_back(workMode4);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//sweep floor mode
			cmdList.clear();
			SupportedCommand workMode5("WorkMode", "5");
			cmdList.push_back(workMode5);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(20);

			//stop mode
			cmdList.clear();
			SupportedCommand workMode0("WorkMode", "0");
			cmdList.push_back(workMode0);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(10);

			//auto recharge on
			cmdList.clear();
			SupportedCommand autoRecharge1("Auto_Recharge", "1");
			cmdList.push_back(autoRecharge1);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(10);

			//auto recharge off
			cmdList.clear();
			SupportedCommand autoRecharge0("Auto_Recharge", "0");
			cmdList.push_back(autoRecharge0);
			dev->setCommands(cmdList);
			mHttpRequest->controlDevice(dev);
			sleep(10);

			break;
		}
	}
#else
	ret = createResources(devList);
	if (ret != RS_OK) {
		fprintf(stderr, "failed to create resources\n");
		return ret;
	}
#endif
    printf("register resources done\n");
    return RS_OK;
}

RSCode DeviceManager::createResources(std::list<Device *> *devList)
{
	if (!devList || devList->empty()) {
		fprintf(stderr, "invalid parameter\n");
		return RS_INVALID_PARAMETER;
	}

	OCStackResult result = OC_STACK_OK;
	std::list<Device *>::iterator it;
	string uri, rt, itf;

	for (it = devList->begin(); it != devList->end(); it++) {
		Device *dev = (Device *)(*it);
		std::string product_uuid = dev->getProductUUID();

		if ("USPVGB" == product_uuid) {
			mUSPResource = new USPResource("/jd/usp/0", 
										{ "x.com.jd.da.usp" }, 
										{ DEFAULT_INTERFACE, BATCH_INTERFACE, LINK_INTERFACE });

			uri = mUSPResource->getResourceUri();
			rt = mUSPResource->getResourceType()[0];
			itf = mUSPResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mUSPResource->m_handle,
												uri,
												rt,
												itf,
												bind(&USPResource::entityHandler
													, mUSPResource, placeholders::_1),
												OC_DISCOVERABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register USPResource done\n");

			itf = mUSPResource->getInterfaces()[1];
			result = OCPlatform::bindInterfaceToResource(mUSPResource->m_handle, itf);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Binding 2nd interface was unsuccessful\n");
				return RS_BIND_RESOURCE_INTERFACE_ERROR;
			}

			itf = mUSPResource->getInterfaces()[2];
			result = OCPlatform::bindInterfaceToResource(mUSPResource->m_handle, itf);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Binding 3nd interface was unsuccessful\n");
				return RS_BIND_RESOURCE_INTERFACE_ERROR;
			}
			printf("bind USPResource interfaces done\n");

			mPowerResource = new PowerResource("/power/vs/0", 
													{ "x.com.jd.da.operation" }, 
													{ DEFAULT_INTERFACE });
			mPowerResource->setDevice(dev);
			mPowerResource->setHttpRequester(mHttpRequest);

			uri = mPowerResource->getResourceUri();
			rt = mPowerResource->getResourceType()[0];
			itf = mPowerResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mPowerResource->m_handle,
												uri,
												rt,
												itf,
												bind(&PowerResource::entityHandler
													, mPowerResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register PowerResource done\n");

			mUSPResource->addChildResource(mPowerResource);
			printf("add USPResource childs done\n");
		} else if ("XMS7RF" == product_uuid) {
			mRvcResource = new RvcResource("/jd/rvc/0", 
										{ "x.com.jd.da.rvc" }, 
										{ DEFAULT_INTERFACE, BATCH_INTERFACE, LINK_INTERFACE });

			uri = mRvcResource->getResourceUri();
			rt = mRvcResource->getResourceType()[0];
			itf = mRvcResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mRvcResource->m_handle,
												uri,
												rt,
												itf,
												bind(&RvcResource::entityHandler
													, mRvcResource, placeholders::_1),
												OC_DISCOVERABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register RvcResource done\n");

			itf = mRvcResource->getInterfaces()[1];
			result = OCPlatform::bindInterfaceToResource(mRvcResource->m_handle, itf);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Binding 2nd interface was unsuccessful\n");
				return RS_BIND_RESOURCE_INTERFACE_ERROR;
			}

			itf = mRvcResource->getInterfaces()[2];
			result = OCPlatform::bindInterfaceToResource(mRvcResource->m_handle, itf);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Binding 3nd interface was unsuccessful\n");
				return RS_BIND_RESOURCE_INTERFACE_ERROR;
			}
			printf("bind RvcResource interfaces done\n");

			mRvcPowerResource = new PowerResource("/power/vs/1", 
													{ "x.com.jd.da.operation" }, 
													{ DEFAULT_INTERFACE });
			mRvcPowerResource->setDevice(dev);
			mRvcPowerResource->setHttpRequester(mHttpRequest);

			uri = mRvcPowerResource->getResourceUri();
			rt = mRvcPowerResource->getResourceType()[0];
			itf = mRvcPowerResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mRvcPowerResource->m_handle,
												uri,
												rt,
												itf,
												bind(&PowerResource::entityHandler
													, mRvcPowerResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register PowerResource done\n");

			mMovementResource = new MovementResource("/movement/vs/0", 
													{ "x.com.jd.da.movement" }, 
													{ DEFAULT_INTERFACE });
			mMovementResource->setDevice(dev);
			mMovementResource->setHttpRequester(mHttpRequest);

			uri = mMovementResource->getResourceUri();
			rt = mMovementResource->getResourceType()[0];
			itf = mMovementResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mMovementResource->m_handle,
												uri,
												rt,
												itf,
												bind(&MovementResource::entityHandler
													, mMovementResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register MovementResource done\n");

			mModeResource = new ModeResource("/mode/vs/0", 
											{ "x.com.jd.da.mode" }, 
											{ DEFAULT_INTERFACE });
			mModeResource->setDevice(dev);
			mModeResource->setHttpRequester(mHttpRequest);

			uri = mModeResource->getResourceUri();
			rt = mModeResource->getResourceType()[0];
			itf = mModeResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mModeResource->m_handle,
												uri,
												rt,
												itf,
												bind(&ModeResource::entityHandler
													, mModeResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register ModeResource done\n");

			mBatteryChargeResource = new BatteryChargeResource("/energy/battery/vs/0", 
											{ "x.com.samsung.da.energyconsumption" }, 
											{ DEFAULT_INTERFACE });
			mBatteryChargeResource->setDevice(dev);
			mBatteryChargeResource->setHttpRequester(mHttpRequest);

			uri = mBatteryChargeResource->getResourceUri();
			rt = mBatteryChargeResource->getResourceType()[0];
			itf = mBatteryChargeResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mBatteryChargeResource->m_handle,
												uri,
												rt,
												itf,
												bind(&BatteryChargeResource::entityHandler
													, mBatteryChargeResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register BatteryChargeResource done\n");

			mRvcResource->addChildResource(mRvcPowerResource);
			mRvcResource->addChildResource(mMovementResource);
			mRvcResource->addChildResource(mModeResource);
			mRvcResource->addChildResource(mBatteryChargeResource);
			printf("add RvcResource childs done\n");
		} else if ("PUUFZK" == product_uuid) {
			mOvenResource = new OvenResource("/jd/oven/0", 
										{ "x.com.jd.da.oven" }, 
										{ DEFAULT_INTERFACE, BATCH_INTERFACE, LINK_INTERFACE });

			uri = mOvenResource->getResourceUri();
			rt = mOvenResource->getResourceType()[0];
			itf = mOvenResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mOvenResource->m_handle,
												uri,
												rt,
												itf,
												bind(&OvenResource::entityHandler
													, mOvenResource, placeholders::_1),
												OC_DISCOVERABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register OvenResource done\n");

			itf = mOvenResource->getInterfaces()[1];
			result = OCPlatform::bindInterfaceToResource(mOvenResource->m_handle, itf);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Binding 2nd interface was unsuccessful\n");
				return RS_BIND_RESOURCE_INTERFACE_ERROR;
			}

			itf = mOvenResource->getInterfaces()[2];
			result = OCPlatform::bindInterfaceToResource(mOvenResource->m_handle, itf);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Binding 3nd interface was unsuccessful\n");
				return RS_BIND_RESOURCE_INTERFACE_ERROR;
			}
			printf("bind OvenResource interfaces done\n");

			mKidLockResource = new KidLockResource("/kidlock/vs/0", 
													{ "x.com.jd.da.operation" }, 
													{ DEFAULT_INTERFACE });
			mKidLockResource->setDevice(dev);
			mKidLockResource->setHttpRequester(mHttpRequest);

			uri = mKidLockResource->getResourceUri();
			rt = mKidLockResource->getResourceType()[0];
			itf = mKidLockResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mKidLockResource->m_handle,
												uri,
												rt,
												itf,
												bind(&KidLockResource::entityHandler
													, mKidLockResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register KidLockResource done\n");

			mOvenOperationResource = new OvenOperationResource("/operation/vs/0", 
													{ "x.com.jd.da.operation" }, 
													{ DEFAULT_INTERFACE });
			mOvenOperationResource->setDevice(dev);
			mOvenOperationResource->setHttpRequester(mHttpRequest);

			uri = mOvenOperationResource->getResourceUri();
			rt = mOvenOperationResource->getResourceType()[0];
			itf = mOvenOperationResource->getInterfaces()[0];

			result = OCPlatform::registerResource(mOvenOperationResource->m_handle,
												uri,
												rt,
												itf,
												bind(&OvenOperationResource::entityHandler
													, mOvenOperationResource, placeholders::_1),
												OC_OBSERVABLE);

			if (result != OC_STACK_OK) {
				fprintf(stderr, "Resource registration was unsuccessful\n");
				return RS_RESOURCE_REGISTER_ERROR;
			}
			printf("register OvenOperationResource done\n");

			mOvenResource->addChildResource(mKidLockResource);
			mOvenResource->addChildResource(mOvenOperationResource);
			printf("add OvenResource childs done\n");
		}
	}

	Device dev("jd_0001", "JD_GATEWAY");
	dev.setType("com.jd.da.gateway");
	mCIConnector->registerDeviceInfo(&dev);
	printf("register gateway device done\n");

	mCIConnector->publishResource(mResourceHandles);

	if (mUSPResource) {
		mResourceHandles.push_back(mUSPResource->m_handle);
	}
	
	if (mRvcResource) {
		mResourceHandles.push_back(mRvcResource->m_handle);
	}
	
	if (mOvenResource) {
		mResourceHandles.push_back(mOvenResource->m_handle);
	}

	mCIConnector->publishResource(mResourceHandles);
	printf("publish resource to RD server done\n");

	return RS_OK;
}

void DeviceManager::unregisterResources()
{
	mCIConnector->deleteResourceFromRD(mResourceHandles);

	if (mUSPResource) {
		OCPlatform::unregisterResource(mUSPResource->m_handle);
		delete mUSPResource;
		mUSPResource = nullptr;
	}

	if (mPowerResource) {
		OCPlatform::unregisterResource(mPowerResource->m_handle);
		delete mPowerResource;
		mPowerResource = nullptr;
	}

	if (mRvcResource) {
		OCPlatform::unregisterResource(mRvcResource->m_handle);
		delete mRvcResource;
		mRvcResource = nullptr;
	}

	if (mRvcPowerResource) {
		OCPlatform::unregisterResource(mRvcPowerResource->m_handle);
		delete mRvcPowerResource;
		mRvcPowerResource = nullptr;
	}

	if (mMovementResource) {
		OCPlatform::unregisterResource(mMovementResource->m_handle);
		delete mMovementResource;
		mMovementResource = nullptr;
	}

	if (mModeResource) {
		OCPlatform::unregisterResource(mModeResource->m_handle);
		delete mModeResource;
		mModeResource = nullptr;
	}

	if (mBatteryChargeResource) {
		OCPlatform::unregisterResource(mBatteryChargeResource->m_handle);
		delete mBatteryChargeResource;
		mBatteryChargeResource = nullptr;
	}

	if (mOvenResource) {
		OCPlatform::unregisterResource(mOvenResource->m_handle);
		delete mOvenResource;
		mOvenResource = nullptr;
	}

	if (mKidLockResource) {
		OCPlatform::unregisterResource(mKidLockResource->m_handle);
		delete mKidLockResource;
		mKidLockResource = nullptr;
	}

	if (mOvenOperationResource) {
		OCPlatform::unregisterResource(mOvenOperationResource->m_handle);
		delete mOvenOperationResource;
		mOvenOperationResource = nullptr;
	}
}