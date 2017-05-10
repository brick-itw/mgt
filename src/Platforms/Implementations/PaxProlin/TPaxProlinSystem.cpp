#include "Platforms/Implementations/PaxProlin/TPaxProlinSystem.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinFileSystem.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinCardReader.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinKeyBoard.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinTouchScreen.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinCryptoDevice.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinGraphPrinter.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinGraphicDisplay.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinGprsInterface.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinBuzzer.h"

#include "Platforms/Implementations/PaxProlin/TPaxProlinRS232Interface.h"
#include "Platforms/Implementations/PC/lin/TLinEthernetInterface.h"
#include "Platforms/Implementations/PC/lin/TLinError.h"
#include "Platforms/Implementations/PC/lin/TLinSystem.h"
#include "Platforms/Implementations/PC/lin/TLinTimer.h"
#include "Platforms/Implementations/PaxProlin/TPaxProlinEthernetInterface.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include "osal.h"

Error TPaxDeviceLayer::Test(DEVICE deviceType, unsigned* deviceCount)
{
  *deviceCount = 0;
  return (LastError = ERROR_NO_SUPPORT);
}

std::string TPaxDeviceLayer::GetRegValue(std::string name){
  std::string result;
  char buf[255];
  memset(buf, 0, sizeof(buf));
  int res = OsRegGetValue(name.c_str(), buf);//model name short string
  if(res)
    result.assign(buf);
  return result;
}

IDevice* TPaxDeviceLayer::CreateDevice(DEVICE deviceType, unsigned deviceIndex)
{
  switch(deviceType)
  {
  case DVC_BUZZER:
    if(GetRegValue("ro.fac.buzzer").size())
      return new TPaxProlinBuzzer();
    return NULL;

  case DVC_RS232:
    return new TPaxProlinRS232Interface();

  case DVC_GPRS:
    if(GetRegValue("ro.fac.radio").size())
      return new TPaxProlinGprsInterface(deviceIndex);
    return NULL;
  case DVC_CARD_READER:
	return new TPaxProlinCardReader();

  case DVC_CRYPTO:
	return new TPaxProlinCryptoDevice();

  case DVC_PRINTER:
    if(GetRegValue("ro.fac.printer").size())
      return new TPaxProlinGraphPrinter();
    return NULL;
  case DVC_DISPLAY:
	return new TPaxProlinGraphicDisplay();

  case DVC_ETHERNET:
    {
      std::string res = GetRegValue("persist.sys.eth0.enable");
      if(res != "false")
        return new TPaxProlinEthernetInterface();
    }
    return NULL;
  case DVC_WIFI:
    if(GetRegValue("ro.fac.wifi").size())
      return new TPaxProlinEthernetInterface();
    return NULL;
  case DVC_LOOPBACK:
    return new TLinEthernetInterface();

  case DVC_KEYBOARD:
	  return new TPaxProlinKeyBoard();
	  break;
  case DVC_TOUCHSCREEN:
	  return new TPaxProlinTouchScreen();
	  break;
  default:
    return NULL;
  };
  return NULL;
}

TPaxProlinSystem::TPaxProlinSystem() : TPCSystem(), default_filesystem(NULL){
  char buf[255];
  memset(buf, 0, sizeof(buf));
  //int res = OsRegGetValue("ro.fac.boardid", buf);//model name full string
  int res = OsRegGetValue("ro.fac.mach", buf);//model name short string
  if(res)
    model_name.assign(buf);
  else
    model_name.assign("PAX");//unknown model name
}

Error TPaxProlinSystem::UpdateSoftware(const char* archiveName)
{
  IFileSystem *fsys = NULL;
  Error err = ISystem::Instance()->MountFileSystem("", &fsys);

  const char* fname = "/data/app/MAINAPP/data/new.aip";
  chmod(fname, S_IRWXU);

  int res = 0;
  res = OsInstallFile("/data/app/MAINAPP", fname, FILE_TYPE_APP);
  if(0 == res)
  {
    //fsys->EraseFile(fname);
    //ITimer *timer = ISystem::Instance()->CreateTimer(5000);
    //while(!timer->IsExpired());
    Restart();
    //exit(1);
  }

  return ERROR_NO_SUPPORT;
}

Error TPaxProlinSystem::MountFileSystem(const char* path, IFileSystem** fileSystem)
{
  if(path[0])
  {
    char dirbuf[256];
    snprintf(dirbuf, sizeof(dirbuf), "data/%s", path);
    int res =  mkdir(dirbuf, S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
//  if(res < 0)
//    return LinError(ERROR_FILE_INVALID_DIR);
  }
  *fileSystem = new TPaxProlinFileSystem(path);
  return SUCCESS;
}

void TPaxProlinSystem::FillModelName(char* pDest, int maxLen){
	snprintf(pDest, maxLen, model_name.c_str());
}

Error TPaxProlinSystem::Restart()
{
  OsReboot();
  return SUCCESS;
}

Error TPaxProlinSystem::Shutdown()
{
  OsPowerOff();
  return SUCCESS;
}

Error TPaxProlinSystem::GenerateRandom(BYTE* pData8)
{
  OsGetRandom(pData8, 8);
  return SUCCESS;
}

void TPaxProlinSystem::Idle()
{
	//TPixelDesktop *desktop = TPixelDesktop::Instance();
	//desktop->
}

void TPaxProlinSystem::CreateDeviceLayer()
{
  pDeviceLayer = (TPaxDeviceLayer*)new TPaxDeviceLayer();
}

IFileSystem* TPaxProlinSystem::GetDefaultFileSystem()
{
	if(default_filesystem == NULL)
		MountFileSystem("", &default_filesystem);
	return default_filesystem;
}

void TPaxProlinSystem::GetScreenSize(WORD &width, WORD &height)
{
  int Width = 0;
  int Height = 0;
  OsScrGetSize(&Width, &Height);
  width = Width;
  height = Height;
}

Error TPaxProlinSystem::GetDateTime(DWORD* pDate, DWORD* pTime)
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

void TPaxProlinSystem::Wait(DWORD msec)
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

ITimer* TPaxProlinSystem::CreateTimer(DWORD msec)
{
  return new TLinTimer(msec);
}

void TPaxProlinSystem::FillOsVersion(char* pDest, int maxLen){
  OsRegGetValue("persist.sys.prolin", pDest);
}

