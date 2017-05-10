#ifndef _PC_SYSTEM_LIN_H
#define _PC_SYSTEM_LIN_H

#include "Platforms/Interfaces/ISystem.h"
#include "Base/Types/error.h"
#include "Platforms/Implementations/PC/TPCSystem.h"
#include "Platforms/Implementations/PC/lin/TLinFileSystem.h"
#include "Platforms/Implementations/PC/lin/TLinDeviceLayer.h"

class TLinSystem : public TPCSystem {
public:

  TLinSystem()
  {
    //pDeviceLayer = 0;
    //pTraces = 0;
  };

  virtual ~TLinSystem()
  {
    //UnmountFileSystem(pTraces);
  };

  virtual Error GetDateTime(DWORD* pdwDate, DWORD* pdwTime);
  virtual ITimer* CreateTimer(DWORD msec);
  virtual void Wait(DWORD msec);

  
  virtual Error MountFileSystem(const char* path, IFileSystem** fileSystem);
  virtual Error UnmountFileSystem(IFileSystem* fileSystem);
};


#endif
