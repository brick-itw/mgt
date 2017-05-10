/**
 * @Author DGKokorev.SBT@sberbank.ru
 * @date   November 2016
 * @brief  Printer device interface declaration
 */

#ifndef _U2_TEXT_PRINTER
#define _U2_TEXT_PRINTER

//#include "base.h"
#include "Base/Types/inttypes.h"

#include "Platforms/Interfaces/IDevice.h"
#include "Base/Types/modules.h"
//#include "UI/TImage.h"

class ITextPrinter : public IDevice {
public:

/*
  Returns ERR_OK if the text is printed instantly.
  Returns ERR_PROCESSING if printing was started and is going to take some time.
  Returns an error code if printing cannot be started.
*/
  virtual Error StartPrinting(const char *text) = 0;
  virtual BOOLEAN   IsStillPrinting() = 0;
  virtual BOOLEAN   IsPaperMissing()  = 0;
};

#endif
 
