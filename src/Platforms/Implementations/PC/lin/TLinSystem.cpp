#include "Platforms/Implementations/PC/lin/TLinSystem.h"
#include "Platforms/Implementations/PC/lin/TLinFileSystem.h"
#include "Platforms/Implementations/PC/lin/TLinRS232Interface.h"
#include "Platforms/Implementations/PC/lin/TLinEthernetInterface.h"
#include "Platforms/Implementations/PC/lin/TLinError.h"
#include "Base/Types/errno_error.h"
#include "Base/Types/errors.h"
#include "error.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

Error TLinDeviceLayer::Test(DEVICE deviceType, unsigned* deviceCount)
{
  *deviceCount = 0;
  return (LastError = ERROR_NO_SUPPORT);
}

IDevice* TLinDeviceDebugLayer::CreateDevice(DEVICE deviceType, unsigned deviceIndex)
{
  switch(deviceType)
  {
  case DVC_RS232:
    return new TLinRS232Interface(pLogDirectory);

  case DVC_ETHERNET:
  case DVC_WIFI:
  case DVC_GPRS:
  case DVC_LOOPBACK:
    return new TLinEthernetInterface(pLogDirectory);

  default: 
    return NULL;
  };
}

Error TLinSystem::GetDateTime(DWORD* pDate, DWORD* pTime)
{
  struct timeval tv;
  struct tm* ptm;

  gettimeofday(&tv, NULL);
  ptm = localtime(&tv.tv_sec);

  if(pDate)
    *pDate = (ptm->tm_year + 1900) * 10000L +
             (ptm->tm_mon +1)      * 100L + 
             (ptm->tm_mday);

  if(pTime)
    *pTime = (ptm->tm_hour) * 10000L +  
             (ptm->tm_min)  *  100L + 
             (ptm->tm_sec);

  return SUCCESS;
}

void TLinSystem::Wait(DWORD msec)
{
  struct timespec timeout, left;

  timeout.tv_sec = msec / 1000;
  timeout.tv_nsec = (msec%1000)*1000000;

  left.tv_sec = 0;
  left.tv_nsec = 0;

  while(nanosleep(&timeout, &left) && (errno == EINTR))  
  {
    timeout.tv_sec = left.tv_sec;
    timeout.tv_nsec= left.tv_nsec;
  }
}

ITimer* TLinSystem::CreateTimer(DWORD msec)
{
  return new TLinTimer(msec);
}

Error TLinSystem::MountFileSystem(const char* path, IFileSystem** fileSystem)
{
  char dirbuf[256];

  sprintf(dirbuf,"data/%s", path);
  int res =  mkdir(dirbuf, S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
//  if(res < 0)
//    return LinError(ERROR_FILE_INVALID_DIR);

  *fileSystem = new TLinFileSystem(path);
  return SUCCESS;
}

void TLinSystem::CreateDeviceLayer()
{
#ifdef _NEED_TRACES
  MountFileSystem("Trace", (IFileSystem**)&pTraces);
#endif
  pDeviceLayer = new TLinDeviceDebugLayer(pTraces);
}




