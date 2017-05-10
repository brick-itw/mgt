#ifndef _U2_LIN_RS232_H
#define _U2_LIN_RS232_H

#include "Platforms/Implementations/PC/TRS232Interface.h"
#include "Platforms/Implementations/PC/lin/TLinRS232Interface.h"
#include "Platforms/Implementations/PC/lin/TLinError.h"


class TLinRS232Interface : public TRS232Interface {

public:
  TLinRS232Interface(IFileSystem* ptraces = NULL)
                    : TRS232Interface(ptraces)
  {
  };

  virtual Error Connect(const TRs232Params& par, IConnection** ppConn);
};


class TLinRS232Connection : public TRS232Connection {//{public IConnection, public TConnectionLogger
protected:
  long    hFile;
  void	  CloseConnection();

public:
  TLinRS232Connection(long h, 
          	  	  	  const char* logName = NULL, IFileSystem* pLogFileSystem = NULL)
    				: TRS232Connection(logName, pLogFileSystem)
  {
    hFile = h;
  }

  virtual ~TLinRS232Connection()
  {
    CloseConnection();
  };

  virtual int SendData   (const BYTE* dataOut, WORD dataLen);
  virtual int ReceiveData(BYTE*       dataIn,  WORD maxLen);

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
