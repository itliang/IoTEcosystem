#ifndef _BGSERVICE_STATE_H_
#define _BGSERVICE_STATE_H_
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

#endif
