#ifndef _HOST_INFO_H_
#define _HOST_INFO_H_

using namespace std;
#include <string>
class HostInfo{

private:
	
	string platformVersion;
    string abi;

    string mcc=""; //mobile country code
    string mnc=""; //mobile network code
    string imei;
    string modelId;
    string csc;
    string sdkVersion;
	
	static HostInfo *instance;


public :
	static HostInfo*getInstance() {
        if(instance == NULL) {
            instance = new HostInfo();
        }
        return instance;
    }

    HostInfo(){
        setPlatformVersion("10.0.0");
        sdkVersion ="" ;
        string SAMSUNG_PREFIX = "SAMSUNG-";
        modelId = "";
        mcc="";
        mnc="";
    }

   

  
    string getPlatformVersion(){
        return platformVersion;
    }

    void setPlatformVersion(string platformVersion){
        this->platformVersion=platformVersion;
    }

    string getSdkVersion() {
        return sdkVersion;
    }

    void setSdkVersion(string sdkVersion) {
        this->sdkVersion = sdkVersion;
    }

    string getMcc() {
        return mcc;
    }

    void setMcc(string mcc) {
        this->mcc = mcc;
    }

    string getMnc() {
        return mnc;
    }

    void setMnc(string mnc) {
        this->mnc = mnc;
    }

    string getImei() {
        return imei;
    }

    void setImei(string imei) {
       
    }

    string getModelId() {
        return modelId;
    }

    void setModelId(string modelId) {
        this->modelId = modelId;
    }

    string getCsc() {
        return csc;
    }

    void setCsc(string csc) {
        this->csc = csc;
    }

   
    bool equals(HostInfo *o) {
       
		return (   (this->sdkVersion == o->getSdkVersion())
				&& (this->platformVersion == o->getPlatformVersion())
				&& (this->abi == o->getAbi())
				&& (this->mcc == o->getMcc())
				&& (this->mnc == o->getMnc())
				&& (this->imei == o->getImei()));
       
    }

   
    string toString() {
        return "SDK version: "+this->sdkVersion
                +"/"+"plugin platform: "+this->platformVersion
                +"/"+"cpu_arch: "+this->abi
                +"/"+"mcc: "+this->mcc
                +"/"+"mnc: "+this->mnc
                +"/"+"imei: "+this->imei
                +"/"+"modelId: "+this->modelId
                +"/"+"csc: "+this->csc;
    }

    void setAbi(string abi) {
        this->abi = abi;
    }

    string getAbi(){
        return abi;
    }

    int describeContents() {
        return 0;
    }
};

#endif
