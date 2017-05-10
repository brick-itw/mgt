#ifndef TEXT_PRINTER_PAX_H
#define TEXT_PRINTER_PAX_H

#include "Platforms/Interfaces/ITextPrinter.h"
#include "Platforms/Interfaces/IDevice.h"

class TPaxProlinTextPrinter : public IDevice {
public:

	virtual Error StartPrinting(const char *text);
	virtual BOOLEAN   IsStillPrinting(){return false;}
	virtual BOOLEAN   IsPaperMissing(){return true;}

	virtual Error Open();
	virtual void Close();
};

#endif
