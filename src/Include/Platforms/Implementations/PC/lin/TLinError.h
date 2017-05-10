#ifndef _U2_LIN_ERROR_H
#define _U2_LIN_ERROR_H

#include "Base/Types/error.h"
#include "Base/Types/modules.h"
#include <errno.h>

#define LIN_ERROR(code) REGISTER_ERROR(MODULE_OS_LIN,(code))

class LinError : public Error{
public:

  LinError(unsigned long mainError) : Error(mainError) 
  {
    if(mainError != SUCCESS)
      AddSubError(LIN_ERROR(errno));
  }
};

#endif
 
