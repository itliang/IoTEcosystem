#ifndef _PLUG_INFO_H_
#define _PLUG_INFO_H_

#include <string>
#include <list>
#include "database/constant/PluginInfoTable.h"
#include "database/constant/PluginStatusCode.h"
#include "database/constant/PluginDataStatusCode.h"
using namespace std;

class PluginInfo {
private:
	/**
	* Plugin ID, uniquely assigned by the plugin server
	*/
	string mId;
	
	/**
	* The version code of a plugin
	*/
	string mVersionCode;
	
	/**
	* The version name of a plugin
	*/
	string mVersionName;
	
	/**
	* The name of a plugin
	*/
	string mName;

	/**
	* file size
	*/
	long mFileSize;

	/**
	*
	*/
	//    private int installed;

    int mPluginType;
	int mPluginStatus;
    int mPluginDataStatus;
	long long mLastCheckUpdate;
    long long mTimeStamp;

	string mUrl;
	string mReqTime;
	string mHashValue;
	string mDeviceManufacturer;
	string mModelID;
	string mPlatformVersion;
	string mDeviceType;
	string mDeviceSubType;
	string mSignature;
	string mDependencies;

public:
	PluginInfo(){
		mVersionCode = "";
		mFileSize = 0;
	    mPluginType = 0;
		mPluginStatus = 0;
	    mPluginDataStatus = 0;
		mLastCheckUpdate = 0;
	    mTimeStamp = 0;
	}
	
	PluginInfo(string id, string versionCode){
		mId=id;
		mVersionCode = versionCode;
		mFileSize = 0;
	    mPluginType = 0;
		mPluginStatus = 0;
	    mPluginDataStatus = 0;
		mLastCheckUpdate = 0;
	    mTimeStamp = 0;
	}
	
	PluginInfo(PluginInfo *libInfo) {
		setId(libInfo->getId());
		setVersionCode(libInfo->getVersionCode());
		setVersionName(libInfo->getVersionName());
		setName(libInfo->getName());
		setFileSize(libInfo->getFileSize());
		setUrl(libInfo->getUrl());
		setDeviceManufacturer(libInfo->getDeviceManufacturer());
		setModelID(libInfo->getModelID());
		setPlatformVersion(libInfo->getPlatformVersion());
		setDeviceType(libInfo->getDeviceType());
		setDeviceSubType(libInfo->getDeviceSubType());
		setDependencies(libInfo->getDependencies());
		setSignature(libInfo->getSignature());
		setPluginType(libInfo->getPluginType());
		setPluginStatus(libInfo->getPluginStatus());
		setPluginDataStatus(libInfo->getPluginDataStatus());
		setLastCheckUpdate(libInfo->getLastCheckUpdate());
		setTimeStamp(libInfo->getTimeStamp());
	}

	void setDeviceInfo(string deviceType, string deviceSubType, string deviceManufacturer, string modelID, string platformVersion) {
		mDeviceType = deviceType;
		mDeviceSubType = deviceSubType;
		mDeviceManufacturer = deviceManufacturer;
		mModelID = modelID;
		mPlatformVersion = platformVersion;
	}

	string getId() {return mId;}
	void setId(string id) {mId = id;}

	string getVersionCode() {  return mVersionCode;}
	void setVersionCode(string versionCode) { mVersionCode = versionCode;}

	string getVersionName() {return mVersionName;}
	void setVersionName(string versionName) {mVersionName = versionName;}

	string getName() {return mName;}
	void setName(string name) {mName = name;}

	string getUrl() {return mUrl;}
	void setUrl(string url) { mUrl = url;}

	long getFileSize() {return mFileSize;}
	void setFileSize(long fileSize) {mFileSize = fileSize;}

	long long getLastCheckUpdate() {return mLastCheckUpdate;}
	void setLastCheckUpdate(long long lastCheckUpdate) {mLastCheckUpdate = lastCheckUpdate;}

    int getPluginType() { return mPluginType;}
    void setPluginType(int type) { mPluginType = type;}

    int getPluginStatus() { return mPluginStatus;}
    void setPluginStatus(int status) { mPluginStatus = status; }
    void setPluginStatus(PluginStatusCode status) { mPluginStatus = (int)status;}

    int getPluginDataStatus() { return mPluginDataStatus;}
    void setPluginDataStatus(int status) { mPluginDataStatus = status;}
    void setPluginDataStatus(PluginDataStatusCode status) { mPluginDataStatus = (int)status;}

    bool isLocalTestPlugin() { return (mPluginType == PLUGIN_TYPE_LOCAL_TEST || mPluginType == PLUGIN_TYPE_LOCAL_WEB_TEST); }

    bool isLatestVersion() { return (mPluginStatus == STATUS_VALID); }

    long long getTimeStamp() { return mTimeStamp; }
    void setTimeStamp(long long time) { mTimeStamp = time;}

    string getSignature() { return mSignature; }
    void setSignature(string signature) { mSignature = signature; }

	string getVMFPath() {
		//return PluginPlatformConstants.VMFPATH_PREFIX+ File.separator+this.getId()+"_"+this.getVersion();
		return "";
	}
	string getOVMFPath() {
		//return PluginPlatformConstants.OVMFPATH_PREFIX+ File.separator+this.getId()+"_"+this.getVersion();
		return "";
	}

    string getDependencies() { return mDependencies;}
    void setDependencies(string dependencies) {mDependencies = dependencies;}

	string getKey(){
		if(getId() == "")
			return "";
		if(getVersionCode() == "")
			return getId();
		return getId() + "_" + getVersionCode();
	}

	string getDeviceTypeKey(){
        if(getDeviceType() == "")
            return "";

        if(getDeviceSubType() == "")
            return getDeviceType();

        return getDeviceType()+"-"+getDeviceSubType();
    }

	string toString() {return getKey()+":"+getDeviceTypeKey()+"["+getName()+"]";}

	string getDeviceManufacturer() {return mDeviceManufacturer;}
	void setDeviceManufacturer(string deviceManufacturer) {mDeviceManufacturer = deviceManufacturer;}

	string getModelID() {return mModelID;}
	void setModelID(string modelID) {mModelID = modelID;}

	string getPlatformVersion() {return mPlatformVersion;}
	void setPlatformVersion(string platformVersion) {mPlatformVersion = platformVersion;}

	string getDeviceType() {return mDeviceType;}
	void setDeviceType(string deviceType) {mDeviceType = deviceType;}

	string getDeviceSubType() {return mDeviceSubType;}
	void setDeviceSubType(string deviceSubType) {mDeviceSubType = deviceSubType;}
};

#endif
