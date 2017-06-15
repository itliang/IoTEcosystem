#ifndef RS_TYPES_H_
#define RS_TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <list>
#include <map>
#include <functional>
#include <algorithm>

/**
 * Declares Results & Errors.
 */
typedef enum
{
    RS_OK = 0,
    RS_INVALID_PARAMETER,
    RS_NETWORK_ERROR,
    RS_GET_SIGNATURE_ERROR,
    RS_SIGN_UP_ERROR,
    RS_SIGN_IN_ERROR,
    RS_SIGN_OUT_ERROR,
    RS_BIND_DEVICE_ERROR,
    RS_GET_DEVICES_ERROR,
    RS_RESOURCE_REGISTER_ERROR,
    RS_DEVICE_INFO_REGISTER_ERROR,
    RS_BIND_RESOURCE_INTERFACE_ERROR,
    RS_PUBLISH_RESOURCE_TO_RD_ERROR,
    RS_DELETE_RESOURCE_FROM_RD_ERROR,
} RSCode;

/**
 * Declares Authorization Type.
 */
typedef enum
{
    AUTH_CODE = 0,
    AUTH_IMPLICIT,
    AUTH_CLIENT_CREDENTIALS,
    AUTH_RESOURCE_OWNER_PASSWORD_CREDENTIALS,
} Auth_Type;


typedef std::map<std::string, std::string> ParamsMap;

typedef std::map<std::string, std::string> HeadersMap;

typedef size_t (*ResponseCallBack)(char *ptr, size_t size, size_t nmemb, void *userdata);

#define JD_APP_KEY          "8730968236F4570F95D361CE290587D2" 
#define JD_APP_SECRET       "cc3eab64f77f484ba315735bf35182da"
#define WULIAN_USER_ID      "samsung" 
#define WULIAN_USER_PWD     "123456"

/**
 * product uuid, 
 * resource uri, 
 * resource type
 */
// const std::string ocf_table[][3] = {
//     {"USPVGB", "/power/vs", "x.com.jd.da.operation"},
//     {"RVC", "/jd/oven", "x.com.jd.da.oven"},
// };

#endif /* RS_TYPES_H_ */