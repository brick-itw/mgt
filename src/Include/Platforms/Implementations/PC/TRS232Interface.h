#ifndef _U2_PC_RS232_H
#define _U2_PC_RS232_H

#include "Platforms/Interfaces/IConnection.h"
#include "Platforms/Interfaces/ISystem.h"
#include "Base/Types/error.h"
#include "Common/Loggers/LogEvents.h"
#include "Common/Loggers/TLoggerToFile.h"
#include "Platforms/Interfaces/IRS232Interface.h"

class TRS232Interface : public IRS232Interface {
protected:
  IFileSystem* pTraceDir;
  char __internal_buf[100];        

  const char* GetConnLogName(long portID);

public:
  TRS232Interface(IFileSystem* ptraces = NULL)
  {
    pTraceDir = ptraces;
  };

  virtual Error Open()
  {
    return SUCCESS;
  };

  virtual void Close()
  {
  };
};

class TRS232Connection : public IConnection, public TLoggerToFile {
protected:
  Error   LastError;

public:
  TRS232Connection(const char* logName = NULL, IFileSystem* pLogFileSystem = NULL) : TLoggerToFile(logName, pLogFileSystem), LastError(SUCCESS)
  {
    LogData(TConnected(""));
  }

  virtual ~TRS232Connection()
  {
    LogData(TDisconnected());
  };

  virtual int SendData   (const BYTE* dataOut, DWORD dataLen) = 0;
  virtual int ReceiveData(BYTE*       dataIn,  DWORD maxLen) = 0;

  virtual int Send   (const BYTE* dataOut, DWORD dataLen);
  virtual int Receive(BYTE*       dataIn,  DWORD maxLen);

  virtual CONNSTATE GetState()
  {
    return CST_ONLINE;
  };

  virtual Error GetLastError()
  {
    return LastError;
  };

  virtual DWORD GetPeerAddress()
  {
    return 0;
  };

};

#endif
