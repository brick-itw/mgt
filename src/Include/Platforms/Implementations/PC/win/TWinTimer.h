#ifndef T_WIN_TIMER
#define T_WIN_TIMER

#include "Platforms/Implementations/PC/TPCSystem.h"

class TWinTimer : public TPCTimer{
protected:
  virtual DWORD GetMsecCounter();

public:
  TWinTimer(DWORD tmo) : TPCTimer()
  {
    Setup(tmo);
  };

};

#endif
