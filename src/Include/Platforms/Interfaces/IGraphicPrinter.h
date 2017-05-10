/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Printer device interface declaration
 */

#ifndef _U2_GRAPHIC_PRINTER
#define _U2_GRAPHIC_PRINTER

#include "Base/Types/inttypes.h"
#include "Base/Types/modules.h"

#include "Platforms/Interfaces/IDevice.h"
#include "UI/TMonochromeImage.h"
#include "UI/TImage.h"
#include "UI/TCanvas.h"

#define PRINTER_ERROR(code) REGISTER_ERROR(MODULE_PRINTER, (code))
enum PrinterErrorCode
{
  ERROR_PRN_NO_PRINTER       = PRINTER_ERROR(1), //Device does not exist.
  ERROR_PRN_BUSY             = PRINTER_ERROR(2),
  ERROR_PRN_PAPEROUT         = PRINTER_ERROR(3),
  ERROR_PRN_WRONG_PACKAGE    = PRINTER_ERROR(4), //The format of printing data package error.
  ERROR_PRN_OVERHEAT         = PRINTER_ERROR(5),
  ERROR_PRN_OUTOFMEMORY      = PRINTER_ERROR(6),
  ERROR_PRN_INVALID_PARAM    = PRINTER_ERROR(7),
  ERROR_PRN_NO_FONT          = PRINTER_ERROR(8),
};

class IGraphicPrinter : public IDevice {
public:

/*
  Returns ERR_OK if the image is printed instantly.
  Returns ERR_PROCESSING if printing was started and is going to take some time.
  Returns an error code if printing cannot be started.
*/

  virtual Error StartPrinting(TCanvas &canvas) = 0;

  virtual BOOLEAN   IsStillPrinting() = 0;

  virtual BOOLEAN   IsPaperMissing()  = 0;
};

#endif

