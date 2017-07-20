#include "util/FileUtil.h"
#include "util/Log.h"
#include "glib.h"
#include "glib/gstdio.h"

FileUtil* FileUtil::__pInstance = new FileUtil;

FileUtil::FileUtil(){

}

FileUtil *FileUtil::getInstance(){
	return __pInstance;
}

return_e FileUtil::isExistFile(char *file_path)
{
	//FUNC_ENTER();

	RETVM_IF(file_path == NULL, ERROR_INVALID_PARAMETER, "file_path is NULL");
	WDEBUG("file_path = %s", file_path);
	if (g_file_test(file_path, G_FILE_TEST_EXISTS) == false) {
		WINFO("file is not exist=");
		return ERROR_NOT_EXIST;
	}

	//FUNC_EXIT();
	return ERROR_NONE;
}

return_e FileUtil::createDirectory(char *directory_path)
{
	//FUNC_ENTER();

	RETVM_IF(directory_path == NULL, ERROR_INVALID_PARAMETER, "directory_path is NULL");
	//DBG("directory_path = [%s]", directory_path);
	RETVM_IF(isExistFile(directory_path) == ERROR_NONE, ERROR_NONE, "directory is exist");

	RETVM_IF(g_mkdir_with_parents(directory_path, 0777) == -1, ERROR_FAILED, "directory create was failed, err = [%s]", strerror(errno));
	RETVM_IF(isExistFile(directory_path) != ERROR_NONE, ERROR_NOT_EXIST, "directory create was failed, directory is not exist.");

	//FUNC_EXIT();
	return ERROR_NONE;
}

return_e FileUtil::unZipData(char *src_path, char *dest_path)
{
	FUNC_BEGIN();
	WDEBUG("un zip data");
	RETVM_IF(src_path == NULL, ERROR_INVALID_PARAMETER, "src_path is NULL");
	RETVM_IF(dest_path == NULL, ERROR_INVALID_PARAMETER, "dest_path is NULL");
	RETVM_IF(isExistFile(src_path) != ERROR_NONE, ERROR_NOT_EXIST, "src_path is not exist");
	RETVM_IF(FileUtil::createDirectory(dest_path) != ERROR_NONE, ERROR_FAILED, "create directory failed");
	RETVM_IF(isExistFile(dest_path) != ERROR_NONE, ERROR_NOT_EXIST, "dest_path is not exist");
	WDEBUG("src_path = [%s], dest_path = [%s]", src_path, dest_path);

	unzFile unzf = NULL;
	unz_file_info file_info = {0, };
	char *target_file_path = NULL;
	char *p = NULL;
	int len = 0;
	int unz_err = 0;
	return_e ssm_err = ERROR_FAILED;

	/* Open zip file */
	unzf = unzOpen(src_path);
	GOTO_IF(unzf == NULL, "unzOpen() failed, unzf is NULL");

	unz_err = unzGoToFirstFile(unzf);
	GOTO_IF(unz_err != UNZ_OK, "unzGoToFirstFile() failed, unz_err = [%d]", unz_err);

	do {
		unz_err = unzOpenCurrentFile(unzf);
		if (unz_err != UNZ_OK) {
			WERROR("unzOpenCurrentFile() failed, unz_err = [%d]", unz_err);
			continue;
		}

		memset(&file_info, 0, sizeof(unz_file_info));

		unz_err = unzGetCurrentFileInfo(unzf, &file_info, NULL, 0, NULL, 0, NULL, 0);
		if (unz_err != UNZ_OK) {
			WERROR("unzGetCurrentFileInfo() failed, unz_err = [%d]", unz_err);
			continue;
		}

		len = strlen(dest_path) + file_info.size_filename + BUF_MARGIN;
		WDEBUG("len = [%d]", len);

		if (target_file_path != NULL) {
			free(target_file_path);
			target_file_path = NULL;
		}
		target_file_path = (char *)calloc(len, sizeof(char));
		GOTO_IF(target_file_path == NULL, "calloc() failed, target_file_path is NULL");

		snprintf(target_file_path, len, "%s/", dest_path);

		/* Go to the start of file name part */
		p = target_file_path + strlen(target_file_path);

		unz_err = unzGetCurrentFileInfo(unzf, &file_info, p, len - (p - target_file_path), NULL, 0, NULL, 0);
		if (unz_err != UNZ_OK) {
			WERROR("unzGetCurrentFileInfo() failed, unz_err = [%d]", unz_err);
			continue;
		}

		if (extractFileFromZip(unzf, target_file_path) != ERROR_NONE) {
			WDEBUG("extract file from zip error");
			continue;
		}
	} while (unzGoToNextFile(unzf) == UNZ_OK);


	ssm_err = ERROR_NONE;

return_part:
	if (target_file_path != NULL) {
		free(target_file_path);
		target_file_path = NULL;
	}

	unzClose(unzf);

	return ssm_err;
}

return_e FileUtil::extractFileFromZip(unzFile unzf, char *file_path)
{
	FUNC_BEGIN();
	//WDEBUG("extract file from zip");

	RETVM_IF(unzf == NULL, ERROR_INVALID_PARAMETER, "unzf is NULL");
	RETVM_IF(file_path == NULL, ERROR_INVALID_PARAMETER, "file_path is NULL");
	WDEBUG("file_path = [%s]", file_path);

#if 1
	char str[100]={'\0'};
	strcpy(str,file_path);
	WDEBUG("str is %s",str);

	char *dir = str;
	char *ptr = strrchr(dir,'/');
	*ptr = '\0';
	WDEBUG("dir is %s",dir);
	if(ERROR_NOT_EXIST == isExistFile(dir))
		createDirectory(dir);
#endif

	char *buf = NULL;
	FILE *file = NULL;
	unsigned int len = 0;
	return_e ssm_err = ERROR_FAILED;
	WDEBUG("before calloc");
	buf = (char *)calloc(BUF_SIZE_MAX, sizeof(char));
	WDEBUG("after calloc");
	//if(buf == NULL){
	//	WERROR("calloc error");
	//	return ERROR_FAILED;
	//}
	//RETVM_IF(buf == NULL, ERROR_FAILED, "calloc() failed, buf is NULL");
	WDEBUG("open file %s",file_path);
	/* Open the file */
	file = fopen(file_path, "wb");
	GOTO_IF(file == NULL, "fopen() failed, file is NULL");

	while ((len = unzReadCurrentFile(unzf, buf, BUF_SIZE_MAX)) > 0) {
		GOTO_IF(fwrite(buf, sizeof(char), len, file) != len, "fwrite() failed");
	}

	ssm_err = ERROR_NONE;

return_part:
	if (buf != NULL) {
		free(buf);
		buf = NULL;
	}

	if (file != NULL) {
		fclose(file);
		file = NULL;
	}

	FUNC_END();
	return ssm_err;
}
