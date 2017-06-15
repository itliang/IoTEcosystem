#ifndef BASE_RESOURCE_H_
#define BASE_RESOURCE_H_

#include "IHttpRequest.h"

#include "OCApi.h"
#include "OCPlatform.h"

using namespace OC;
using namespace std;

class BaseResource
{
public:
    OCResourceHandle m_handle;
    BaseResource(string uri, vector<string> rt, vector<string> itf)
    {
        m_device = nullptr;
        m_requester = nullptr;

        m_representation.setUri(uri);
        m_representation.setResourceTypes(rt);
        m_representation.setResourceInterfaces(itf);
    }

    void setDevice(Device *dev)
    {
        m_device = dev;
    }

    void setHttpRequester(IHttpRequest *req)
    {
        m_requester = req;
    }

    string getResourceUri()
    {
        return m_representation.getUri();
    }

    vector<string> getResourceType()
    {
        return m_representation.getResourceTypes();
    }

    vector<string> getInterfaces()
    {
        return m_representation.getResourceInterfaces();
    }

    OCRepresentation getRepresentation(void)
    {
        m_representation.clearChildren();
        for (auto it = m_childResources.begin(); it != m_childResources.end(); it++)
        {
            m_representation.addChild((*it)->getRepresentation());
        }
        return m_representation;
    }

    vector<BaseResource *> getChildResource()
    {
        return m_childResources;
    }

    OCStackResult addChildResource(BaseResource *childResource)
    {
        m_childResources.push_back(childResource);
        return OCPlatform::bindResource(m_handle, childResource->m_handle);
    }

    OCStackResult sendRepresentation(shared_ptr<OCResourceRequest> pRequest)
    {
        auto pResponse = make_shared<OC::OCResourceResponse>();
        pResponse->setRequestHandle(pRequest->getRequestHandle());
        pResponse->setResourceHandle(pRequest->getResourceHandle());

        // Check for query params (if any)
        QueryParamsMap queryParamsMap = pRequest->getQueryParameters();

        printf("query params: \n");
        for (auto it = queryParamsMap.begin(); it != queryParamsMap.end(); it++)
        {
            printf("%s: %s", it->first.c_str(), it->second.c_str());
        }

        auto findRes = queryParamsMap.find("if");

        if (findRes != queryParamsMap.end())
        {
            pResponse->setResourceRepresentation(getRepresentation(), findRes->second);
        }
        else
        {
            pResponse->setResourceRepresentation(getRepresentation(), DEFAULT_INTERFACE);
        }

        pResponse->setErrorCode(200);
        pResponse->setResponseResult(OC_EH_OK);

        return OCPlatform::sendResponse(pResponse);
    }

    OCStackResult propagate()
    {
        if (m_interestedObservers.size() > 0)
        {
            shared_ptr<OCResourceResponse> resourceResponse =
            { make_shared<OCResourceResponse>() };

            resourceResponse->setErrorCode(200);
            resourceResponse->setResourceRepresentation(getRepresentation(), DEFAULT_INTERFACE);

            return OCPlatform::notifyListOfObservers(m_handle,
                    m_interestedObservers,
                    resourceResponse);
        }

        return OC_STACK_OK;
    }

    virtual OCEntityHandlerResult entityHandler(shared_ptr<OCResourceRequest> request) = 0;

protected:
    Device              *m_device;
    IHttpRequest        *m_requester;
    OCRepresentation    m_representation;
    vector<BaseResource *>  m_childResources;
    ObservationIds      m_interestedObservers;
};

#endif /* BASE_RESOURCE_H_ */