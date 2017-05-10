#ifndef _PAX_SYSTEM_LIN_H
#define _PAX_SYSTEM_LIN_H

#include "Platforms/Implementations/TSystem.h"
#include "Platforms/Implementations/PC/TPCSystem.h"
#include "Platforms/Implementations/TDeviceLayer.h"

#include <string>

class TPaxDeviceLayer : public TDeviceLayer {
protected:
  Error LastError;

public:
  TPaxDeviceLayer() : LastError(SUCCESS)
  {
  };

  virtual Error Test(DEVICE deviceType, unsigned* deviceCount);
  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0);

  virtual Error GetLastError()
  {
    return LastError;
  };

private:
  std::string GetRegValue(std::string name);
};

class TPaxProlinSystem : public TPCSystem {
protected:
  virtual void CreateDeviceLayer();

public:
  TPaxProlinSystem();
  virtual ~TPaxProlinSystem()  {};

  virtual void FillModelName(char* pDest, int maxLen);
  virtual void FillOsVersion(char* pDest, int maxLen);

  virtual Error Shutdown();
  virtual Error Restart();
  virtual Error MountFileSystem(const char* path, IFileSystem** fileSystem);
  virtual Error GenerateRandom(BYTE* pData8);
  virtual void Idle();

  Error UpdateSoftware(const char* archiveName);

  virtual Error GetDateTime(DWORD* pdwDate, DWORD* pdwTime);
  virtual ITimer* CreateTimer(DWORD msec);
  virtual void Wait(DWORD msec);

  virtual IFileSystem* GetDefaultFileSystem();

  virtual void GetScreenSize(WORD &width, WORD &height);

private:
  IFileSystem *default_filesystem;
  std::string model_name;
};


#endif
