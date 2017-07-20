
#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_


#include "galaxyapps/PluginFileRequest.h"
#include "util/Log.h"
#include "http.h"
#include <string>
#include <list>

class HttpRequest{
public:
		HttpRequest();
		~HttpRequest();

		void request();
		void setPluginInfo(PluginInfo *info){mPluginInfo = info;}
		void setRequestType(int type){mRequestType = type;}
		void setUri(string uri){mUri = uri;}
		void setResponseCallback(PluginTaskCallback *callback){mCallback = callback;}
		int mRequestType ;
private:

		string mUri;
		
		http_session_h mSession;
		http_transaction_h mTransaction;
		PluginInfo *mPluginInfo;
		list<PluginInfo *> *mPluginInfoList;
		PluginTaskCallback *mCallback;
		void registerHttpCB();
		static void body_cb(http_transaction_h transaction, char* body, size_t size, size_t nmemb, void* user_data);
		static void header_cb(http_transaction_h transaction, char* header, size_t header_len, void* user_data);
		static void uploaded_cb(http_transaction_h http_transaction, int recommended_chunk_size, void *user_data);
		static void completed_cb(http_transaction_h transaction, void* user_data);
		static void aborted_cb(http_transaction_h http_transaction, http_error_code_e error, void *user_data);
};


#endif

