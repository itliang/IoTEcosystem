#include "service/rest/HttpRequest.h"
#include "util/XmlParse.h"

HttpRequest::HttpRequest()
{
	mPluginInfoList = new list<PluginInfo *>();
	
	int ret = http_init();
	
	ret = http_session_create(HTTP_SESSION_MODE_PIPELINING, &mSession);
	
	if (ret != HTTP_ERROR_NONE){
	    WDEBUG("http_session_create failed: %d", ret);
	}		
}


HttpRequest::~HttpRequest()
{
	int ret = http_deinit();
	if (ret != HTTP_ERROR_NONE){
		 WDEBUG("http_deinit failed: %d", ret);
	}

	ret = http_session_destroy_all_transactions(mSession);
	if (ret != HTTP_ERROR_NONE){
		 WDEBUG("http_session_destroy_all_transactions failed: %d", ret);
	}
	
	ret = http_session_destroy(&mSession);
	if (ret != HTTP_ERROR_NONE){
		WDEBUG("http_session_destroy failed: %d", ret);
	}
}

void HttpRequest::request()
{
	WDEBUG("HttpRequest::request,Start .....................");
	int ret = http_session_open_transaction(mSession, HTTP_METHOD_GET, &mTransaction);
	if (ret != HTTP_ERROR_NONE){
		WDEBUG("http_session_open_transaction failed: %d", ret);
	}
	
	registerHttpCB();
	
	ret = http_transaction_request_set_uri(mTransaction,(char *)mUri.c_str());
	if (ret != HTTP_ERROR_NONE){
		WDEBUG("http_transaction_request_set_uri failed: %d", ret);
	}
	
	http_method_e method = HTTP_METHOD_GET;
	ret = http_transaction_request_set_method(mTransaction, method);
	if (ret != HTTP_ERROR_NONE){
		WDEBUG("http_transaction_request_set_method failed: %d", ret);
	}
	
	http_version_e version = HTTP_VERSION_1_1;
	ret = http_transaction_request_set_version(mTransaction, version);
	if (ret != HTTP_ERROR_NONE){
		WDEBUG("http_transaction_request_set_version failed: %d", ret);
	}
	
	ret = http_transaction_submit(mTransaction);
	if (ret != HTTP_ERROR_NONE){
		WDEBUG("http_transaction_submit failed: %d", ret);
	}


	WDEBUG("HttpRequest::request,End .......................");
}


void HttpRequest::registerHttpCB()
{
	http_transaction_set_received_body_cb(mTransaction, body_cb, this);
	http_transaction_set_received_header_cb(mTransaction, header_cb, this);
	http_transaction_set_aborted_cb(mTransaction, aborted_cb, this);
	http_transaction_set_uploaded_cb(mTransaction, uploaded_cb, this);
	http_transaction_set_completed_cb(mTransaction, completed_cb, this);
}
void HttpRequest::body_cb(http_transaction_h transaction, char* body, size_t size, size_t nmemb, void* user_data) 
{
	WDEBUG("HttpRequest:: body_cb %s",body);

	HttpRequest *pThis = (HttpRequest *)user_data;
	xml_parse_type(pThis->mRequestType);
	xml_parse_response(pThis->mPluginInfo,body);
}

void HttpRequest::header_cb(http_transaction_h transaction, char* header, size_t header_len, void* user_data)
{
	WDEBUG("HttpRequest:: header_cb");
}


void HttpRequest::uploaded_cb(http_transaction_h http_transaction, int recommended_chunk_size, void *user_data)
{
	WDEBUG("HttpRequest:: uploaded_cb");
}

void HttpRequest::completed_cb(http_transaction_h transaction,  void* user_data)
{
	WDEBUG("HttpRequest:: completed_cb");

	HttpRequest *pThis = (HttpRequest *)user_data;
	int ret = HTTP_ERROR_NONE;
    http_status_code_e status_code = HTTP_STATUS_UNDEFINED;
	
    ret = http_transaction_response_get_status_code(transaction, &status_code);
    if (ret != HTTP_ERROR_NONE){
	     WDEBUG("http_transaction_response_get_status_code failed: %d", ret);
	}
    
	WDEBUG("HttpRequest:: completed_cb,status = %d",status_code);

	if(status_code != HTTP_STATUS_OK )
	{
		pThis->mCallback->onFailure(pThis->mPluginInfo,OPERATION_ERROR);
		
	}else{

		if(pThis->mRequestType ==  MSG::START_FIND){
			pThis->mCallback->onSuccess(pThis->mPluginInfo,PLUGIN_METADATA_FOUND);
		}else if(pThis->mRequestType ==  MSG::START_DOWNLOAD_URI){
			pThis->mCallback->onProgress(pThis->mPluginInfo,CONTENTS_DOWNLOAD_READY,0);
		}
	}
}

void HttpRequest::aborted_cb(http_transaction_h http_transaction, http_error_code_e error, void *user_data)
{
	WDEBUG("HttpRequest:: aborted_cb");

	HttpRequest *pThis = (HttpRequest *)user_data;
	int ret = HTTP_ERROR_NONE;
    http_status_code_e status_code = HTTP_STATUS_UNDEFINED;
	
    ret = http_transaction_response_get_status_code(http_transaction, &status_code);
    if (ret != HTTP_ERROR_NONE){
	     WDEBUG("http_transaction_response_get_status_code failed: %d", ret);
	}

	WDEBUG("HttpRequest:: aborted_cb,status = %d",status_code);


	if(status_code != HTTP_STATUS_OK )
	{
		pThis->mCallback->onFailure(pThis->mPluginInfo,OPERATION_ERROR);
		
	}
}
