
class ErrorCode {
private:
	const static int OPERATION_FAILED = -758400;
	const static int INVALID_PARAM = -758401;
	const static int INIT_FAILED = -758402;
	const static int PLUGIN_NOT_FOUND = -758403;
	const static int PLUGIN_NOT_AVAILABLE = -758404;
	const static int PLUGIN_NOT_EXIST = -758405;
	const static int NOT_ENOUGH_SPACE = -758406;
	const static int DOWNLOADING_NOW = -758407;
	const static int DOWNLOAD_FORCE_STOP = -758408;
	const static int EXPIRED_DOWNLOAD_URL = -758409;
	const static int BGSERVICE_NOT_BOUND = -758410;
	const static int NOT_VALID_SIGNATURE = -758411;
	
	const static int NOT_NECESSARY_TO_UPDATE = -758412;
	const static int APPLICATION_NOT_AVAILABLE = -758413;
	const static int FORCE_STOP = -758414;
	const static int PLUGIN_IS_ALREADY_LAUNCHING = -758415;
	const static int LIBS_EXTRACTION_FAILED = -758416;
	const static int PLUGIN_NOT_COMPATIBLE = -758417;
	int errorCode;

public:
	

	ErrorCode(int errorCode) {
        this->errorCode = errorCode;
    }
	
	int getErrorCode() {return this->errorCode;}
	
};