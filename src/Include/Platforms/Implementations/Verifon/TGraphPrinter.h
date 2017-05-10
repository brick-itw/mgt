#ifndef GRAPH_PRINTER_PAX_H
#define GRAPH_PRINTER_PAX_H

//#include "api/printer.h"
#include "Base/Types/error.h"
#include "Platforms/Interfaces/IDevice.h"

//#include "base.h"
#include "Base/Types/inttypes.h"

#include "UI/TCanvas.h"

#define ERR_OK                  0 /**< Успешно                                                                */
#define DRV_FAILURE           253 /**< Внутренняя ошибка: Что-то пошло не так...                              */
#define DRV_NO_HANDLE -1

class TGraphPrinter : public IDevice {
public:

	virtual Error StartPrinting(TCanvas& canvas);
	virtual BOOLEAN   IsStillPrinting(){return false;}
	virtual BOOLEAN   IsPaperMissing(){return true;}

	virtual Error Open();
	virtual void Close();
private:
  int HandlePrn;
  virtual BYTE ValidHandle(int iHandle);
  virtual BYTE Print(BYTE* pData, DWORD height, DWORD width);
  virtual void WaitPrinterReady(void);

};

#endif
