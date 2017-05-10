#include "Platforms/Implementations/Verifon/TBuzzer.h"
#include <svc.h>

Error TBuzzer::BeepOK(){
	normal_tone();
	return SUCCESS;
}

Error TBuzzer::BeepError(){
	error_tone();
	return SUCCESS;
}
