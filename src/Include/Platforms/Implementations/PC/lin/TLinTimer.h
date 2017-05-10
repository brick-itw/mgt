#ifndef T_LIN_TIMER
#define T_LIN_TIMER

#include "Base/Types/types.h"
#include "Platforms/Implementations/PC/TPCSystem.h"

class TLinTimer : public TPCTimer{
protected:
  virtual DWORD GetMsecCounter();

public:
  TLinTimer(DWORD tmo) : TPCTimer()
  {
    Setup(tmo);
  };

};

#endif
