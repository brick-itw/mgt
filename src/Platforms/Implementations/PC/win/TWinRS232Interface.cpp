#include "Platforms/Implementations/Pc/win/TWinRS232Interface.h"
#include "Platforms/Implementations/Pc/win/TWinError.h"
#include <stdio.h>

#define COM_BUFF_SIZE_SEND    8192
#define COM_BUFF_SIZE_RECV    8192

int TWinRS232Connection::SendData(const BYTE* dataOut, DWORD dataLen)
{
  DWORD res=0;

  if(!WriteFile (hFile,  dataOut, dataLen, &res, NULL)){
    LastError = WinError(ERROR_LINK_BROKEN);
    return -1;
  }

  return (int)res;
}

int TWinRS232Connection::ReceiveData(BYTE* dataIn, DWORD maxLen)
{
  DWORD err=0, res=0;

  if(!ReadFile (hFile,  dataIn, maxLen, &res, NULL)){
    LastError = WinError(ERROR_LINK_BROKEN);
    return -1;
  }

  if(!ClearCommError(hFile, &err, NULL)) {
    LastError = WinError(ERROR_LINK_PROBLEM);
    return -1;
  }

  return (int)res;
}

Error TWinRS232Interface::Connect(const TRs232Params& par, IConnection** ppConn)
{
  HANDLE hdl;
  char fname[16];
  DCB dBlock;
  COMMTIMEOUTS tMo;

  *ppConn = NULL;

  sprintf(fname, "\\\\.\\COM%d", par.PortNumber);
  hdl = CreateFile(fname,GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

  if(hdl == INVALID_HANDLE_VALUE)
    return WinError(ERROR_CONNECT_FAILED);

  SetupComm(hdl, COM_BUFF_SIZE_SEND, COM_BUFF_SIZE_RECV);

  if(GetCommState(hdl,&dBlock)){
    dBlock.BaudRate = par.dwSpeed;
    dBlock.fBinary           = 1;                        // binary mode, no EOF check
    dBlock.fParity           = 1;                        // enable parity checking
    dBlock.fOutxCtsFlow      = 0;
    dBlock.fOutxDsrFlow      = 0;
    dBlock.fDtrControl       = DTR_CONTROL_ENABLE;       // DTR flow control type
    dBlock.fDsrSensitivity   = 0;
    dBlock.fTXContinueOnXoff = 0;
    dBlock.fOutX             = 0;
    dBlock.fInX              = 0;
    dBlock.fErrorChar        = 0;
    dBlock.fNull             = 0;
    dBlock.fRtsControl       = RTS_CONTROL_ENABLE;       // RTS flow control
    dBlock.fAbortOnError     = 0;
    dBlock.ByteSize          = par.DataBits;  // number of bits/BYTE, 4-8
    dBlock.Parity            = (par.Parity   == PAR_NONE) ? 0 : ((par.Parity == PAR_ODD) ? 1 : 2);
    dBlock.StopBits          = (par.StopBits == STB_ONE)  ? 0 : 2;      // 0,1,2 = 1, 1.5, 2
    dBlock.EvtChar           = 0;
    dBlock.EofChar           = 0;

    if(SetCommState(hdl,&dBlock)){
      memset(&tMo,0,sizeof(tMo));
      tMo.ReadIntervalTimeout = MAXDWORD;
      tMo.ReadTotalTimeoutConstant = 10;
      if(SetCommTimeouts(hdl,&tMo)){
        ClearCommError(hdl,NULL,NULL);
        PurgeComm(hdl, PURGE_RXCLEAR);
      }
    }
  }

  *ppConn = new TWinRS232Connection(hdl, GetConnLogName(par.PortNumber), pTraceDir);
  return SUCCESS;
}

void TWinRS232Connection::CloseConnection()
{
	CloseHandle(hFile);
}