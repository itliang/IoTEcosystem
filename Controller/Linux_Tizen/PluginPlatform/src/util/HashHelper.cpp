#include "util/HashHelper.h"
#include <string>
#include "util/Log.h"
using namespace std;

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

int base64_encode(char *str,int str_len,char *encode,int encode_len){
    BIO *bmem,*b64;
    BUF_MEM *bptr;
    b64=BIO_new(BIO_f_base64());
    bmem=BIO_new(BIO_s_mem());
    b64=BIO_push(b64,bmem);
    BIO_write(b64,str,str_len); //encode
    BIO_flush(b64);
    BIO_get_mem_ptr(b64,&bptr);
    if(bptr->length>encode_len){
        printf("encode_len too small\n");
        return -1;
    }
    encode_len=bptr->length;
    memcpy(encode,bptr->data,bptr->length);
    BIO_free_all(b64);
    return encode_len;
}

char *getHashValue(char *time,char *appid){


	char value[100]={'\0'};
	snprintf(value,sizeof(value),"%s%s%s%s",appid,"SCP",time,"GALAXYAPPSAPI");
	WDEBUG("get hash value for %s",value);

	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char*)value, strlen(value), hash);

//	char encoded[100]={'\0'};
	char *encoded = (char *)calloc(45,sizeof(char));
	int len =  base64_encode((char *)hash, 32,encoded,100);

	WDEBUG("hash value is %s,len = %d",encoded,len);

	encoded[len-1] = '\0';
	return (char *)encoded;

}
