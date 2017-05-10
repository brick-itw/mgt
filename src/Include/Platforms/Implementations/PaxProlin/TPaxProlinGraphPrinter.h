#ifndef GRAPH_PRINTER_PAX_H
#define GRAPH_PRINTER_PAX_H

#include "Base/Types/error.h"
#include "Base/Types/inttypes.h"
#include "Platforms/Interfaces/IGraphicPrinter.h"

#include "UI/TCanvas.h"

class TPaxProlinGraphPrinter : public IGraphicPrinter {
public:
	virtual Error StartPrinting(TCanvas& canvas);

	virtual BOOLEAN  IsStillPrinting(){return false;}
	virtual BOOLEAN  IsPaperMissing(){return false;}

	virtual Error Open();
	virtual void Close();

private:
	Error PaxErrorCodeToUposErrorCode(int code);
};

#endif
