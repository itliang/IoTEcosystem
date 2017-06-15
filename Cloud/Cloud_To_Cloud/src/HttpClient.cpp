#include "HttpClient.h"

HttpClient * HttpClient::s_instance = NULL;

HttpClient::HttpClient()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

HttpClient::~HttpClient()
{
	curl_global_cleanup();
}

HttpClient *HttpClient::getInstance()
{
    if (!s_instance)
    {
        if (!s_instance)
        {
            s_instance = new HttpClient();
        }
    }
    return s_instance;
}

void HttpClient::destroy()
{
    if (s_instance)
    {
    	delete s_instance;
    	s_instance = NULL;
    }
}

size_t HttpClient::onDebugReceivedCb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr)
{
	char logBuffer[1024] = {0};
	int logSize = 0;

	if (size < 1024) {
		logSize = size;
	} else {
		logSize = 1023;
	}

	if (type == CURLINFO_TEXT) {
		strncpy(logBuffer, data, logSize);
		logBuffer[logSize] = '\0';
		printf("[DEBUG] %s\n", logBuffer);
	} else if (type == CURLINFO_HEADER_IN || type == CURLINFO_HEADER_OUT) {
		//Ignore the body message.
		if (size >= 2 && data[0] == 0x0D && data[1] == 0x0A) {
			return 0;
		} else {
			strncpy(logBuffer, data, logSize);
			logBuffer[logSize] = '\0';
			printf("[DEBUG] %s\n", logBuffer);
		}
	}

	return 0;
}

int HttpClient::get(const string &base_uri, const string &params, HeadersMap &headers, ResponseCallBack cb, void *user_data)
{
	CURL *pCurl = curl_easy_init();
	if (!pCurl) {
		fprintf(stderr, "curl_easy_init() failed\n");
		return -1;
	}

	string uri;

	if (params.length() > 0) {
	    char *tmp = curl_easy_escape(pCurl, params.c_str(), params.length());
	    string params_encoded(tmp);
	    curl_free(tmp);
	    printf("encoded params: %s\n", params_encoded.c_str());

	    uri = base_uri + "?" + params_encoded;
	} else {
		uri = base_uri;
	}

	printf("uri: %s\n", uri.c_str());
	curl_easy_setopt(pCurl, CURLOPT_URL, uri.c_str());
	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(pCurl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
	curl_easy_setopt(pCurl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
#if defined (WITH_DEBUG)
	curl_easy_setopt(pCurl, CURLOPT_DEBUGFUNCTION, onDebugReceivedCb);
#endif
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "curl/7.35.0");

	if (cb)
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, cb);

	if (user_data)
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, user_data);

	//add HTTP header
	struct curl_slist *headerList = NULL;
    headerList = curl_slist_append(headerList, "Content-Type: application/x-www-form-urlencoded; charset=utf-8");

    char item[512] = {0};
    std::map<std::string, std::string>::iterator it;
    
    for (it = headers.begin(); it != headers.end(); ++it) {
    	snprintf(item, 511, "%s:%s", it->first.c_str(), it->second.c_str());
    	headerList = curl_slist_append(headerList, item);
    }

	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headerList);

	CURLcode res = curl_easy_perform(pCurl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}	

	curl_slist_free_all(headerList);
	curl_easy_cleanup(pCurl);
	return res;
}

int HttpClient::post(const string &base_uri, const string &post_data, HeadersMap &headers, ResponseCallBack cb, void *user_data)
{
	CURL *pCurl = curl_easy_init();
	if (!pCurl) {
		fprintf(stderr, "curl_easy_init() failed\n");
		return -1;
	}

	printf("uri: %s\n", base_uri.c_str());
	curl_easy_setopt(pCurl, CURLOPT_URL, base_uri.c_str());
	curl_easy_setopt(pCurl, CURLOPT_POST, 1L);
	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(pCurl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
	curl_easy_setopt(pCurl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

	printf("post data: %s\n", post_data.c_str());
	curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, post_data.c_str());
	curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, (long)post_data.length());

#if defined (WITH_DEBUG)
	curl_easy_setopt(pCurl, CURLOPT_DEBUGFUNCTION, onDebugReceivedCb);
#endif

	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "curl/7.35.0");

	if (cb)
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, cb);

	if (user_data)
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, user_data);

	//add HTTP header
	char content_length[64] = {0};
	snprintf(content_length, 63, "Content-Length: %d", (int )post_data.length());

	struct curl_slist *headerList = NULL;
    headerList = curl_slist_append(headerList, "Content-Type: application/x-www-form-urlencoded; charset=utf-8");
    headerList = curl_slist_append(headerList, content_length);

    char item[512] = {0};
    std::map<std::string, std::string>::iterator it;
    
    for (it = headers.begin(); it != headers.end(); ++it) {
    	snprintf(item, 511, "%s:%s", it->first.c_str(), it->second.c_str());
    	headerList = curl_slist_append(headerList, item);
    }

	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headerList);

	CURLcode res = curl_easy_perform(pCurl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}

	curl_slist_free_all(headerList);
	curl_easy_cleanup(pCurl);
	return res;
}

int HttpClient::post(const string &base_uri, ParamsMap &params, const string &post_data, HeadersMap &headers, ResponseCallBack cb, void *user_data)
{
	CURL *pCurl = curl_easy_init();
	if (!pCurl) {
		fprintf(stderr, "curl_easy_init() failed\n");
		return -1;
	}

	string uri = base_uri;

	if (!params.empty()) {
		uri = base_uri + "?";
		map<string, string>::const_iterator it;

		for (it = params.begin(); it != params.end(); it++) {
			string param = (string)(it->second);
			char *tmp = curl_easy_escape(pCurl, param.c_str(), param.length());
			string param_encoded(tmp);
			curl_free(tmp);

			string item = it->first + "=" + param_encoded + "&";
			uri += item;
		}

		uri.pop_back();
	}

	printf("uri: %s\n", uri.c_str());
	curl_easy_setopt(pCurl, CURLOPT_URL, uri.c_str());
	curl_easy_setopt(pCurl, CURLOPT_POST, 1L);
	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(pCurl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
	curl_easy_setopt(pCurl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
	
	printf("post data: %s\n", post_data.c_str());
	curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, post_data.c_str());
	curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, (long)post_data.length());

#if defined (WITH_DEBUG)
	curl_easy_setopt(pCurl, CURLOPT_DEBUGFUNCTION, onDebugReceivedCb);
#endif

	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(pCurl, CURLOPT_USERAGENT, "curl/7.35.0");

	if (cb)
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, cb);

	if (user_data)
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, user_data);

	//add HTTP header
	char content_length[64] = {0};
	snprintf(content_length, 63, "Content-Length: %d", (int )post_data.length());

	struct curl_slist *headerList = NULL;
    headerList = curl_slist_append(headerList, "Content-Type: application/x-www-form-urlencoded; charset=utf-8");
    headerList = curl_slist_append(headerList, content_length);

    char item[512] = {0};
    std::map<std::string, std::string>::iterator it;
    
    for (it = headers.begin(); it != headers.end(); ++it) {
    	snprintf(item, 511, "%s:%s", it->first.c_str(), it->second.c_str());
    	headerList = curl_slist_append(headerList, item);
    }

	curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headerList);

	CURLcode res = curl_easy_perform(pCurl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}

	curl_slist_free_all(headerList);
	curl_easy_cleanup(pCurl);
	return res;
}
