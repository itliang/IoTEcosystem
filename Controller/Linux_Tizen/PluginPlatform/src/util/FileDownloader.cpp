#include <cstring>
#include "FileDownloader.h"
#include "Log.h"
//#include "MRFileUtil.h"
//#include "MRConnectionUtil.h"


MRFileDownloader::MRFileDownloader():
		__songUrl(""),
		__downloadPath(""),
		__downloadFileName(""),
		__downloadID(-1),
		mListener(NULL),
		mCurrentState(DOWNLOAD_STATE_NONE)
{

	mTimer.SetInterval(PROGRESS_RECEIVE_TIMER_TIME_OUT);
	mTimer.SetEventListener((TimerEventListener*) this);
	mPipe = ecore_pipe_add( SGIOPipeWatchCB, (void*)this);

}

void MRFileDownloader::SGIOPipeWatchCB( void *data, void *pdata, unsigned int nbyte )
{
	FUNC_BEGIN();
	WDEBUG("%d",nbyte);
	MRFileDownloader *pThis = static_cast<MRFileDownloader *>(data);
	pThis->CheckDownlaodStatus((char*)pdata, sizeof(PipeTransType));
	FUNC_END();

	return ;
}

MRFileDownloader::~MRFileDownloader()
{
	FUNC_BEGIN();
	mTimer.UnSetEventListener();
	if(__downloadID != -1){
		download_unset_state_changed_cb(__downloadID);
		download_unset_progress_cb(__downloadID);
		download_destroy(__downloadID);
		if(mCurrentState != DOWNLOAD_STATE_COMPLETED){
			//remove the temp file
			std::string filepath = __downloadPath;
			filepath.append(__downloadFileName);
			WDEBUG("Removing file %s",filepath.c_str());
			//FileUtil::RemoveFile(filepath);
		}
	}
	ecore_pipe_del(mPipe);

	FUNC_END();
}

void
MRFileDownloader::SetDownloadFileUrl (std::string url)
{
	__songUrl = url;
}

void
MRFileDownloader::SetDownloadPath (std::string downloadPath)
{
	FUNC_BEGIN();
	__downloadPath = downloadPath;
	FUNC_END();
}

void
MRFileDownloader::SetDownloadFileName (std::string fileName)
{
	FUNC_BEGIN();
	__downloadFileName = fileName;
	FUNC_END();
}

std::string MRFileDownloader::GetDownloadFile()
{
	char* filename= NULL;
	std::string filenameStr;
	download_get_file_name(__downloadID,&filename);
	if(filename != NULL){
		filenameStr.append(filename);
	}
	return filenameStr;
}

int
MRFileDownloader::CancelDownload()
{	FUNC_BEGIN();
	int ret;
	download_state_e state;

	ret = download_get_state (__downloadID, &state);
	WDEBUG("Download State %d",state);
	if (ret != DOWNLOAD_ERROR_NONE) {
		WERROR("download_get_state failed with error: %d", ret);
		return ret;
	}

	if (state == DOWNLOAD_STATE_QUEUED || state == DOWNLOAD_STATE_DOWNLOADING || state == DOWNLOAD_STATE_PAUSED) {
		ret = download_cancel(__downloadID);
		if(ret != DOWNLOAD_ERROR_NONE){
			WERROR("download_cancel failed");
		}
	}else{
		WERROR("Invalid state");
	}
//	__downloadID = -1;
	mCurrentState =  DOWNLOAD_STATE_NONE;
	FUNC_END();
	return ret;
}

int
MRFileDownloader::PauseDownload()
{
	FUNC_BEGIN();
	int ret;
	download_state_e state;

	ret = download_get_state (__downloadID, &state);
	if(state == DOWNLOAD_STATE_DOWNLOADING){
		if(__downloadID != -1){
			download_pause(__downloadID);
		}
		else{
			WERROR("No download id ");
			return -1;
		}
	}else
	{
		WERROR("Invalid state %d",state);
		return -1;
	}

	return ret;
	FUNC_END();
}

int
MRFileDownloader::ResumeDownload()
{
	FUNC_BEGIN();
	int ret;
	download_state_e state;

	ret = download_get_state (__downloadID, &state);
	if(state == DOWNLOAD_STATE_PAUSED){
		if(__downloadID != -1){
			download_start(__downloadID);
		}
		else{
			WERROR("No download id ");
			return -1;
		}
	}else
	{
		WERROR("Invalid state %d",state);
		return -1;
	}

	return ret;
	FUNC_END();
}

