#include <osal.h>

#include "Platforms/Implementations/PaxProlin/TPaxProlinTextPrinter.h"

Error TPaxProlinTextPrinter::StartPrinting(const char *text){
	OsPrnPrintf("%s", text);
	OsPrnStart();
	return SUCCESS;
}

Error TPaxProlinTextPrinter::Open(){
	OsPrnOpen(PRN_REAL, REAL_PRN_NAME);
	OsPrnSelectFontSize(24, 48, 48, 48);
	return SUCCESS;
}

void TPaxProlinTextPrinter::Close(){
	OsPrnClose();
}
