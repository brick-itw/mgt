#ifndef _U2_ERRNO_ERROR_H
#define _U2_ERRNO_ERROR_H

#include "Base/Types/error.h"
#include "Base/Types/modules.h"
#include <errno.h>

class ErrnoError : public Error{
public:

  ErrnoError(unsigned long mainError) : Error(mainError) 
  {
    if(mainError != SUCCESS)
      AddSubError(errno);
  }
};

#endif
 