FILE_DOWNLOAD_STATE
MRFileDownloader::GetDownloadState()
{
	FUNC_BEGIN();
	//int ret;
	download_state_e state;

	download_get_state (__downloadID, &state);
	FUNC_END();
	return (FILE_DOWNLOAD_STATE)state;

}
FileDownloadError
MRFileDownloader::StartDownload(void)
{
	FUNC_BEGIN();
	FileDownloadError errorRet = FILE_DOWNLOAD_ERROR_NONE;

	if(__downloadID != -1 ){
		mCurrentState =  DOWNLOAD_STATE_NONE;
		download_cancel(__downloadID);
		download_destroy(__downloadID);
		download_unset_state_changed_cb(__downloadID);
		download_unset_progress_cb(__downloadID);

	}
	int ret = download_create(&__downloadID);
	if (ret == 0)
	{

		WDEBUG(" download created,%s",__songUrl.c_str());
		ret = download_set_url(__downloadID, __songUrl.c_str());
		if(ret != DOWNLOAD_ERROR_NONE){
			WERROR(" value of download_set_url is %d %s %s", ret,__songUrl.c_str(),get_error_message(ret));
			errorRet = FILE_DOWNLOAD_ERROR_OTHERS;
		}

		ret = download_set_destination(__downloadID, __downloadPath.c_str());
		if(ret != DOWNLOAD_ERROR_NONE){
			WERROR(" value of download_set_destination is %d %s %s", ret,__downloadPath.c_str(),get_error_message(ret));
			errorRet = FILE_DOWNLOAD_ERROR_OTHERS;
		}

		ret = download_set_file_name(__downloadID, __downloadFileName.c_str());
		if(ret != DOWNLOAD_ERROR_NONE){
			WERROR(" value of download_set_file_name is %d %s %s", ret,__downloadFileName.c_str(),get_error_message(ret));
			errorRet = FILE_DOWNLOAD_ERROR_OTHERS;
		}

		ret = download_set_auto_download(__downloadID, false);
		if(ret != DOWNLOAD_ERROR_NONE){
			WERROR(" value of download_set_auto_download is (%d) %s", ret,get_error_message(ret));
			errorRet = FILE_DOWNLOAD_ERROR_OTHERS;
		}

		download_set_state_changed_cb(__downloadID,SDownloadStateChange,this);
		download_set_progress_cb(__downloadID,SDownloadProgressCallback,this);


		ret = download_start(__downloadID);
		if(ret != DOWNLOAD_ERROR_NONE){
			WERROR(" value of download_start is (%d) %s", ret,get_error_message(ret));
			errorRet = FILE_DOWNLOAD_ERROR_OTHERS;
		}

	}else{
		errorRet = FILE_DOWNLOAD_ERROR_OTHERS;
		WDEBUG("value of download_create is (%d) %s", ret,get_error_message(ret));
	}

	return errorRet;
	FUNC_END();
}

void MRFileDownloader::SDownloadStateChange(int download_id,download_state_e state, void *user_data)
{
	FUNC_BEGIN();

	MRFileDownloader* pThis = static_cast<MRFileDownloader*>(user_data);
	if(pThis == NULL){
		WERROR("pThis == NULL ahhhhhhhhh common download provider");
		return;
	}
	PipeTransDownloadStatusData *pipeTransStateData = new PipeTransDownloadStatusData();
	pipeTransStateData->mDowloadId = download_id;
	pipeTransStateData->mState = state;
	pipeTransStateData->mStatusType = DOWNLOAD_STATE_CHANGED;
	WDEBUG("===============================>writing to pipe  %x,state is %d",pThis->mPipe,state);
	Eina_Bool ret  =ecore_pipe_write( pThis->mPipe, &pipeTransStateData, sizeof(PipeTransType) );
	WDEBUG("writing to pipe  %d",ret);
	FUNC_END();
}

