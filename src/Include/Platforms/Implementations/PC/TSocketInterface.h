#ifndef _SOCKET_ETHERNET_H
#define _SOCKET_ETHERNET_H

#include "Platforms/Interfaces/IConnection.h"
#include "Platforms/Interfaces/IFileSystem.h"
#include "Common/Loggers/TLoggerToFile.h"

#ifdef WIN32

  #include <winsock.h>
  #define socklen_t int
  #define EINPROGRESS WSAEWOULDBLOCK

#else
  #include <sys/socket.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <sys/ioctl.h>
  #include <stropts.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <fcntl.h>

  #define SOCKET long
  #define INVALID_SOCKET (-1)
  #define SOCKET_ERROR   (-1)
  #define WSAEWOULDBLOCK EALREADY

  #define closesocket(x) close(x)
#endif

class TSocketInterface : public INetworkInterface {
protected:
  IFileSystem* pTraceDir;
  char __internal_buf[100];        

  const char* GetConnLogName(long sock);
  virtual void SetNonblockingMode(SOCKET sock) = 0;
  SOCKET CreateNonblockingSocket();

public:
  TSocketInterface(IFileSystem* ptraces = NULL)
  {
    pTraceDir = ptraces;
  };

  virtual NWINTSTATE GetCurrentState()
  {
    return NWS_UP;
  };

  virtual Error GetNetworkError()
  {
    return SUCCESS;
  };

  virtual Error SetSelfParams(const TNetworkClientParam& self_ip)
  {
    return SUCCESS;
  };

  virtual Error GetSelfParams(TNetworkClientParam& self_ip);

  virtual IConnection* CreateSocketConnection(SOCKET s, BOOLEAN bcalling, 
	            const char* logName = NULL, IFileSystem* pLogFileSystem = NULL) = 0;

  Error Call  (const TNetworkHostParam& host, IConnection**);
  Error Listen(WORD port, IConnection**);
  Error Ping(DWORD dwAddress);

  virtual Error ResolveAddressUsingDNS(const char* pHostAddr, DWORD* pIpAddr);


  virtual DWORD GetSystemError() = 0;
};

class TSocketConnection : public IConnection, public TLoggerToFile {
protected:
  Error     LastError;
  SOCKET    Socket;
  BOOLEAN   bIsCalling, bHasError, bReadyToSend, bReadyToReceive;
  CONNSTATE InternalState;
  DWORD     dwPeerAddress;

  void SetDefState()
  {
    bHasError = bReadyToSend = bReadyToReceive = FALSE;
  };

  void CheckSocketState();

public:
  TSocketConnection(SOCKET s, BOOLEAN bcalling, const char* logName = NULL, IFileSystem* pLogFileSystem = NULL)
	          : TLoggerToFile(logName, pLogFileSystem), LastError(SUCCESS)
  {
    Socket = s;
    bIsCalling = bcalling;
    InternalState = CST_CALLING; 
    dwPeerAddress = 0;
    SetDefState();
  }

  virtual ~TSocketConnection();

  virtual int Send   (const BYTE* dataOut, DWORD dataLen);
  virtual int Receive(BYTE*       dataIn,  DWORD maxLen);

  virtual CONNSTATE GetState();

  virtual Error GetLastError()
  {
    return LastError;
  };

  virtual DWORD GetPeerAddress()
  {
    return dwPeerAddress;
  };

  virtual DWORD GetSystemError() = 0;

};




#endif
