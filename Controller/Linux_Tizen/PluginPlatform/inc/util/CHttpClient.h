/**
 * @file     httpclient.h
 *
 * @brief
 *
 * @author  Android 1G
 *
 * @version Unspecified.
 */

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "Log.h"

#include <curl/curl.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

struct POST_DATA {
  const char *readptr;
  int sizeleft;
};

class HttpClient {
    public:
        HttpClient();
        ~HttpClient();
        void addPost(const char *payload);
        void addPut(const char *payload);
        void addHeaders(vector<string> headers);
        void setIptype(int iptype);
        void setTimeout(long int value);
        void setConnectTimeout(long int value);
        string fetchPage(const string &url,int usepost=0);
        string deletePage(const string &url);
        void setInterface(const char *intf);
        const string lastError();
        void destroyInstance();
        string getContents();
        vector<string> getHeader();
        CURLcode getCurlCode();
        long getStatusCode();
        void supportTLS();

    private:
        HttpClient& operator=(const HttpClient& c) { return *this; }
        CURL *handle;
        curl_httppost* post;
        curl_httppost* last;
        string writeBuffer;
        vector<string> headerBuffer;
        char errorBuffer[CURL_ERROR_SIZE];
        bool useCookies;
        struct curl_slist *mHeaders;
        CURLcode curl_code;
        long status_code;
        POST_DATA post_data;
};

#endif // __HTTP_CLIENT_H__

