/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Interface of operation system presentation. Entry point. First object for creation.
 */

#ifndef _U2_SYSTEM_H
#define _U2_SYSTEM_H

#include "Platforms/Interfaces/IDevice.h"
#include "Platforms/Interfaces/IDeviceLayer.h"
#include "Platforms/Interfaces/IFileSystem.h"
#include "Base/Types/modules.h"
#include "Platforms/Interfaces/ITimer.h"

#include <map>

#define SYSTEM_ERROR(code) REGISTER_ERROR(MODULE_SYSTEM, (code))

enum SystemErrorCode
{
  ERROR_SYSTEM_INTERNAL = SYSTEM_ERROR(1),
};

class ISystem {
 
public:

  static ISystem* Instance();

  virtual ~ISystem(){};

  virtual Error Shutdown() = 0;
  virtual Error Restart()  = 0;
  virtual Error UpdateSoftware(const char* archiveName) = 0;

  virtual IDeviceLayer* GetDeviceLayer() = 0;

  virtual Error SetDateTime(DWORD   dwDate, DWORD   dwTime) = 0;
  virtual Error GetDateTime(DWORD* pdwDate, DWORD* pdwTime) = 0;
  virtual ITimer* CreateTimer(DWORD msec) = 0;
  virtual void Wait(DWORD msec) = 0;
  virtual Error GenerateRandom(BYTE* pData8) = 0;
  virtual void Idle() = 0;

  virtual Error MountFileSystem(const char* path, IFileSystem** fileSystem) = 0;
  virtual Error UnmountFileSystem(IFileSystem* fileSystem) = 0;

  virtual IFileSystem* GetDefaultFileSystem() = 0;//returns default mounted filesystem, or NULL, if mount failed

  virtual void FillModelName(char* pDest, int maxLen) = 0;
  virtual void FillOsVersion(char* pDest, int maxLen) = 0;

  virtual void GetScreenSize(WORD &width, WORD &height) = 0;

  virtual void StartInDebugMode(){}
};

#endif

