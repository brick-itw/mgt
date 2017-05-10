#ifndef _PAX_SYSTEM_LIN_H
#define _PAX_SYSTEM_LIN_H

#include "Platforms/Implementations/PC/lin/TLinSystem.h"

class TDeviceLayer : public TDeviceLayer {
protected:
  Error LastError;

public:
  TDeviceLayer() : LastError(SUCCESS)
  {
  };

  virtual Error Test(DEVICE deviceType, unsigned* deviceCount);
  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0);

  virtual Error GetLastError()
  {
    return LastError;
  };
};


class TSystem : public TLinSystem {
protected:
  virtual void CreateDeviceLayer();

public:
  TSystem() : TLinSystem(), default_filesystem(NULL){};
  virtual ~TSystem()  {};

  virtual void FillModelName(char* pDest, int maxLen);
  virtual Error Restart();
  virtual Error MountFileSystem(const char* path, IFileSystem** fileSystem);
  virtual Error GenerateRandom(BYTE* pData8);
  virtual void Idle();

  virtual IFileSystem* GetDefaultFileSystem();

private:
  IFileSystem *default_filesystem;
};


#endif
