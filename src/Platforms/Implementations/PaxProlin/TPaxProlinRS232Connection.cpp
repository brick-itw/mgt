#include "Platforms/Implementations/PaxProlin/TPaxProlinRS232Interface.h"
#include "osal.h"
#include <stdio.h>
#include <termios.h>

#define COM_BUFF_SIZE_SEND    8192
#define COM_BUFF_SIZE_RECV    8192

int TPaxProlinRS232Connection::SendData(const BYTE* dataOut, WORD dataLen)
{
	int res;
	
	res = OsPortSend(PORT_COM1, dataOut, dataLen);
	if (res != RET_OK)
	{
		LastError = ERROR_LINK_BROKEN;
		return -1;
	}
	return dataLen;
}

int TPaxProlinRS232Connection::ReceiveData(BYTE* dataIn, WORD maxLen)
{
	int res;

	res = OsPortRecv(PORT_COM1, dataIn, maxLen, 100);
	if (res < 0)
	{
		LastError = ERROR_LINK_BROKEN;
		return -1;
	}

	return res;
}

Error TPaxProlinRS232Interface::Connect(const TRs232Params& par, IConnection** ppConn)
{
  char           paramBuf[120];
  int            bd = B115200;
  int			 dbits = 8;
  char			 parity = 'n';
  int			 stopBit = 1;
  int 			 res;

  *ppConn = NULL;

  if (par.DataBits == 7)
	dbits = 7;

  if (par.Parity != PAR_NONE)
  {
	parity = 'e';
	if (par.Parity == PAR_ODD) 
		parity = 'o';
  }

  if (par.StopBits == STB_TWO)
	 stopBit = 2;

  sprintf(paramBuf, "%lu,%d,%c,%d", par.dwSpeed, dbits, parity, stopBit);
  res = OsPortOpen(PORT_COM1, paramBuf);
  if (res != RET_OK)
	  return ERROR_CONNECT_FAILED;

  *ppConn = new TPaxProlinRS232Connection(PORT_COM1, GetConnLogName(par.PortNumber), pTraceDir);
  return SUCCESS;

}

void TPaxProlinRS232Connection::CloseConnection()
{
	OsPortClose(PORT_COM1);
}
