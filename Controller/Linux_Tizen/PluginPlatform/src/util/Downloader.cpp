/*
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//#include "debug.h"
#include "util/Log.h"
#include "util/Downloader.h"
//#include "sk-notification.h"
//#include "utility.h"
#include <glib.h>
#include <stdio.h>
#include <vconf.h>
#include <vconf-keys.h>

/******************************************************************************
* defines, Typedef & Structure  Section
******************************************************************************/

/*
 * Frees the memory chunck allocated by free() whose base address is ptr.
	Avoids double free as it checks for NULL and after freeing make
	the ptr as NULL.
*/
#define DOWNLOADER_FREE(ptr)	\
	do { \
		if (ptr != NULL) { \
			free(ptr); \
			ptr = NULL; \
		} \
	} while (0);

#define _(str) gettext(str)  /**< gettext alias */
#define gettext_noop(str) (str)	/**< keyword for xgettext
				  to extract translatable strings */
#define N_(str) gettext_noop(str)  /**< gettext_noop alias */

int Downloader::__m_no_of_dwnlds_in_progress = 0;

Downloader::Downloader()
{
	m_net_type = DOWNLOAD_NETWORK_ALL;
	m_content_size = 0;
	m_progress = 0;
	m_download_cancelled = FALSE;
	m_download_status = DOWNLOAD_READY;
	m_is_no_of_downloads_incremented = false;
}

Downloader::~Downloader()
{
	if (m_is_no_of_downloads_incremented)
		Downloader::__m_no_of_dwnlds_in_progress--;
}



void Downloader::on_progress_cb(int _download_id, unsigned long long _received, void *_user_data)
{
	Downloader *dwn_ptr = (Downloader *)_user_data;
	if (dwn_ptr->GetContentSize()) {
//		PRINTFUNC(NO_PRINT,"download_progress_cb Received=%f, ContentSize=%f\n", (double)_received, (double)dwn_ptr->GetContentSize());
		dwn_ptr->SetProgress((double)_received/(double)dwn_ptr->GetContentSize());
	}
//	PRINTFUNC(DLOG_DEBUG,"download_progress_cb Progress Percentage = %f\n", dwn_ptr->GetProgress());
}

void Downloader::on_state_changed_cb(int _download_id, download_state_e _state, void *_user_data)
{
	WDEBUG("download_state_changed_cb Called and is in  state %d\n", _state);
	Downloader *dwn_ptr = (Downloader *)_user_data;

	//int ret;

	if (DOWNLOAD_STATE_DOWNLOADING == _state) {
		unsigned long long cnt_size;
		int ret = download_get_content_size(_download_id, &cnt_size);
		dwn_ptr->SetContentSize(cnt_size);
		if (ret != DOWNLOAD_ERROR_NONE) {
			WERROR("download_get_content_size failed with error: %d", ret);
		}
		if(dwn_ptr->GetDownloadStatus() < DOWNLOAD_FAILURE){
			dwn_ptr->SetDownloadStatus(DOWNLOAD_PROGRESS);
		}
		WDEBUG("__m_no_of_dwnlds_in_progress: %d", Downloader::__m_no_of_dwnlds_in_progress);
	} else if (DOWNLOAD_STATE_COMPLETED == _state) {
		if (dwn_ptr) {
			dwn_ptr->SetProgress(1.0);
			download_destroy(dwn_ptr->GetDownloadId());
			dwn_ptr->SetDownloadStatus(DOWNLOAD_SUCCESS);
		}
		WDEBUG("__m_no_of_dwnlds_in_progress: %d", Downloader::__m_no_of_dwnlds_in_progress);
	} else if (DOWNLOAD_STATE_FAILED == _state || DOWNLOAD_STATE_CANCELED  == _state) {
		if (dwn_ptr) {
			download_destroy(dwn_ptr->GetDownloadId());
			if (FALSE == dwn_ptr->m_download_cancelled) {
				dwn_ptr->SetDownloadStatus(DOWNLOAD_FAILURE);
			} else {
				dwn_ptr->SetDownloadStatus(DOWNLOAD_CANCEL);
			}
		}
		WDEBUG("__m_no_of_dwnlds_in_progress: %d", Downloader::__m_no_of_dwnlds_in_progress);
	}
}

/**
 * @fn url_downloader
 * This function creates and returns a parsing SUCCESS or FALIURE.
 * @param [IN] \n
 * _url: string url link \n
 * @param [OUT] \n
 * @return	url_download_status Return type is int. \n
 * @remark This API is synchronous.
 */
