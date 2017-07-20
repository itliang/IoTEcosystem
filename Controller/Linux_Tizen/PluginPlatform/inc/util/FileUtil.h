#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_


#include <minizip/unzip.h>

#define BUF_SIZE_MAX 4096
#define BUF_MARGIN 8

typedef enum {
	ERROR_NONE = 0,
	ERROR_FAILED,
	ERROR_STORAGE_FULL,
	ERROR_NOT_EXIST,
	ERROR_INVALID_PARAMETER,
	ERROR_PERMISSION_DENIED
}return_e;

class FileUtil{
public:
	static FileUtil *getInstance();
	static return_e isExistFile(char *file_path);
	static return_e unZipData(char *src_path, char *des_path);
	static return_e extractFileFromZip(unzFile unzf, char *file_path);
	static return_e createDirectory(char *directory_path);
private:
	FileUtil(void);
	static FileUtil *__pInstance;
};
#endif
