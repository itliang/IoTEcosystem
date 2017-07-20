/**
 * @file    httpclient.cpp
 *
 * @brief
 *
 * @author  Android 1G
 *
 * @version Unspecified.
 */

#include "util/CHttpClient.h"

size_t bodyWriter(const char *data, size_t size, size_t nmemb, string *buffer)
{
    size_t result = 0;
    if (buffer != NULL) {
        WDEBUG("bodyWriter size, nmemb: %d, %d\n", static_cast<int>(size), static_cast<int>(nmemb));
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

size_t headerWriter(const char *data, size_t size, size_t nmemb, vector<string> *buffer)
{
    size_t result = 0;
    if (buffer != NULL) {
        buffer->push_back(string(data, size * nmemb));
        WINFO("headerWriter size, nmemb: %d, %d\n", static_cast<int>(size), static_cast<int>(nmemb));
        WINFO("headerWriter : %s\n", buffer->at(buffer->size() - 1).c_str());
        result = size * nmemb;
    }
    return result;
}

size_t postdata_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  POST_DATA *pooh = (POST_DATA *)userp;

  WINFO("postdata_callback: %d, %d\n", static_cast<int>(size), static_cast<int>(nmemb));

  if(size * nmemb < 1)
    return 0;

  size_t curl_size = nmemb * size;
  size_t to_copy = (pooh->sizeleft < curl_size) ? pooh->sizeleft : curl_size;
  memcpy(ptr, pooh->readptr, to_copy);
  pooh->sizeleft -= to_copy;
  pooh->readptr += to_copy;
  WINFO("postdata_callback to_copy: %d\n", static_cast<int>(to_copy));
  return to_copy;
}

void debug_trace(CURL *handle, curl_infotype type,
              char *data, size_t size,
              void *userp)
{
   const char *text;
   switch (type) {
      case CURLINFO_TEXT:
          text = "== Info";
          break;
      case CURLINFO_HEADER_OUT:
         text = "=> Send header";
         break;
       case CURLINFO_DATA_OUT:
         text = "=> Send data";
         break;
       case CURLINFO_SSL_DATA_OUT:
         text = "=> Send SSL data";
         break;
       case CURLINFO_HEADER_IN:
         text = "<= Recv header";
         break;
       case CURLINFO_DATA_IN:
         text = "<= Recv data";
         break;
       case CURLINFO_SSL_DATA_IN:
         text = "<= Recv SSL data";
         break;
       default:
           text = "== unknown type";
           break;
   }

   WINFO("%s : %s", text, data);
}

HttpClient::~HttpClient() {
    curl_easy_cleanup(handle);
    if (post != NULL) {
        free(post);
    }
    if (last != NULL) {
        free(last);
    }
    mHeaders = NULL;
}

void HttpClient::addPost(const char *payload) {
    post_data.readptr = payload;
    post_data.sizeleft = strlen(payload);
    curl_easy_setopt(handle, CURLOPT_POST, true);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, postdata_callback);
    curl_easy_setopt(handle, CURLOPT_READDATA, &post_data);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, post_data.sizeleft);
}

void HttpClient::addPut(const char *payload) {
    post_data.readptr = payload;
    post_data.sizeleft = strlen(payload);
    curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, postdata_callback);
    curl_easy_setopt(handle, CURLOPT_READDATA, &post_data);
    curl_easy_setopt(handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t) post_data.sizeleft);
    WINFO("addPut payload length: %d\n", post_data.sizeleft);
}

void HttpClient::setIptype(int iptype)
{
    curl_easy_setopt(handle, CURLOPT_IPRESOLVE, iptype);
}

void HttpClient::setInterface(const char *intf)
{
    curl_easy_setopt(handle, CURLOPT_INTERFACE, intf);
}

void HttpClient::setTimeout(long int timeout) {
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
}

void HttpClient::setConnectTimeout(long int timeout) {
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, timeout);
}

void HttpClient::addHeaders(vector<string> headers)
{
    for(auto &i : headers) {
        mHeaders = curl_slist_append(mHeaders, (char*)&(i[0]));
    }
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, mHeaders);
}

void HttpClient::supportTLS()
{
    curl_easy_setopt(handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
}

string HttpClient::fetchPage(const string &url,int usepost)
{
    if(usepost != 0) {
        curl_easy_setopt(handle, CURLOPT_HTTPPOST, post);
    }
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

    if (string::npos != url.find("https"))
        supportTLS();

    curl_code = curl_easy_perform (handle);
    curl_easy_getinfo (handle, CURLINFO_RESPONSE_CODE, &status_code);

    if(curl_code == CURLE_OK) {
        if(usepost != 0) {
            curl_formfree(post);
        }
        return writeBuffer;
    } else {
        WERROR("CURL Error :  curl code = %d errorBuffer = %s \n", curl_code, errorBuffer);
        return "";
    }
}

string HttpClient::deletePage(const string &url)
{
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_code = curl_easy_perform (handle);
    curl_easy_getinfo (handle, CURLINFO_RESPONSE_CODE, &status_code);

    if(curl_code == CURLE_OK) {
        return writeBuffer;
    } else {
        return "";
    }
}

const string HttpClient::lastError()
{
    return errorBuffer;
}

string HttpClient::getContents()
{
    return writeBuffer;
}

vector<string> HttpClient::getHeader()
{
    return headerBuffer;
}

CURLcode HttpClient::getCurlCode()
{
    return curl_code;
}

long HttpClient::getStatusCode()
{
    return status_code;
}

HttpClient::HttpClient()
{
    /* Initiate libcurl */
//    curl_global_init(CURL_GLOBAL_ALL);
//    curl_global_cleanup();
    /* Get connection handle */
    handle = curl_easy_init();
    /* Set all standard options */
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, &errorBuffer);
    curl_easy_setopt(handle, CURLOPT_HEADER, 0L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, bodyWriter);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &writeBuffer);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, headerWriter);
    curl_easy_setopt(handle, CURLOPT_WRITEHEADER, &headerBuffer);
    //curl_easy_setopt(handle, CURLOPT_CAINFO, "/data/data/com.sec.imsservice/files/certs/root_cert_CAfile.pem");
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
//    curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L);
//    curl_easy_setopt(handle, CURLOPT_DEBUGFUNCTION, debug_trace);
    /* NULL out post variables */
    post = NULL;
    last = NULL;
    errorBuffer[0]='\0';
    writeBuffer = "";
    headerBuffer.clear();
    mHeaders = NULL;
    useCookies = false;
    curl_code = CURLE_OK;
    status_code = 0;
    post_data.readptr = NULL;
    post_data.sizeleft = 0;
}
