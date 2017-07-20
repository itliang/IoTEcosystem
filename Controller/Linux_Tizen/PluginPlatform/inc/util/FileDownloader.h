#ifndef __FILEDOWNLOADER_H_
#define __FILEDOWNLOADER_H_

#include <string>
#include <tr1/memory>
#include <glib.h>

#include "Timer.h"


typedef enum
{
	FILE_DOWNLOAD_ERROR_NO_SPACE,
	FILE_DOWNLOAD_CONNECTION_TIME_OUT,
	FILE_DOWNLOAD_ERROR_OTHERS,
	FILE_DOWNLOAD_ERROR_NONE
} FILE_DOWNLOAD_ERROR;

typedef enum
{
   FILE_DOWNLOAD_STATE_NONE = 0,
    FILE_DOWNLOAD_STATE_READY = 1,
    FILE_DOWNLOAD_STATE_QUEUED = 2,
    FILE_DOWNLOAD_STATE_DOWNLOADING = 3,
    FILE_DOWNLOAD_STATE_PAUSED = 4,
	FILE_DOWNLOAD_STATE_COMPLETED = 5,
    FILE_DOWNLOAD_STATE_FAILED = 6,
    FILE_DOWNLOAD_STATE_CANCELED = 7,
} FILE_DOWNLOAD_STATE;

class IFileDownloadListener{
public:
	IFileDownloadListener(){};
	virtual ~IFileDownloadListener(){};
	virtual void OnFileDownloadStateChanged(std::string requestId,FILE_DOWNLOAD_STATE state)=0;
};


class FileDownloader: TimerEventListener, IFileDownloadListener {
public:
	FileDownloader();
	~FileDownloader();

	void SetDownloadFileUrl (std::string url);
	void SetDownloadPath (std::string downloadPath);
	void SetDownloadFileName (std::string fileName);
	std::string GetDownloadFile();
	void GetError(FileDownloadError& error);
	void SetDownloadStateChangeListener(IFileDownloadListener *listener)
	{
		mListener = listener;
	}


	std::string GetDownloadSongUrl(void)
	{
		return __songUrl;
	}

	std::string GetDownloadPath(void)
	{
		return __downloadPath;
	}

	std::string GetDownloadfileName(void)
	{
		return __downloadFileName;
	}

	FileDownloadError StartDownload(void);
	int CancelDownload(void);
	int PauseDownload();
	int ResumeDownload();
	FILE_DOWNLOAD_STATE GetDownloadState();



	static void SDownloadStateChange(int download_id,download_state_e state, void *user_data);
	static void SDownloadProgressCallback (int download_id, unsigned long long received, void *user_data);

	//TimerEventListener
	virtual bool OnTimerTick();
	//IFileDownloadListener
	virtual void OnFileDownloadStateChanged(std::string requestId,FILE_DOWNLOAD_STATE state);

	enum DownStatusType {
		DOWNLOAD_STATE_INITIALIZED,
		DOWNLOAD_STATE_CHANGED,
		DOWNLOAD_STATE_PROGRESS,
	};

	class PipeTransDownloadStatusData
	{
	public:
		PipeTransDownloadStatusData()
		{
			mStatusType = DOWNLOAD_STATE_INITIALIZED;
			mState = DOWNLOAD_STATE_NONE;
			mDowloadId = -1;
			mReceivedLen =0;
		}
		virtual ~PipeTransDownloadStatusData() {}

	public:
		DownStatusType mStatusType;
		//download_state_e mState;
		int mDowloadId;
		unsigned long long mReceivedLen;


	};

private:
	static void SGIOPipeWatchCB(void *data, void *pdata, unsigned int nbyte );
	void CheckDownlaodStatus(char* message,unsigned int messagelen);
	void OnDownloadStateChange(int download_id,download_state_e state);
	void OnDownloadProgress(int download_id, unsigned long long received);

private:
	static const double PROGRESS_RECEIVE_TIMER_TIME_OUT = 30.0; //second
	std::string __songUrl;
	std::string __downloadPath;
	std::string __downloadFileName;
	int __downloadID;
	//IFileDownloadListener* mListener;
	Timer mTimer;
	//download_state_e mCurrentState;
	typedef void *PipeTransType;
	//Ecore_Pipe *mPipe;


};

typedef std::tr1::shared_ptr<MRFileDownloader> FileDownloaderPtr;


#endif /* MRFILEDOWNLOADER_H_ */
