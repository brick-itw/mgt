#ifndef T_BUZZER_PAX
#define T_BUZZER_PAX

#include "Platforms/Interfaces/IBuzzer.h"

class TBuzzer : public IBuzzer
{
	virtual Error BeepOK();
	virtual Error BeepError();
	virtual Error Open(){return SUCCESS;}
	virtual void Close(){}
};

#endif
