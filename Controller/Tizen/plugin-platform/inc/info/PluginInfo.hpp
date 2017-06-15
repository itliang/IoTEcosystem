#ifndef _PLUG_INFO_H_
#define _PLUG_INFO_H_

#include <string>
#include <list>
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
	string mVersion;
	
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


	int mCurrentStatus;
	long mLastCheckUpdate;
	string mUrl;
	string mReqTime;
	string mHashValue;
	string mDeviceManufacturer;
	string mModelID;
	string mPlatformVersion;
	string mDeviceType;
	string mDeviceSubType;
	string mSignature;
	list<PluginInfo> *mDependencies;

public:
	string getSignature() {
		return mSignature;
	}

	void setSignature(string signature) {
		mSignature = signature;
	}
	
	

	PluginInfo(){
		mDependencies = new list<PluginInfo>();
		mVersion = "-1";
		mFileSize = 0;
	
	}
	
	PluginInfo(string id){
		mId=id;
		mFileSize = 0;
	}

	
	PluginInfo(PluginInfo *libInfo) {
		setName(libInfo->getName());
		setVersion(libInfo->getVersion());
		setVersionName(libInfo->getVersionName());
		setDependencies(libInfo->getDependencies());
		setFileSize(libInfo->getFileSize());
		setId(libInfo->getId());
		//		  this.setInstalled(libInfo.getInstalled());
		setUrl(libInfo->getUrl());
		setDeviceType(libInfo->getDeviceType());
		setDeviceSubType(libInfo->getDeviceSubType());
		setModelID(libInfo->getModelID());
		setDeviceManufacturer(libInfo->getDeviceManufacturer());
		setPlatformVersion(libInfo->getPlatformVersion());
		setCurrentStatus(libInfo->getCurrentStatus());
		setLastCheckUpdate(libInfo->getLastCheckUpdate());
	}

	void setDeviceInfo(string deviceType, string deviceSubType, string deviceManufacturer, string modelID, string platformVersion) {
		mDeviceType = deviceType;
		mDeviceSubType = deviceSubType;
		mDeviceManufacturer = deviceManufacturer;
		mModelID = modelID;
		mPlatformVersion = platformVersion;
	}

	/**
	 * Returns the id of a plugin
	 * @return the id of a plugin
	 */
	string getId() {return mId;}

	/**
	 * Sets the id of a plugin, do not use this method from your own code, it will break the plugin platform system
	 * @param id new id value
	 */
	void setId(string id) {mId = id;}

	/**
	 * Returns the name of a plugin
	 * @return the name of a plugin
	 */
	string getName() {return mName;}

	/**
	 * Sets the name of a plugin
	 * @param name the unique name of a plugin
	 */
	void setName(string name) {mName = name;}

	/**
	 * the download url of a plugin
	 * @return the download url of a plugin
	 */
	string getUrl() {return mUrl;}

	/**
	 * Sets the download url of a plugin, do not use this method from your own code, it will break the plugin platform system
	 * @param url
	 */
	void setUrl(string url) { mUrl = url;}

	long getFileSize() {return mFileSize;}

	void setFileSize(long fileSize) {mFileSize = fileSize;}

	long getLastCheckUpdate() {return mLastCheckUpdate;}

	void setLastCheckUpdate(long lastCheckUpdate) {mLastCheckUpdate = lastCheckUpdate;}

	int getCurrentStatus() {return mCurrentStatus;}

	void setCurrentStatus(int currentStatus) {mCurrentStatus = currentStatus;}

	string getHashValue() {return mHashValue;}

	void setHashValue(string hashValue) {mHashValue = hashValue;}

	string getReqTime() {return mReqTime;}

	void setReqTime(string reqTime) {mReqTime = reqTime;}

	/**
	*
	* @return
	*/
	string getVMFPath() {
		//return PluginPlatformConstants.VMFPATH_PREFIX+ File.separator+this.getId()+"_"+this.getVersion();
		return "";
	}

	/**
	*
	* @return
	*/
	string getOVMFPath() {
		//return PluginPlatformConstants.OVMFPATH_PREFIX+ File.separator+this.getId()+"_"+this.getVersion();
		return "";
	}

	/**
	*
	* @return
	*/
	list<PluginInfo> * getDependencies() {return mDependencies;}

	/**
	*
	* @param dependencies
	*/
	void setDependencies(list<PluginInfo> *dependencies) {mDependencies = dependencies;}

	/**
	*
	* @param lib
	*/
	void addLib(PluginInfo *lib) {mDependencies->push_back(lib);}


	/**
	*
	* @return
	*/
	string getVersion() {return mVersion;}

	/**
	*
	* @param version
	*/
	void setVersion(string version) {mVersion = version;}

	/**
	*
	* @return version name in String
	*/
	string getVersionName() {return mVersionName;}

	/**
	*
	* @param versionName
	*/
	void setVersionName(string versionName) {mVersionName = versionName;}

	string getKey(){return getId() + "_" + getVersion();}

	string toString() {return getName();}


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
