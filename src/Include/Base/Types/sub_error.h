#ifndef _U2_SUB_ERROR_H
#define _U2_SUB_ERROR_H

#include "Base/Types/error.h"

class SubError : public Error{
public:

  SubError(unsigned long mainError, unsigned long subError) : Error(mainError) 
  {
    if(mainError != SUCCESS)
      AddSubError(subError);
  }
};

#endif
 