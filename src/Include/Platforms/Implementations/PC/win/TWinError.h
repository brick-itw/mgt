#ifndef _U2_WIN_ERROR_H
#define _U2_WIN_ERROR_H

#include "Base/Types/error.h"
#include "Base/Types/modules.h"

#define WIN_ERROR(code) REGISTER_ERROR(MODULE_OS_WIN,(code))

class WinError : public Error{
public:

  WinError(unsigned long mainError) : Error(mainError) 
  {
    if(mainError != SUCCESS)
      AddSubError(WIN_ERROR(GetLastError()));
  }
};

#endif
 