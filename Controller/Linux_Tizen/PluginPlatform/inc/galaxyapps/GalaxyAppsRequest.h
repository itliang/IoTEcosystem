#ifndef __GALAXY_APPS_REQUEST_H__
#define __GALAXY_APPS_REQUEST_H__

#include <memory>
#include <map>
#include <string>

using namespace std;
/*
namespace HttpContentNs
{
    const string HTTPS_SCHEME("https");
    const string HTTP_SCHEME("http");
    const string HEADER_FIELD_NONE("none");
    const string HEADER_FIELD_WWW_AUTH("www-authenticate");
    const string HEADER_FIELD_ETAG("etag");
    const string HEADER_REALM("realm");
    const string HEADER_NONCE("nonce");
    const string HEADER_NEXTNONCE("nextnonce");
    const string HEADER_ALGORITHM("algorithm");
    const string HEADER_QOP("qop");
    const string HEADER_OPAQUE("opaque");

    const string HEADER_ETAG_LC("etag");
    const string HEADER_RETRY_LC("retry-after");
    const string HEADER_AUTH_INFO_LC("authentication-info");
    const string HEADER_CONTENT_TYPE("Content-Type: ");
    const string HEADER_HOST("Host: ");
    const string HEADER_IF_MATCH("If-Match: ");
    const string HEADER_CLIENT_UID("X-Client-UID: ");
    const string HEADER_ACCESS_TOKEN("access_token: ");
    const string HEADER_3GPP_ASSERTED_IDENTITY("X-3GPP-Asserted-Identity: ");

    const string CONTENT_TYPE_URLENCODED("application/x-www-form-urlencoded");

    // 3GPP TS 33.220 v12.3.0 GAA; GBA
    // H.3 (0x01, 0x00, 0x01, yy, zz) Ua security protocol
    // RFC 5246 A.5. The Cipher Suite
    const unsigned char UA_PROTOCOL_IDENTIFIER_CHAR[5] = {0x01, 0x00, 0x01, 0x00, 0x2F};
    const string UA_PROTOCOL_IDENTIFIER(UA_PROTOCOL_IDENTIFIER_CHAR, UA_PROTOCOL_IDENTIFIER_CHAR+5);

    static const string GBA_UICC("gba-u");
    static const string GBA_ME("gba-me");

    enum HttpMethodType {
        GET = 0,
        PUT = 1,
        POST = 2,
        DELETE = 3,
    };

    typedef struct HttpSessionInfo {
        int rid;                        //Request Id
        int sid;                        //Session Id
        std::string xui;
        std::string digestUri;
        std::string nonce;
        std::string algorithm;
        std::string qop;
        std::string opaque;
        std::string realm;
        std::string cnonce;
        std::string nc;
        std::string username;   // header value, after gba auth or MD5 auth it will be used
        std::string eTag;
        std::string response;   // md5 auth
        std::string method;
        std::string gbaKey;
        std::string nafFqdn;
        int retryCount;

        HttpSessionInfo(int rid, int sid,
                const std::string &method, const std::string &resourceUri,
                const std::string &nafFqdn, const std::string &xui,
                const std::string &realm, const std::string &username,
                const std::string &passwd = "", const std::string &nonce = "",
                const std::string &algorithm = "", const std::string &qop = "")
        : rid(rid), sid(sid), method(method), digestUri(resourceUri),
          nafFqdn(nafFqdn), xui(xui), realm(realm), username(username),
          gbaKey(passwd), nonce(nonce), algorithm(algorithm), qop(qop), retryCount(0) {}

        void reset() {
            nonce = "";
            algorithm = "";
            qop = "";
            opaque = "";
            cnonce = "";
            nc = "";
            response = "";
        }
    } HttpSessionInfo;
}
*/
namespace GalaxyAppsQueryRequest
{
    const string URL_PREFIX("http://vas.samsungapps.com");
    const string HTTPS_URL_PREFIX("https://vas.samsungapps.com");

    const int PROCESS_ERROR = 1000;
    const int REQUEST_PARSING_FAIL = 1001;
    const int SERVICE_ERROR = 2000;

    const int OK = 0;
    const int REQUEST_NO_DATA = 1;

    const int APPLICATION_NOT_AVAILABLE = 0;
    const int NOT_NECESSARY_TO_UPDATE = 1;
    const int UPDATE_AVAILABLE = 2;

    const int DOWNLOAD_URI_NOT_AVAILABLE = 0;
    const int DOWNLOAD_URI_AVAILABLE = 1;
}

namespace MSG
{
	const int START_FIND = 0;
	const int ERROR_FIND = 1;
	const int RETRY_FIND = 2;
	const int ERROR_UPDATE = 3;
	const int ERROR_GET_DOWNLOADURL = 5;
	const int END_GET_DOWNLOADURL = 7;
	const int UPDATE_NOT_NEEDED = 8;
	const int START_DOWNLOAD_URI = 9;
	const int START_DOWNLOAD = 10;
	const int START_UPDATE = 10;
	const int ERROR_DOWNLOAD = 14;
	const int END_DOWNLOAD = 15;
	const int EXIT = 16;
}
#endif
