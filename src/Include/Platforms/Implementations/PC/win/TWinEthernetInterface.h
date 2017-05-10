#ifndef _WIN_ETHERNET_H
#define _WIN_ETHERNET_H

#include "Platforms/Interfaces/IConnection.h"
#include "Common/Loggers/TLoggerToFile.h"
#include <windows.h>
#include <winsock.h>
#include "Platforms/Implementations/Pc/TSocketInterface.h"

class TWinEthernetInterface : public TSocketInterface {
protected:
  virtual void SetNonblockingMode(SOCKET sock);

public:
  TWinEthernetInterface(IFileSystem* ptraces = NULL) : TSocketInterface(ptraces)  
  {
  };

  virtual Error SetSelfParams(const TNetworkClientParam& self_ip)
  {
    return SUCCESS;
  }
  virtual Error GetSelfParams(TNetworkClientParam& self_ip);

  virtual Error Open();
  virtual void Close();

  virtual IConnection* CreateSocketConnection(SOCKET s, BOOLEAN bcalling, const char* logName = NULL, IFileSystem* pLogFileSystem = NULL);

  virtual DWORD GetSystemError();
};

class TWinEthernetConnection : public TSocketConnection {

public:
  TWinEthernetConnection(SOCKET s, BOOLEAN bcalling, 
                         const char* logName = NULL, IFileSystem* pLogFileSystem = NULL) 
                       : TSocketConnection(s, bcalling, logName, pLogFileSystem)
  {
  };

  virtual DWORD GetSystemError();
};


#endif
