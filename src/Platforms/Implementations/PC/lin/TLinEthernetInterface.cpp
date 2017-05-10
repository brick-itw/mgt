#include "Platforms/Implementations/PC/lin/TLinEthernetInterface.h"
#include "Platforms/Implementations/PC/lin/TLinError.h"
//#include "error.h"
#include "Base/Types/sub_error.h"
#include <errno.h>
#include <sys/socket.h>

Error TLinEthernetInterface::Open()
{
  return SUCCESS;
}

void TLinEthernetInterface::Close()
{
}

void TLinEthernetInterface::SetNonblockingMode(SOCKET sock)
{
  fcntl(sock, F_SETFL, O_NONBLOCK);
}

IConnection* TLinEthernetInterface::CreateSocketConnection(SOCKET s, BOOLEAN bcalling,
	                            const char* logName, IFileSystem* pLogFileSystem)
{
  return new TLinEthernetConnection(s, bcalling, logName, pLogFileSystem);
}

DWORD TLinEthernetInterface::GetSystemError()
{
  return errno;
}

DWORD TLinEthernetConnection::GetSystemError()
{
  return errno;
}



