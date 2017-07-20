/*
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
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

#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <vector>
#include <string>
#include <map>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

using namespace std;
/******************************************************************************
* defines, Typedef & Structure  Section
******************************************************************************/
#define DOWNLOAD_COMPLETE_QUICKPANEL_IMAGE         "/usr/apps/com.samsung.quickpanel/shared/res/noti_icons/Common/noti_download_complete.png"
#define DOWNLOAD_FAILED_QUICKPANEL_IMAGE           "/usr/apps/com.samsung.quickpanel/shared/res/noti_icons/Common/noti_download_failed.png"

#define DOWNLOAD_COMPLETE_INDICATOR_IMAGE          "/usr/apps/com.samsung.indicator/res/icons/Processing/b03_processing_download_complete.png"
#define DOWNLOAD_FAILED_INDICATOR_IMAGE          "/usr/apps/com.samsung.indicator/res/icons/Processing/b03_processing_download_failed.png"

#define DOWNLOAD_NOTI_QUICKPANEL_ONGOING_ICON_PATH    "reserved://quickpanel/ani/downloading"
#define DOWNLOAD_NOTI_INDICATOR_ONGOING_ICON_PATH     "reserved://indicator/ani/downloading"

#define SWIFTKEY_LANG_DIR                          "/opt/usr/share/scim/swiftkey/lang/"

#ifdef LIVELANG
#define DOWNLOAD_DESTINATION_LANGPACKS             "/opt/usr/apps/com.samsung.swiftkey-lpm/data/livelangpacks/"
#define DOWNLOAD_TEMP_DESTINATION_LANGPACKS        "/opt/usr/apps/com.samsung.swiftkey-lpm/data/temp-livelangpacks/"
#else
#define DOWNLOAD_DESTINATION_LANGPACKS             "/opt/usr/apps/com.samsung.swiftkey-lpm/data/langpacks/"
#define DOWNLOAD_TEMP_DESTINATION_LANGPACKS        "/opt/usr/apps/com.samsung.swiftkey-lpm/data/temp-langpacks/"
#endif

#ifdef VO_DOWNLOAD_SUPPORT
#define DOWNLOAD_DESTINATION_HWR_LANGPACKS         "/opt/usr/apps/com.samsung.swiftkey-lpm/data/hwr-langpacks/"
#define DOWNLOAD_TEMP_DESTINATION_HWR_LANGPACKS    "/opt/usr/apps/com.samsung.swiftkey-lpm/data/temp-hwr-langpacks/"
#define VO_LANG_DIR                                "/opt/usr/share/scim/vo/"
#endif

#ifdef LIVELANG
#define DOWNLOAD_DESTINATION_JSON                  "/opt/usr/apps/com.samsung.swiftkey-lpm/data/json/livelang/"
#else
#define DOWNLOAD_DESTINATION_JSON                  "/opt/usr/apps/com.samsung.swiftkey-lpm/data/json/language/"
#endif
/**
*@enum lpm_download_status
* This enumerates the error codes of lpm-downloader.
*/
typedef enum {
	/** Status indicating ready state */
	DOWNLOAD_READY,
	/** Status indicating download is currently in progress */
	DOWNLOAD_PROGRESS,
	/** Status indicating download is compled and post download operations are in progress */
	DOWNLOAD_POST_PROGRESS,
	/** Status indicating success */
	DOWNLOAD_SUCCESS,
	/** Status indicating some kind of failure */
	DOWNLOAD_FAILURE,
	/** Status indicating download is cancelled */
	DOWNLOAD_CANCEL,
	/** Status indicating download is failed and pre failure cleanup operations are in progress */
	DOWNLOAD_PRE_FAILURE,
	/** Status indicating download is cancelled and pre cancelled cleanup operations are in progress */
	DOWNLOAD_PRE_CANCEL,
} lpm_download_status;

typedef struct appdata App_Data;

class Downloader
{
private:
	//Ecore_Pipe *mPipe;
protected:
	int m_download_id;
	int m_download_status;
	//download_network_type_e m_net_type;
	double m_progress;
	unsigned long long m_content_size;
	string filename;
public:
	static int __m_no_of_dwnlds_in_progress;
	bool m_download_cancelled;
	bool m_is_no_of_downloads_incremented;
public:
	Downloader();
	virtual ~Downloader();
	double GetProgress(){return m_progress;};
	void SetProgress(double fill){m_progress = fill;};
	int GetDownloadStatus(){return m_download_status;};
	void SetDownloadStatus(int fill){m_download_status = fill;};
	unsigned long long GetContentSize(){return m_content_size;};
	void SetContentSize(unsigned long long fill){m_content_size = fill;};
	int GetDownloadId(){return m_download_id;};
	int GetNoOfDownloads(){return Downloader::__m_no_of_dwnlds_in_progress;};
	string GetDownloadFilename(){return filename;};
	int url_downloader(string url, void *data, string lang_code);
	int cancel_download();

	static void on_progress_cb(int download_id, unsigned long long received, void *user_data);
	static void on_state_changed_cb(int download_id, download_state_e state, void *user_data);

};

#endif /* __DOWNLOADER_H__ */

