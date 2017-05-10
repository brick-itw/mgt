#include "Platforms/Implementations/PC/lin/TLinRS232Interface.h"
#include "Platforms/Implementations/PC/lin/TLinError.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

#define COM_BUFF_SIZE_SEND    8192
#define COM_BUFF_SIZE_RECV    8192

int WaitSocketReadyForWriting(int hdl)
{
  int res = 0;
  int err;

  fd_set TheSet;
  struct  timeval timeout;

  timeout.tv_sec  = 3;
  timeout.tv_usec = 0;

  FD_ZERO(&TheSet);
  FD_SET(hdl, &TheSet);

  if(select(hdl + 1, 0, &TheSet, 0, &timeout) > 0L)
  {
    res = 1;
  }

  return res;
}


int TLinRS232Connection::SendData(const BYTE* dataOut, WORD dataLen)
{
  int cLen, portion;
  int res;

  if(!WaitSocketReadyForWriting(hFile))
  {
	LastError = LinError(ERROR_LINK_BROKEN);
	return -1;
  }
  res = write(hFile, dataOut+cLen, dataLen);
  if (res < 0)
  {
	LastError = LinError(ERROR_LINK_BROKEN);
	return -1;
  }
  return res;
}

int TLinRS232Connection::Receive(BYTE* dataIn, WORD maxLen)
{
  struct  timeval timeout;
  fd_set          TheSet;
  int             res = 0;

  timeout.tv_sec  = 0;
  timeout.tv_usec = 50000;

  FD_ZERO(&TheSet);
  FD_SET(hFile, &TheSet);

  if(select(hFile + 1, &TheSet, 0, 0, &timeout) > 0L)
    res = read(hFile, dataIn, maxLen);

  return res;
}

Error TLinRS232Interface::Connect(const TRs232Params& par, IConnection** ppConn)
{
  struct termios ComSett;
  char           buf[120];
  long           hdl;
  int            bd = B115200;


  *ppConn = NULL;

  sprintf(buf, "/dev/ttyS%d", par.PortNumber - 1);
  hdl = open(buf, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (hdl < 0)
	  return LinError(ERROR_CONNECT_FAILED);

  switch(par.dwSpeed)
  {
      case 1200:
		bd = B1200;
        break;
      case 2400: 
		bd = B2400;
        break;
      case 9600: 
		bd = B9600;
        break;
      case 19200: 
		bd = B19200;         
		break;
      case 38400: 
		bd = B38400;
        break;
      case 57600: 
		bd = B57600;
        break;
      case 115200: 
		bd = B115200;
        break;
      default: 
		bd = B9600;
        break;
  };
   
  memset(&ComSett, 0, sizeof(ComSett));
  cfsetispeed(&ComSett, bd);
  cfsetospeed(&ComSett, bd);
  ComSett.c_cflag |= (CLOCAL|CREAD);
    
  if(par.Parity != PAR_NONE)
  {
	ComSett.c_cflag |= PARENB;
	if (par.Parity == PAR_ODD) 
		ComSett.c_cflag |= PARODD;
  }

  if (par.StopBits == STB_TWO)
	ComSett.c_cflag |= CSTOPB;

  switch(par.DataBits)
  {
	case 5:
		ComSett.c_cflag |= CS5;
        break;
	case 6:
		ComSett.c_cflag |= CS6;
        break;
	case 7:
		ComSett.c_cflag |= CS7;
        break;
	case 8:
		ComSett.c_cflag |= CS8;
        break;
	default:
		ComSett.c_cflag |= CS8;
        break;
  }

  ComSett.c_iflag |= INPCK;
  tcsetattr(hdl, TCSANOW, &ComSett);
  tcgetattr(hdl, &ComSett);

  *ppConn = new TLinRS232Connection(hdl, GetConnLogName(par.PortNumber), pTraceDir);
  return SUCCESS;

}

void TLinRS232Connection::CloseConnection()
{
	close(hFile);
}
