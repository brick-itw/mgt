#include "Platforms/Implementations/PC/lin/TLinTimer.h"
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

DWORD TLinTimer::GetMsecCounter()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (DWORD)((tv.tv_sec%1000000)*1000 + tv.tv_usec/1000);
}
