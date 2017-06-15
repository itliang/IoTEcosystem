
class SuccessCode {
private:
		int successCode;
		//map<string, Object> properties;
		const static int PLUGIN_METADATA_FOUND = 758400;
		const static int PLUGIN_UPDATE_FOUND = 758401;
		const static int PLUGIN_OUTDATED = 758402;
		const static int PLUGIN_DOWNLOADED = 758410;
		const static int PLUGIN_ALREADY_DOWNLOADED = 758411;
		const static int PLUGIN_LOADED = 758420;
		const static int PLUGIN_ALREADY_LOADED = 758421;
		const static int PLUGIN_INSTALLED = 758430;
		const static int PLUGIN_ALREADY_INSTALLED = 758431;
		const static int PLUGIN_IS_LAUNCHING = 758440;
		const static int PLUGIN_LAUNCHED = 758441;
		
		SuccessCode(int successCode){
			this->successCode=successCode;
			//properties = new HashMap<String, Object>();
		}

public:		
		/**
		 * Returns the success code number
		 */
		int getSuccessCode() {
			return this->successCode;
		}
};