void MRFileDownloader::SDownloadProgressCallback (int download_id, unsigned long long received, void *user_data)
{

	//Write to pipe
	MRFileDownloader* pThis = static_cast<MRFileDownloader*>(user_data);
	if(pThis == NULL){
		WERROR("pThis == NULL ahhhhhhhhh common download provider");
		return;
	}
	PipeTransDownloadStatusData *pipeTransStateData = new PipeTransDownloadStatusData();
	pipeTransStateData->mDowloadId = download_id;
	pipeTransStateData->mReceivedLen = received;
	pipeTransStateData->mStatusType = DOWNLOAD_STATE_PROGRESS;
	WDEBUG("writing to pipe ");
	Eina_Bool ret  =ecore_pipe_write( pThis->mPipe, &pipeTransStateData, sizeof(PipeTransType) );
	WDEBUG("writing to pipe  %d",ret);
}

void MRFileDownloader::CheckDownlaodStatus(char* message,unsigned int messagelen)
{
	FUNC_BEGIN();
	PipeTransDownloadStatusData *pipeDownloadData = NULL;

	if (message == NULL || sizeof(pipeDownloadData) != messagelen) {
		WDEBUG("invalid pointer type");
		return;
	}

	std::memcpy(&pipeDownloadData, message, messagelen);

	if (pipeDownloadData == NULL) {
		WDEBUG("Invalid pipe Curl data");
		return;
	}
	switch(pipeDownloadData->mStatusType)
	{
	case DOWNLOAD_STATE_CHANGED:
		OnDownloadStateChange(pipeDownloadData->mDowloadId,pipeDownloadData->mState);
		break;
	case DOWNLOAD_STATE_PROGRESS:
		OnDownloadProgress(pipeDownloadData->mDowloadId,pipeDownloadData->mReceivedLen);
		break;
	case DOWNLOAD_STATE_INITIALIZED:
		WERROR("###");
		break;
	}

	if (pipeDownloadData) {
		delete pipeDownloadData;
		pipeDownloadData = NULL;
	}

	FUNC_END();

}

void MRFileDownloader::GetError(FileDownloadError& error)
{
	FUNC_BEGIN();
	download_error_e dlerror;
	download_get_error(__downloadID,&dlerror);
	WERROR("download_get_error=%d %s",dlerror,get_error_message(dlerror));
	if(dlerror ==  DOWNLOAD_ERROR_NONE){
		error = FILE_DOWNLOAD_ERROR_NONE;
	}else if(dlerror== DOWNLOAD_ERROR_NO_SPACE){
		error = FILE_DOWNLOAD_ERROR_NO_SPACE;
	}else if(dlerror== DOWNLOAD_ERROR_CONNECTION_TIMED_OUT){
		error = FILE_DOWNLOAD_CONNECTION_TIME_OUT;
	}
	else
	{
		error = FILE_DOWNLOAD_ERROR_OTHERS;
	}
	FUNC_END();
	return;
}

void MRFileDownloader::OnDownloadStateChange(int download_id,download_state_e state)
{
	if(mListener != NULL){

		mListener->OnFileDownloadStateChanged(__songUrl,(FILE_DOWNLOAD_STATE)state);
	}
	if(state == DOWNLOAD_STATE_DOWNLOADING){
		//Start timeout timer
		mTimer.Start();
	}
	if(state == DOWNLOAD_STATE_COMPLETED){
		mTimer.Stop();
	}
	mCurrentState = state;
}
void MRFileDownloader::OnDownloadProgress(int download_id, unsigned long long received)
{
	WDEBUG("received = %lld",received);
	mTimer.Stop();
	mTimer.Start();
}


bool MRFileDownloader::OnTimerTick()
{
	/*
	ConnectionUtil connUtil;
	//Check the connection before starting the timer
	if(connUtil.IsNetworkConnected() == true){
		DMSG_ERR("Download progress did not occur in 30 secs");
		std::string filepath = __downloadPath;
		filepath.append(__downloadFileName);
		DMSG("Removing file %s",filepath.c_str());
		//FileUtil::RemoveFile(filepath);
		FileDownloadError error = StartDownload();
		if(error != FILE_DOWNLOAD_ERROR_NONE){
			if(mListener != NULL){
				mListener->OnFileDownloadStateChanged(__songUrl,FILE_DOWNLOAD_STATE_FAILED);
			}
		}
	}else{
		DMSG_ERR("No connection ");
	}*/
	return false;
}

void MRFileDownloader::OnFileDownloadStateChanged(std::string requestId,FILE_DOWNLOAD_STATE state){
   FUNC_BEGIN();
}

