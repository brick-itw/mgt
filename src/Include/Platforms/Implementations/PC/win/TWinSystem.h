#ifndef _PC_SYSTEM_WIN_H
#define _PC_SYSTEM_WIN_H

#include "Platforms/Implementations/Pc/TPCSystem.h"
#include "Base/Types/error.h"
#include "Platforms/Implementations/Pc/win/TWinFileSystem.h"
#include "Platforms/Implementations/PC/win/TWinTimer.h"

class TWinSystem : public TPCSystem {
protected:  
  TWinFileSystem     DefaultFS;
  int                CurrentRandomIndex;

  virtual void CreateDeviceLayer();

public:

  TWinSystem() : DefaultFS(""), CurrentRandomIndex(0) {};

  virtual ~TWinSystem() {};
    
  virtual Error GetDateTime(DWORD* pdwDate, DWORD* pdwTime);
  virtual ITimer* CreateTimer(DWORD msec);
  virtual void Wait(DWORD msec);
  virtual void Idle();
  virtual Error GenerateRandom(BYTE* pData8);
  virtual Error UpdateSoftware(const char* archiveName);
  
  virtual Error MountFileSystem(const char* path, IFileSystem** fileSystem);
  
  virtual void FillModelName(char* pDest, int maxLen);
  virtual IFileSystem* GetDefaultFileSystem()
  {
    return &DefaultFS;
  };

  virtual void GetScreenSize(WORD &width, WORD &height);
};


#endif
