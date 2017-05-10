#include "Platforms/Implementations/PC/TRS232Interface.h"
#include "error.h"
#include <stdio.h>

int TRS232Connection::Send(const BYTE* dataOut, DWORD dataLen)
{
  int res = SendData(dataOut, dataLen);
  if(res > 0)
    LogData(TSent(T_BinaryData(dataOut, (DWORD)res)));
  return res;
}

int TRS232Connection::Receive(BYTE* dataIn, DWORD maxLen)
{
  int res = ReceiveData(dataIn, maxLen);
  if(res > 0)
    LogData(TReceived(T_BinaryData(dataIn, (DWORD)res)));
  return (int)res;
}

const char* TRS232Interface::GetConnLogName(long portID)
{
  sprintf(__internal_buf, "CommLog%ld.txt", portID);
  return __internal_buf;        
}


