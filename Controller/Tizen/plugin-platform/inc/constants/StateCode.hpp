
class StateCode {
private:
		int stateCode;
		const static int CHECK_VALIDITY = 0;
		const static int CHECK_SIGNATURE = 1;
		const static int SOS_COPIED = 2;
		const static int RESOURCE_COPIED = 3;
		const static int CREATE_OPTIMIZEDDIR = 4;
		const static int DEPENDENCY_CHECKED = 5;
		StateCode(int stateCode){
			this->stateCode=stateCode;
		}
};