int Downloader::url_downloader(string _url, void *_data, string _lang_code)
{
	WDEBUG("_url : %s", _url.c_str());
	int ret, found/*, create_ret*/ = 0;
	string filetype;
	m_download_cancelled = FALSE;
	SetDownloadStatus(DOWNLOAD_READY);

	if (_url.empty()) {
		WERROR(" Invalid input url");
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	/* Extract filename string from url string */
	found = _url.find_last_of("/\\");
	filename = _url.substr(found+1);
	found = _url.find_last_of(".");
	filetype = _url.substr(found+1);

	ret = download_create(&m_download_id);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WDEBUG("download_create failed, error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	if (filetype.compare("zip") == 0) {
		ret = download_set_destination(m_download_id, DOWNLOAD_TEMP_DESTINATION_LANGPACKS);
	}
#ifdef VO_DOWNLOAD_SUPPORT
	else if (filetype.compare("res") == 0) {
		string download_dir = DOWNLOAD_TEMP_DESTINATION_HWR_LANGPACKS + _lang_code + "/";
		if (DirectoryExists(download_dir.c_str()) == FALSE) {
			create_ret = CreateDir(download_dir.c_str());
			if (create_ret != 0) {
				download_destroy(m_download_id);
				SetDownloadStatus(DOWNLOAD_FAILURE);
				return DOWNLOAD_FAILURE;
			}
		}
		ret = download_set_destination(m_download_id, download_dir.c_str());
	}
#endif
	else if (filetype.compare("json") == 0) {
		ret = download_set_destination(m_download_id, DOWNLOAD_DESTINATION_JSON);
	}
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_set_destination failed with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	ret = download_set_file_name(m_download_id, filename.c_str());
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_set_file_name failed with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	ret = download_get_network_type(m_download_id, &m_net_type);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_set_file_name failed with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	} else {
		// Add popup code here
	}

	ret = download_set_progress_cb(m_download_id, (download_progress_cb)&on_progress_cb, this);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("failed to register download_set_progress_cb with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	ret = download_set_state_changed_cb(m_download_id, (download_state_changed_cb)&on_state_changed_cb, this);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("failed to register download_set_state_changed_cb with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	ret = download_set_url(m_download_id, _url.c_str());
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("failed to register download_set_state_changed_cb with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	/* Do not allow download to be running in background when app is closed */
	ret = download_set_auto_download(m_download_id, FALSE);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_set_auto_download failed with error: %d", ret);
		download_destroy(m_download_id);
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	/* Increment download progress counter before download start,
	 * as sometimes if user exit application before this pointer is greated than 0 then app may crash
	 */
	Downloader::__m_no_of_dwnlds_in_progress++;
	m_is_no_of_downloads_incremented = true;
	ret = download_start(m_download_id);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_start failed with error: %d", ret);
		download_destroy(m_download_id);
		Downloader::__m_no_of_dwnlds_in_progress--;
		m_is_no_of_downloads_incremented = false;
		SetDownloadStatus(DOWNLOAD_FAILURE);
		return DOWNLOAD_FAILURE;
	}

	SetDownloadStatus(DOWNLOAD_PROGRESS);

	return DOWNLOAD_PROGRESS;
}

/**
 * @fn cancel_download
 * This function cancel the current download and returns a parsing SUCCESS or FALIURE.
 * @param [IN] \n
 * @param [OUT] \n
 * @return	url_download_status Return type is int. \n
 * @remark This API is synchronous.
 */
int Downloader::cancel_download()
{
	int ret/*, found*/;
	string lang_code,  lang_name;
	string str_downloaded = "";
	string filename, filetype, filepath;
	download_state_e state;

	m_download_cancelled = TRUE;

	ret = download_get_state (m_download_id, &state);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_get_state failed with error: %d", ret);
		download_destroy(m_download_id);
		return DOWNLOAD_FAILURE;
	}
	if (state == DOWNLOAD_STATE_QUEUED || state == DOWNLOAD_STATE_DOWNLOADING || state == DOWNLOAD_STATE_PAUSED) {
		ret = download_cancel(m_download_id);
		if (ret != DOWNLOAD_ERROR_NONE) {
			WERROR("download_cancel failed with error: %d", ret);
			download_destroy(m_download_id);
			return DOWNLOAD_FAILURE;
		} else {
			SetDownloadStatus(DOWNLOAD_CANCEL);
		}
	}

	ret = download_unset_progress_cb(m_download_id);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_unset_progress_cb failed with error: %d", ret);
		return DOWNLOAD_FAILURE;
	}

	ret = download_unset_state_changed_cb(m_download_id);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_unset_state_changed_cb failed with error: %d", ret);
		return DOWNLOAD_FAILURE;
	}

	if(GetDownloadStatus() != DOWNLOAD_CANCEL){
		SetDownloadStatus(DOWNLOAD_CANCEL);
	}

	return DOWNLOAD_CANCEL;
}

