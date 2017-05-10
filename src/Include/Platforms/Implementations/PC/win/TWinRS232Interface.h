#ifndef _WIN_RS232_H
#define _WIN_RS232_H

#include "Platforms/Implementations/Pc/TRS232Interface.h"
#include <windows.h>

class TWinRS232Interface : public TRS232Interface {

public:
  TWinRS232Interface(IFileSystem* ptraces = NULL) 
                    : TRS232Interface(ptraces) 
  {
  };

  virtual Error Connect(const TRs232Params& par, IConnection** ppConn);
};

class TWinRS232Connection : public TRS232Connection {
protected:
  HANDLE  hFile;
  void CloseConnection();

public:
  TWinRS232Connection(HANDLE h, 
                      const char* logName = NULL, IFileSystem* pLogFileSystem = NULL) 
  		      : TRS232Connection(logName, pLogFileSystem)
  {
    hFile = h;
  }

  virtual ~TWinRS232Connection()
  {
	  CloseConnection();
  };

  virtual int SendData   (const BYTE* dataOut, DWORD dataLen);
  virtual int ReceiveData(BYTE*       dataIn,  DWORD maxLen);
};

#endif
