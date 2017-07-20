/*
 * Copyright (c) 2000-2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance with the terms of the license agreement
 * you entered into with SAMSUNG ELECTRONICS.
 * SAMSUNG make no representations or warranties about the suitability
 * of the software, either express or implied, including but not
 * limited to the implied warranties of merchantability, fitness for
 * a particular purpose, or non-infringement.
 * SAMSUNG shall not be liable for any damages suffered by licensee as
 * a result of using, modifying or distributing this software or its derivatives.

 */

#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

typedef long long TimeMS;

typedef unsigned char byte;

#define INVALID_REQUEST_ID  -1

// Macro to get the value in byte
#define GET_MASK(val, pos, msk) (((val) & ((msk) << (pos))) >> (pos))
// Macro to put the value in byte
#define PUT_MASK(var, val, pos) ((var) | ((val) << (pos)))

#ifndef Handle
typedef int Handle;
#define INVALID_HANDLE  -1
#endif // Handle

#ifndef null
#define null    0
#endif // null

#ifndef INFINITE
#define INFINITE    0xFFFFFFFF
#endif // INFINITE

#endif // _DATA_TYPE_H_
