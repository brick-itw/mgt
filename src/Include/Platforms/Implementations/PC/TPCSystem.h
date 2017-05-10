#ifndef _PC_SYSTEM_H
#define _PC_SYSTEM_H

#include "Base/Types/inttypes.h"
#include "Platforms/Interfaces/ISystem.h"
#include "Platforms/Implementations/TSystem.h"

class TPCTimer : public ITimer{
protected:
  DWORD    dwStartedAt;
  DWORD    dwTargetAt;
  BOOLEAN  bOverlapping;

  virtual DWORD GetMsecCounter() = 0;

  void Setup(DWORD msec)
  {
    dwStartedAt = GetMsecCounter();
    dwTargetAt  = dwStartedAt + msec;
    bOverlapping = (dwTargetAt < dwStartedAt);
  };


public:

  TPCTimer()
  {
  };


  virtual BOOLEAN IsExpired()
  {
    DWORD dwCurrentMsec = GetMsecCounter();
    return (dwCurrentMsec >= dwTargetAt) || ((dwCurrentMsec < dwStartedAt) && !bOverlapping);
  };

  virtual void Restart(DWORD newMsec = 0)
  {
    Setup(newMsec);
  };

};


class TPCSystem : public TSystem {

public:
  TPCSystem();
  ~TPCSystem();

  virtual Error Shutdown();
  virtual Error Restart();
  virtual Error UpdateSoftware(const char* archiveName);
  //virtual IMaintenance* CreateMaintenanceInstance(MAINTENANCE maintenanceType);
  virtual void FillOsVersion(char* pDest, int maxLen);

  virtual Error SetDateTime(DWORD   dwDate, DWORD   dwTime);
  virtual Error GenerateRandom(BYTE* pData8);
  virtual void Idle();
  
};


#endif
