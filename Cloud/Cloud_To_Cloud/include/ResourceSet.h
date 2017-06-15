#ifndef RESOURCE_SET_H_
#define RESOURCE_SET_H_

#include "BaseResource.h"
#include "JDHttpRequest.h"

using namespace std;

class USPResource : public BaseResource
{
private:

public:
    USPResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {

    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("In Server USP entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                // If the request type is GET
                if (requestType == "GET")
                {
                    printf("requestType : GET\n");
                    string findRes = request->getQueryParameters().find("if")->second;
                    if (findRes.compare(LINK_INTERFACE) == 0)
                    {
                        if (OC_STACK_OK == sendRepresentation(request))
                        {
                            ehResult = OC_EH_OK;
                        }
                    }
                    else
                    {
                        ehResult = OC_EH_FORBIDDEN;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                }
                else if (requestType == "POST")
                {
                    // POST request operations
                }
                else if (requestType == "DELETE")
                {
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class PowerResource : public BaseResource
{
private:
    string m_value;
    vector<string> m_keyNames;

public:
    PowerResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {
        m_value = "0";
        m_keyNames.push_back("x.com.jd.da.power");
        m_representation.setValue("x.com.jd.da.power", m_value);
        m_representation.setValue("key", m_keyNames);
    }

    int setPowerRepresentation(OCRepresentation &rep)
    {
        string value;
        if (rep.getValue("x.com.jd.da.power", value))
        {
            m_value = value;
            m_representation.setValue("x.com.jd.da.power", m_value);
            printf("x.com.jd.da.power: %s\n", m_value.c_str());

            propagate();
            return 1;
        } else {
            fprintf(stderr, "x.com.jd.da.power is not found!!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("\tIn Server Power entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                if (requestType == "GET")
                {
                    printf("requestType : GET\n");

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->setDeviceInfo(m_device);

                    list<SupportedCommand *> *cmds = m_device->getSupportedCommands();
                    list<SupportedCommand *>::iterator it;
                    for (it = cmds->begin(); it != cmds->end(); it++) {
                        SupportedCommand *cmd = (SupportedCommand *)(*it);

                        if (cmd->getID() == "power") {
                            m_value = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.power", m_value);
                            printf("get power: %s\n", m_value.c_str());
                            break;
                        }
                    }

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                    // PUT request operations
                }
                else if (requestType == "POST")
                {
                    printf("requestType : POST\n");
                    // POST request operations
                    OCRepresentation    rep = request->getResourceRepresentation();
                    if (!setPowerRepresentation(rep)) {
                        fprintf(stderr, "failed to set representation\n");
                        return ehResult;
                    }

                    //control device
                    SupportedCommand cmd("power", m_value);
                    list<SupportedCommand > cmdList;
                    cmdList.push_back(cmd);
                    m_device->setCommands(cmdList);

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->controlDevice(m_device);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "DELETE")
                {
                    printf("requestType : DELETE\n");
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");

                ObservationInfo observationInfo = request->getObservationInfo();
                if (ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_interestedObservers.push_back(observationInfo.obsId);
                }
                else if (ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_interestedObservers.erase(remove(
                                                    m_interestedObservers.begin(),
                                                    m_interestedObservers.end(),
                                                    observationInfo.obsId),
                                                m_interestedObservers.end());
                }
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class MovementResource : public BaseResource
{
private:
    string m_value; //0-stop,1-up,2-back,3-left,4-right
    vector<string> m_keyNames;

public:
    MovementResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {
        m_value = "0";
        m_keyNames.push_back("x.com.jd.da.behavior");
        m_representation.setValue("x.com.jd.da.behavior", m_value);
        m_representation.setValue("key", m_keyNames);
    }

    int setMovementRepresentation(OCRepresentation &rep)
    {
        string value;
        if (rep.getValue("x.com.jd.da.behavior", value))
        {
            int val = stoi(value);
            if (val >= 0 && val <= 4) {
                m_value = value;
                m_representation.setValue("x.com.jd.da.behavior", m_value);
                printf("x.com.jd.da.behavior: %s\n", m_value.c_str());

                propagate();
                return 1;
            } else {
                fprintf(stderr, "invalid parameter!\n");
                return 0;
            }
        } else {
            fprintf(stderr, "x.com.jd.da.behavior is not found!!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("\tIn Server Movement entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                if (requestType == "GET")
                {
                    printf("requestType : GET\n");

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->setDeviceInfo(m_device);

                    list<SupportedCommand *> *cmds = m_device->getSupportedCommands();
                    list<SupportedCommand *>::iterator it;
                    for (it = cmds->begin(); it != cmds->end(); it++) {
                        SupportedCommand *cmd = (SupportedCommand *)(*it);

                        if (cmd->getID() == "Direction") {
                            m_value = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.behavior", m_value);
                            printf("get behavior: %s\n", m_value.c_str());
                            break;
                        }
                    }

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                    // PUT request operations
                }
                else if (requestType == "POST")
                {
                    printf("requestType : POST\n");
                    // POST request operations
                    OCRepresentation    rep = request->getResourceRepresentation();
                    if (!setMovementRepresentation(rep)) {
                        fprintf(stderr, "failed to set representation\n");
                        return ehResult;
                    }

                    //control device
                    SupportedCommand cmd("Direction", m_value);
                    list<SupportedCommand > cmdList;
                    cmdList.push_back(cmd);
                    m_device->setCommands(cmdList);

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->controlDevice(m_device);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "DELETE")
                {
                    printf("requestType : DELETE\n");
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");

                ObservationInfo observationInfo = request->getObservationInfo();
                if (ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_interestedObservers.push_back(observationInfo.obsId);
                }
                else if (ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_interestedObservers.erase(remove(
                                                    m_interestedObservers.begin(),
                                                    m_interestedObservers.end(),
                                                    observationInfo.obsId),
                                                m_interestedObservers.end());
                }
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class ModeResource : public BaseResource
{
private:
    string m_value; //0-stop work,1-auto work,2-local work,3-border work,4-planning work,5-sweeping floor work
    vector<string> m_keyNames;

public:
    ModeResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {
        m_value = "0";
        m_keyNames.push_back("x.com.jd.da.workmode");
        m_representation.setValue("x.com.jd.da.workmode", m_value);
        m_representation.setValue("key", m_keyNames);
    }

    int setModeRepresentation(OCRepresentation &rep)
    {
        string value;
        if (rep.getValue("x.com.jd.da.workmode", value))
        {
            int val = stoi(value);
            if (val >= 0 && val <= 5) {
                m_value = value;
                m_representation.setValue("x.com.jd.da.workmode", m_value);
                printf("x.com.jd.da.workmode: %s\n", m_value.c_str());

                propagate();
                return 1;
            } else {
                fprintf(stderr, "invalid parameter!\n");
                return 0;
            }
        } else {
            fprintf(stderr, "x.com.jd.da.workmode is not found!!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("\tIn Server Work Mode entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                if (requestType == "GET")
                {
                    printf("requestType : GET\n");

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->setDeviceInfo(m_device);

                    list<SupportedCommand *> *cmds = m_device->getSupportedCommands();
                    list<SupportedCommand *>::iterator it;
                    for (it = cmds->begin(); it != cmds->end(); it++) {
                        SupportedCommand *cmd = (SupportedCommand *)(*it);

                        if (cmd->getID() == "WorkMode") {
                            m_value = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.workmode", m_value);
                            printf("get work mode: %s\n", m_value.c_str());
                            break;
                        }
                    }

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                    // PUT request operations
                }
                else if (requestType == "POST")
                {
                    printf("requestType : POST\n");
                    // POST request operations
                    OCRepresentation    rep = request->getResourceRepresentation();
                    if (!setModeRepresentation(rep)) {
                        fprintf(stderr, "failed to set representation\n");
                        return ehResult;
                    }

                    //control device
                    SupportedCommand cmd("WorkMode", m_value);
                    list<SupportedCommand > cmdList;
                    cmdList.push_back(cmd);
                    m_device->setCommands(cmdList);

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->controlDevice(m_device);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "DELETE")
                {
                    printf("requestType : DELETE\n");
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");

                ObservationInfo observationInfo = request->getObservationInfo();
                if (ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_interestedObservers.push_back(observationInfo.obsId);
                }
                else if (ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_interestedObservers.erase(remove(
                                                    m_interestedObservers.begin(),
                                                    m_interestedObservers.end(),
                                                    observationInfo.obsId),
                                                m_interestedObservers.end());
                }
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class BatteryChargeResource : public BaseResource
{
private:
    string m_chargeStatus;
    string m_autoCharge;
    vector<string> m_keyNames;

public:
    BatteryChargeResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {
        m_chargeStatus = "0";
        m_autoCharge = "0";
        m_keyNames.push_back("x.com.jd.da.chargestatus");
        m_keyNames.push_back("x.com.jd.da.autocharge");
        m_representation.setValue("x.com.jd.da.chargestatus", m_chargeStatus);
        m_representation.setValue("x.com.jd.da.autocharge", m_autoCharge);
        m_representation.setValue("key", m_keyNames);
    }

    int setBatteryChargeRepresentation(OCRepresentation &rep)
    {
        string value;
        if (rep.getValue("x.com.jd.da.autocharge", value))
        {
            if (value == "0" || value == "1") {
                m_autoCharge = value;
                m_representation.setValue("x.com.jd.da.autocharge", m_autoCharge);
                printf("x.com.jd.da.autocharge: %s\n", m_autoCharge.c_str());

                propagate();
                return 1;
            } else {
                fprintf(stderr, "invalid parameter!\n");
                return 0;
            }
        } else {
            fprintf(stderr, "x.com.jd.da.autocharge is not found!!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("\tIn Server Battery Charge entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                if (requestType == "GET")
                {
                    printf("requestType : GET\n");

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->setDeviceInfo(m_device);

                    list<SupportedCommand *> *cmds = m_device->getSupportedCommands();
                    list<SupportedCommand *>::iterator it;
                    for (it = cmds->begin(); it != cmds->end(); it++) {
                        SupportedCommand *cmd = (SupportedCommand *)(*it);

                        if (cmd->getID() == "Status_Charging") {
                            m_chargeStatus = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.chargestatus", m_chargeStatus);
                            printf("get battery charge status: %s\n", m_chargeStatus.c_str());
                            break;
                        } else if (cmd->getID() == "Auto_Recharge") {
                            m_autoCharge = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.autocharge", m_autoCharge);
                            printf("get battery auto charge: %s\n", m_autoCharge.c_str());
                            break;
                        }
                    }

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                    // PUT request operations
                }
                else if (requestType == "POST")
                {
                    printf("requestType : POST\n");
                    // POST request operations
                    OCRepresentation    rep = request->getResourceRepresentation();
                    if (!setBatteryChargeRepresentation(rep)) {
                        fprintf(stderr, "failed to set representation\n");
                        return ehResult;
                    }

                    //control device
                    SupportedCommand cmd("Auto_Recharge", m_autoCharge);
                    list<SupportedCommand > cmdList;
                    cmdList.push_back(cmd);
                    m_device->setCommands(cmdList);

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->controlDevice(m_device);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "DELETE")
                {
                    printf("requestType : DELETE\n");
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");

                ObservationInfo observationInfo = request->getObservationInfo();
                if (ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_interestedObservers.push_back(observationInfo.obsId);
                }
                else if (ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_interestedObservers.erase(remove(
                                                    m_interestedObservers.begin(),
                                                    m_interestedObservers.end(),
                                                    observationInfo.obsId),
                                                m_interestedObservers.end());
                }
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class RvcResource : public BaseResource
{
private:

public:
    RvcResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {

    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("In Server RVC entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                // If the request type is GET
                if (requestType == "GET")
                {
                    printf("requestType : GET\n");
                    string findRes = request->getQueryParameters().find("if")->second;
                    if (findRes.compare(LINK_INTERFACE) == 0)
                    {
                        if (OC_STACK_OK == sendRepresentation(request))
                        {
                            ehResult = OC_EH_OK;
                        }
                    }
                    else
                    {
                        ehResult = OC_EH_FORBIDDEN;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                }
                else if (requestType == "POST")
                {
                    // POST request operations
                }
                else if (requestType == "DELETE")
                {
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class KidLockResource : public BaseResource
{
private:
    string m_lock;
    vector<string> m_keyNames;

public:
    KidLockResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {
        m_lock = "0";
        m_keyNames.push_back("x.com.jd.da.kidlock");
        m_representation.setValue("x.com.jd.da.kidlock", m_lock);
        m_representation.setValue("key", m_keyNames);
    }

    int setKidLockRepresentation(OCRepresentation &rep)
    {
        string lock;
        if (rep.getValue("x.com.jd.da.kidlock", lock))
        {
            m_lock = lock;
            m_representation.setValue("x.com.jd.da.kidlock", m_lock);
            printf("kid lock: %s\n", m_lock.c_str());

            propagate();
            return 1;
        } else {
            fprintf(stderr, "x.com.jd.da.kidlock is not found!!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("\tIn Server KidLock entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                if (requestType == "GET")
                {
                    printf("requestType : GET\n");

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->setDeviceInfo(m_device);

                    list<SupportedCommand *> *cmds = m_device->getSupportedCommands();
                    list<SupportedCommand *>::iterator it;
                    for (it = cmds->begin(); it != cmds->end(); it++) {
                        SupportedCommand *cmd = (SupportedCommand *)(*it);

                        if (cmd->getID() == "lock") {
                            m_lock = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.kidlock", m_lock);
                            printf("get oven kid lock: %s\n", m_lock.c_str());
                            break;
                        }
                    }

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                    // PUT request operations
                }
                else if (requestType == "POST")
                {
                    printf("requestType : POST\n");
                    // POST request operations
                    OCRepresentation    rep = request->getResourceRepresentation();
                    if (!setKidLockRepresentation(rep)) {
                        fprintf(stderr, "failed to set representation\n");
                        return ehResult;
                    }

                    //control device
                    SupportedCommand cmd("lock", m_lock);
                    list<SupportedCommand > cmdList;
                    cmdList.push_back(cmd);
                    m_device->setCommands(cmdList);

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->controlDevice(m_device);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "DELETE")
                {
                    printf("requestType : DELETE\n");
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");

                ObservationInfo observationInfo = request->getObservationInfo();
                if (ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_interestedObservers.push_back(observationInfo.obsId);
                }
                else if (ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_interestedObservers.erase(remove(
                                                    m_interestedObservers.begin(),
                                                    m_interestedObservers.end(),
                                                    observationInfo.obsId),
                                                m_interestedObservers.end());
                }
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class OvenOperationResource : public BaseResource
{
private:
    string m_state;     //state
    string m_menu;      //selected work menu(0-unselected, 1-upper heating, 2-lower heating, 3-upper&lower heating, 4-fermentation)
    string m_preheat;   //0-no preheat,1-preheat
    string m_work;      //0-work, 1-no work
    string m_worktemp;  //work temperature
    string m_uworktemp; //upper work temperature
    string m_dworktemp; //down work temperature
    string m_curtemp;   //current temperature
    string m_ucurtemp;  //upper temperature
    string m_dcurtemp;  //down temperature
    string m_worktime;  //work time in minutes
    string m_min_dctime;//minutes part of working time
    string m_sec_dctime;//second part of working time

    vector<string> m_keyNames;

public:
    OvenOperationResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {
        m_state = "0";
        m_min_dctime = "0";
        m_sec_dctime = "0";

        m_representation.setValue("x.com.jd.da.state", m_state);
        m_representation.setValue("x.com.jd.da.min_dctime", m_min_dctime);
        m_representation.setValue("x.com.jd.da.sec_dctime", m_sec_dctime);

        m_keyNames.push_back("x.com.jd.da.state");
        m_keyNames.push_back("x.com.jd.da.min_dctime");
        m_keyNames.push_back("x.com.jd.da.sec_dctime");

        m_representation.setValue("key", m_keyNames);
    }

    int setOvenOperationRepresentation(OCRepresentation &rep)
    {
        string menu;
        string work;
        
        if (rep.getValue("x.com.jd.da.menu", menu)) {
            if (!rep.getValue("x.com.jd.da.work", work)) {
                fprintf(stderr, "x.com.jd.da.work is not found!!!!!!!!!!!!!!!!!\n");
                return 0;
            }

            if (work == "0") {
                string preheat;
                string worktime;
                string worktemp;
                string uworktemp;
                string dworktemp;

                //start working
                if (menu == "3" &&
                    rep.getValue("x.com.jd.da.preheat", preheat) &&
                    rep.getValue("x.com.jd.da.worktime", worktime) &&
                    rep.getValue("x.com.jd.da.uworktemp", uworktemp) &&
                    rep.getValue("x.com.jd.da.dworktemp", dworktemp))
                {
                    m_preheat = preheat;
                    m_worktime = worktime;
                    m_uworktemp = uworktemp;
                    m_dworktemp = dworktemp;
                    m_representation.setValue("x.com.jd.da.preheat", m_preheat);
                    m_representation.setValue("x.com.jd.da.worktime", m_worktime);
                    m_representation.setValue("x.com.jd.da.uworktemp", m_uworktemp);
                    m_representation.setValue("x.com.jd.da.dworktemp", m_dworktemp);

                    printf("menu: %s, preheat: %s, worktime: %s, uworktemp: %s, dworktemp: %s\n", 
                            m_menu.c_str(), m_preheat.c_str(), m_worktime.c_str(), 
                            m_uworktemp.c_str(), m_dworktemp.c_str());
                }
                else if ((menu == "1" || menu == "2" || menu == "4") &&
                    rep.getValue("x.com.jd.da.preheat", preheat) &&
                    rep.getValue("x.com.jd.da.worktime", worktime) &&
                    rep.getValue("x.com.jd.da.worktemp", worktemp))
                {
                    m_preheat = preheat;
                    m_worktime = worktime;
                    m_worktemp = worktemp;
                    m_representation.setValue("x.com.jd.da.preheat", m_preheat);
                    m_representation.setValue("x.com.jd.da.worktime", m_worktime);
                    m_representation.setValue("x.com.jd.da.worktemp", m_worktemp);

                    printf("menu: %s, preheat: %s, worktime: %s, worktemp: %s\n", 
                        m_menu.c_str(), m_preheat.c_str(), m_worktime.c_str(), m_worktemp.c_str());
                }
                else
                {
                    fprintf(stderr, "invalid parameters!\n");
                    return 0;
                }

                m_menu = menu;
                m_work = work;
                m_representation.setValue("x.com.jd.da.menu", m_menu);
                m_representation.setValue("x.com.jd.da.work", m_work);
                printf("start working[%s]\n", m_menu.c_str());
            } else if (work == "1") {
                if (menu == "1" || menu == "2" || menu == "3" || menu == "4") {
                    //stop working
                    m_menu = menu;
                    m_work = work;
                    m_representation.setValue("x.com.jd.da.menu", m_menu);
                    m_representation.setValue("x.com.jd.da.work", m_work);
                    printf("stop working[%s]\n", m_menu.c_str());
                } else {
                    fprintf(stderr, "x.com.jd.da.menu is not valid!!!!!!!!!!!!!!!!!\n");
                    return 0;
                }
            } else {
                //invalid
                fprintf(stderr, "x.com.jd.da.work is not valid!!!!!!!!!!!!!!!!!\n");
                return 0;
            }

            propagate();
            return 1;
        } else {
            fprintf(stderr, "x.com.jd.da.menu is not found!!!!!!!!!!!!!!!!!\n");
            return 0;
        }
    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("\tIn Server OvenOperation entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                // If the request type is GET
                if (requestType == "GET")
                {
                    printf("requestType : GET\n");

                    m_keyNames.clear();

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->setDeviceInfo(m_device);

                    list<SupportedCommand *> *cmds = m_device->getSupportedCommands();
                    list<SupportedCommand *>::iterator it;

                    for (it = cmds->begin(); it != cmds->end(); it++) {
                        SupportedCommand *cmd = (SupportedCommand *)(*it);

                        if (cmd->getID() == "state") {
                            m_state = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.state", m_state);
                            printf("get oven state: %s\n", m_state.c_str());
                            m_keyNames.push_back("x.com.jd.da.state");
                        } else if (cmd->getID() == "menu") {
                            m_menu = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.menu", m_menu);
                            printf("get oven work menu: %s\n", m_menu.c_str());
                            m_keyNames.push_back("x.com.jd.da.menu");
                        } else if (cmd->getID() == "preheat") {
                            m_preheat = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.preheat", m_preheat);
                            printf("get oven preheat: %s\n", m_preheat.c_str());
                            m_keyNames.push_back("x.com.jd.da.preheat");
                        } else if (cmd->getID() == "work") {
                            m_work = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.work", m_work);
                            printf("get oven work: %s\n", m_work.c_str());
                            m_keyNames.push_back("x.com.jd.da.work");
                        } else if (cmd->getID() == "worktemp") {
                            m_worktemp = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.worktemp", m_worktemp);
                            printf("get oven work temp: %s\n", m_worktemp.c_str());
                            m_keyNames.push_back("x.com.jd.da.worktemp");
                        } else if (cmd->getID() == "uworktemp") {
                            m_uworktemp = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.uworktemp", m_uworktemp);
                            printf("get oven upper work temp: %s\n", m_uworktemp.c_str());
                            m_keyNames.push_back("x.com.jd.da.uworktemp");
                        } else if (cmd->getID() == "dworktemp") {
                            m_dworktemp = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.dworktemp", m_dworktemp);
                            printf("get oven lower work temp: %s\n", m_dworktemp.c_str());
                            m_keyNames.push_back("x.com.jd.da.dworktemp");
                        } else if (cmd->getID() == "curtemp") {
                            m_curtemp = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.curtemp", m_curtemp);
                            printf("get oven current temp: %s\n", m_curtemp.c_str());
                            m_keyNames.push_back("x.com.jd.da.curtemp");
                        } else if (cmd->getID() == "ucurtemp") {
                            m_ucurtemp = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.ucurtemp", m_ucurtemp);
                            printf("get oven upper current temp: %s\n", m_ucurtemp.c_str());
                            m_keyNames.push_back("x.com.jd.da.ucurtemp");
                        } else if (cmd->getID() == "dcurtemp") {
                            m_dcurtemp = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.dcurtemp", m_dcurtemp);
                            printf("get oven lower current  temp: %s\n", m_dcurtemp.c_str());
                            m_keyNames.push_back("x.com.jd.da.dcurtemp");
                        } else if (cmd->getID() == "worktime") {
                            m_worktime = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.worktime", m_worktime);
                            printf("get oven work time: %s\n", m_worktime.c_str());
                            m_keyNames.push_back("x.com.jd.da.worktime");
                        } else if (cmd->getID() == "min_dctime") {
                            m_min_dctime = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.min_dctime", m_min_dctime);
                            printf("get oven minute time: %s\n", m_min_dctime.c_str());
                            m_keyNames.push_back("x.com.jd.da.min_dctime");
                        } else if (cmd->getID() == "sec_dctime") {
                            m_sec_dctime = cmd->getStatus();
                            m_representation.setValue("x.com.jd.da.sec_dctime", m_sec_dctime);
                            printf("get oven second time: %s\n", m_sec_dctime.c_str());
                            m_keyNames.push_back("x.com.jd.da.sec_dctime");
                        }
                    }

                    m_representation.setValue("key", m_keyNames);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                    // PUT requeist operations
                }
                else if (requestType == "POST")
                {
                    printf("requestType : POST\n");
                    // POST request operations
                    OCRepresentation    rep = request->getResourceRepresentation();
                    if (!setOvenOperationRepresentation(rep)) {
                        fprintf(stderr, "failed to set representation\n");
                        return ehResult;
                    }

                    //control device
                    list<SupportedCommand > cmdList;
                    
                    SupportedCommand menuCmd("menu", m_menu);
                    SupportedCommand workCmd("work", m_work);
                    cmdList.push_back(menuCmd);
                    cmdList.push_back(workCmd);

                    if (m_work == "0") {
                        SupportedCommand preheatCmd("preheat", m_preheat);
                        SupportedCommand workTimeCmd("worktime", m_worktime);
                        cmdList.push_back(preheatCmd);
                        cmdList.push_back(workTimeCmd);

                        if (m_menu == "3") {
                            SupportedCommand uworkTempCmd("uworktemp", m_uworktemp);
                            SupportedCommand dworkTempCmd("dworktemp", m_dworktemp);
                            cmdList.push_back(uworkTempCmd);
                            cmdList.push_back(dworkTempCmd);
                        } else if ((m_menu == "1" || m_menu == "2" || m_menu == "4")) {
                            SupportedCommand workTempCmd("worktemp", m_worktemp);
                            cmdList.push_back(workTempCmd);
                        }
                    }

                    m_device->setCommands(cmdList);

                    JDHttpRequest *pJDHttpRequest = (JDHttpRequest *)m_requester;
                    pJDHttpRequest->controlDevice(m_device);

                    if (OC_STACK_OK == sendRepresentation(request))
                    {
                        ehResult = OC_EH_OK;
                    }
                }
                else if (requestType == "DELETE")
                {
                    printf("requestType : DELETE\n");
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");

                ObservationInfo observationInfo = request->getObservationInfo();
                if (ObserveAction::ObserveRegister == observationInfo.action)
                {
                    m_interestedObservers.push_back(observationInfo.obsId);
                }
                else if (ObserveAction::ObserveUnregister == observationInfo.action)
                {
                    m_interestedObservers.erase(remove(
                                                    m_interestedObservers.begin(),
                                                    m_interestedObservers.end(),
                                                    observationInfo.obsId),
                                                m_interestedObservers.end());
                }
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

class OvenResource : public BaseResource
{
private:

public:
    OvenResource(string uri, vector<string> rt, vector<string> itf)
        : BaseResource(uri, rt, itf)
    {

    }

    OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request)
    {
        printf("In Server Oven entity handler:\n");
        OCEntityHandlerResult ehResult = OC_EH_ERROR;

        if (request)
        {
            // Get the request type and request flag
            string requestType = request->getRequestType();
            int requestFlag = request->getRequestHandlerFlag();

            if (requestFlag & RequestHandlerFlag::RequestFlag)
            {
                printf("requestFlag : Request\n");

                // If the request type is GET
                if (requestType == "GET")
                {
                    printf("requestType : GET\n");
                    string findRes = request->getQueryParameters().find("if")->second;
                    if (findRes.compare(LINK_INTERFACE) == 0)
                    {
                        if (OC_STACK_OK == sendRepresentation(request))
                        {
                            ehResult = OC_EH_OK;
                        }
                    }
                    else
                    {
                        ehResult = OC_EH_FORBIDDEN;
                    }
                }
                else if (requestType == "PUT")
                {
                    printf("requestType : PUT\n");
                }
                else if (requestType == "POST")
                {
                    // POST request operations
                }
                else if (requestType == "DELETE")
                {
                    // DELETE request operations
                }
            }

            if (requestFlag & RequestHandlerFlag::ObserverFlag)
            {
                printf("requestFlag : Observer\n");
            }
        }
        else
        {
            printf("Request invalid\n");
        }

        return ehResult;
    }
};

#endif /* RESOURCE_SET_H_ */