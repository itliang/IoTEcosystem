/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LOG_H__
#define __LOG_H__

#ifndef LOG_TAG
#define LOG_TAG "PluginPlatform"
#endif

#include <string.h>
#include <assert.h>
#include <stdexcept>
#include <sstream>

// If it is set as 1, assert() is called in WERROR
#define WENABLE_ASSERT_IN_ERROR (0)

#define WCOLOR_RED "\033[0;31m"
#define WCOLOR_GREEN "\033[0;32m"
#define WCOLOR_BROWN "\033[0;33m"
#define WCOLOR_BLUE "\033[0;34m"
#define WCOLOR_PURPLE "\033[0;35m"
#define WCOLOR_CYAN "\033[0;36m"
#define WCOLOR_LIGHTBLUE "\033[0;37m"
#define WCOLOR_END		"\033[0;m"
#define COLOR_RED			"\033[0;40;31m"
#define COLOR_LIGHTBLUE	"\033[1;40;34m"
#define COLOR_YELLOW		"\033[1;40;33m"
#define COLOR_END			"\033[0;m"
#define COLOR_GRAY		"\033[1;40;30m"
#define COLOR_MAGENTA	"\033[1;35m"
#define COLOR_NORMAL		"\033[0m"

// for build test, insert log.
//#define WUSING_PUBLIC_SDK

//#ifdef WUSING_PUBLIC_SDK
// normal log
#define _WFILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/')+1 : __FILE__)

#define _WDLOG_PRINT( prio, fmt, args... ) \
	g_print("%s: %s(%d) > " fmt, _WFILE_NAME, __func__, __LINE__, ##args)

#define WDEBUG(fmt, args...) _WDLOG_PRINT(DLOG_DEBUG, fmt, ##args)
#define WDEBUG_(fmt, args...) _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_RED fmt WCOLOR_END, ##args) // temporal red-colored text for debugging

#define WVERBOSE(fmt, args...) _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_BROWN fmt WCOLOR_END, ##args)
#define WINFO(fmt, args...) _WDLOG_PRINT(DLOG_INFO, WCOLOR_GREEN fmt WCOLOR_END, ##args)
#define WWARNING(fmt, args...) _WDLOG_PRINT(DLOG_WARN, WCOLOR_PURPLE fmt WCOLOR_END, ##args)
//
#if (WENABLE_ASSERT_IN_ERROR == 1)
#define WERROR(fmt, args...) do { \
			_WDLOG_PRINT(DLOG_ERROR, WCOLOR_RED "* Critical * " fmt WCOLOR_END, ##args); \
			assert(0);	\
		} while(0)
#else
#define WERROR(fmt, args...) _WDLOG_PRINT(DLOG_ERROR, WCOLOR_RED "* Critical * " fmt WCOLOR_END, ##args)
#endif

#define WHIT() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_BLUE ">>>HIT<<<" WCOLOR_END)
#define WHIT_() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_RED ">>>HIT<<<" WCOLOR_END)
#define WHITM(fmt, args... ) _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_BLUE ">>>HIT<<<" fmt WCOLOR_END, ##args)

#define WENTER() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_GREEN "BEGIN >>>>" WCOLOR_END)
#define WLEAVE() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_GREEN "END <<<<" WCOLOR_END)

// secure log
#define WSDEBUG(fmt, args...)
#define WSVERBOSE(fmt, args...)
#define WSINFO(fmt, args...)
//
#define WSERROR(fmt, args...)

#define LOG_PREFIX "Plugin_prefix "


#define THROW_RUNTIME_EXCEPTION(message) \
	std::stringstream ss;\
	ss << "[" << __FUNCTION__ << "]" << message;\
	throw std::runtime_error(ss.str())

#define RETM_IF(expr, fmt, arg...) \
	{ \
		if (expr) \
		{ \
			WERROR(fmt, ##arg); \
			return; \
		} \
	}
#define RETVM_IF(expr, val, fmt, arg...) \
{ \
    if (expr) { \
        WERROR(fmt, ##arg); \
        return (val); \
    } \
}

#define GOTO_IF(expr, fmt, arg...) do { \
	if (expr) { \
		WERROR(fmt, ##arg); \
		goto return_part; \
	} \
} while (0)


#define FUNC_BEGIN() \
	do {\
		g_print("%s: %s(%d) > "  COLOR_LIGHTBLUE"" LOG_PREFIX" BEGIN >>>>" COLOR_END, __MODULE__, __func__, __LINE__);\
	} while (0)

#define FUNC_END() \
	do {\
		g_print("%s: %s(%d) > "COLOR_LIGHTBLUE"" LOG_PREFIX" END <<<<" COLOR_END, __MODULE__, __func__, __LINE__);\
	} while (0)


#define IMSG_CHECK()\
	do {\
		g_print("%s: %s(%d) > "COLOR_MAGENTA"" LOG_PREFIX" ###" COLOR_END, __MODULE__, __func__, __LINE__);\
	} while (0)

#define ASSERT(expr) \
    if (!(expr)) \
    {   \
        g_print("Assert [%s] fail", #expr); \
    } \
    assert(expr)
#endif /* __LOG_H__ */
