#ifndef DEVICE_H_
#define DEVICE_H_

#include "RsTypes.h"

using namespace std;

class SupportedCommand
{
public:
    SupportedCommand() {}

    SupportedCommand(string id, string status) {
        mID = id;
        mStatus = status;
    }

    string getID() {
        return mID;
    }

    void setID(string id) {
        mID = id;
    }
    
    void setStatus(string status) {
        mStatus = status;
    }

    string getStatus() {
        return mStatus;
    }

private:
    string mID;
    string mStatus;
};

class Device
{
public:    
    Device() {
        mSupportedCommands = new list<SupportedCommand *>;
    }

    Device(string id, string name) {
        mID = id;
        mName = name;
        mSupportedCommands = new list<SupportedCommand *>;
    }

    ~Device() {
        if (mSupportedCommands) {
            std::list<SupportedCommand *>::iterator it;
            for (it = mSupportedCommands->begin(); it != mSupportedCommands->end(); it++) {
                SupportedCommand *cmd = (SupportedCommand *)(*it);

                if (cmd) {
                    delete cmd;
                    cmd = nullptr;
                }
            }

            delete mSupportedCommands;
            mSupportedCommands = nullptr;
        }
    }
    
    string getID() {
        return mID;
    }

    void setID(string id) {
        mID = id;
    }

    void setProductUUID(string productUUID) {
        mProductUUID = productUUID;
    }

    string getProductUUID() {
        return mProductUUID;
    }
    
    void setName(string name) {
        mName = name;
    }

    string getName() {
        return mName;
    }
    
    void setType(string type) {
        mType = type;
    }

    string getType() {
        return mType;
    }

    void setStatus(string status) {
        mStatus = status;
    }

    string getStatus() {
        return mStatus;
    }

    list<SupportedCommand > getCommands() {
        return mCommands;
    }

    void setCommands(list<SupportedCommand > &cmds) {
        if (cmds.empty()) {
            return;
        }

        mCommands.clear();

        list<SupportedCommand >::iterator it;
        for (it = cmds.begin(); it != cmds.end(); it++) {
            SupportedCommand cmd = (SupportedCommand)(*it);
            mCommands.push_back(cmd);
        }
    }

    void setSupportedCommands(list<SupportedCommand *> *supportedCommands) {
        mSupportedCommands = supportedCommands;
    }

    list<SupportedCommand *>* getSupportedCommands() {
        return mSupportedCommands;
    }

private:
    string mID;
    string mProductUUID;
    string mName;
    string mType;
    string mStatus; //0-offline, 1-online
    list<SupportedCommand > mCommands;
    list<SupportedCommand *> *mSupportedCommands;
};

#endif /* DEVICE_H_ */