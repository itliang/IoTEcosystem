
class BgServiceState {
private:
		int stateCode;
		const static int STARTING = 0;
		const static int PREFETCHING = 1;
		const static int READY = 2;
		BgServiceState(int stateCode){
			this->stateCode=stateCode;
		}
};
