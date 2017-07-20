#ifndef __HASH_HELPER_H__
#define __HASH_HELPER_H__

#include <time.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <iconv.h>
#include <locale.h>

char *getHashValue(char *time,char *appid);
#endif
