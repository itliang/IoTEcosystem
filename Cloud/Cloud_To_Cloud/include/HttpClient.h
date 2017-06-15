#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_

#include <curl/curl.h>
#include "RsTypes.h"

using namespace std;

class HttpClient
{
public:
    static HttpClient *getInstance();
    static void destroy();

	int get(const string &base_uri, 
			const string &params, 
			HeadersMap &headers, 
			ResponseCallBack cb, 
			void *user_data);

	int post(const string &base_uri, 
			const string &post_data, 
			HeadersMap &headers, 
			ResponseCallBack cb, 
			void *user_data);

	int post(const string &base_uri, 
			ParamsMap &params, 
			const string &post_data, 
			HeadersMap &headers, 
			ResponseCallBack cb, 
			void *user_data);
private:
	HttpClient();
	~HttpClient();

	static size_t onDebugReceivedCb(CURL *handle, 
									curl_infotype type, 
									char *data, 
									size_t size, 
									void *userptr);

private:
	static HttpClient *s_instance;
};

#endif /* HTTP_CLIENT_H_ */