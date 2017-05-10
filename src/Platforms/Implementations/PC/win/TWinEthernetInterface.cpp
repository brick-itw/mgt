#include "Base/Types/error.h"
#include "Common/Utils/FormatConverters.h"
#include "Platforms/Implementations/Pc/win/TWinEthernetInterface.h"
#include "Base/Types/sub_error.h"
#include <winsock.h>
#include <stdio.h>

Error TWinEthernetInterface::Open()
{
  WSADATA wsaData;
  WSAStartup(MAKEWORD((2),(1)), &wsaData);

  return SUCCESS;
}

void TWinEthernetInterface::Close()
{
  WSACleanup();
}

void TWinEthernetInterface::SetNonblockingMode(SOCKET sock)
{
  DWORD flag = 1;
  ioctlsocket(sock, FIONBIO, &flag);
}

Error TWinEthernetInterface::GetSelfParams(TNetworkClientParam& self_ip)
{
  char buf[256];
  gethostname(buf, sizeof(buf));
  hostent* pEntry = gethostbyname(buf);
  if(!pEntry)
    return ERROR_NO_SUPPORT;
 
  self_ip.dwAddress =((struct in_addr*)pEntry->h_addr_list[0])->s_addr;
  return SUCCESS;
}

IConnection* TWinEthernetInterface::CreateSocketConnection(SOCKET s, BOOLEAN bcalling, 
	                            const char* logName, IFileSystem* pLogFileSystem)
{
  return new TWinEthernetConnection(s, bcalling, logName, pLogFileSystem);
}

DWORD TWinEthernetInterface::GetSystemError()
{
  return WSAGetLastError();
}

DWORD TWinEthernetConnection::GetSystemError()
{
  return WSAGetLastError();
}


