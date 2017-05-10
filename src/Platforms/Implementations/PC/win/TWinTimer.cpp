#include "Platforms/Implementations/PC/win/TWinTimer.h"

#ifndef _CONSOLE
  #include "stdafx.h"
#else
 #include <windows.h>
#endif


DWORD TWinTimer::GetMsecCounter()
{
  Sleep(0);
  return GetTickCount();
}
