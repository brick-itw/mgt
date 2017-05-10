#ifndef _LIN_ETHERNET_H
#define _LIN_ETHERNET_H

#include "Platforms/Interfaces/IConnection.h"
#include "Platforms/Interfaces/IFileSystem.h"
//#include "Common/Loggers/TConnectionLogger.h"
#include <sys/socket.h>
#include "Platforms/Implementations/pc/TSocketInterface.h"

class TLinEthernetInterface : public TSocketInterface {
protected:
  virtual void SetNonblockingMode(SOCKET sock);

public:
  TLinEthernetInterface(IFileSystem* ptraces = NULL)
                      : TSocketInterface(ptraces)  
  {
  };

  virtual Error Open();
  virtual void Close();

  virtual IConnection* CreateSocketConnection(SOCKET s, BOOLEAN bcalling, 
	            const char* logName = NULL, IFileSystem* pLogFileSystem = NULL);

  virtual DWORD GetSystemError();
};

class TLinEthernetConnection : public TSocketConnection {

public:
  TLinEthernetConnection(SOCKET s, BOOLEAN bcalling, 
                         const char* logName = NULL, IFileSystem* pLogFileSystem = NULL) 
                       : TSocketConnection(s, bcalling, logName, pLogFileSystem)
  {
  };

  virtual DWORD GetSystemError();
};


#endif
