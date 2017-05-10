#include "Platforms/Implementations/PaxProlin/TPaxProlinBuzzer.h"

#include <osal.h>
#include <xui.h>


Error TPaxProlinBuzzer::BeepOK(){
	OsBeep(1,50);
	return SUCCESS;
}

Error TPaxProlinBuzzer::BeepError(){
	OsBeep(5,600);
	return SUCCESS;
}
