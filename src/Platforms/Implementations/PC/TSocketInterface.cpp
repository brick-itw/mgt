#include "Platforms/Implementations/PC/TSocketInterface.h"
#include "Base/Types/errors.h"
#include "Base/Types/sub_error.h"
#include "Common/Loggers/LogEvents.h"

#include <errno.h>
#include <stdio.h>

Error TSocketInterface::ResolveAddressUsingDNS(const char* pHostAddr, DWORD* pIpAddr)
{
   hostent* pHe = gethostbyname(pHostAddr);
   if(!pHe)
     return ERROR_DNS_FAILURE;

   *pIpAddr = *((unsigned long *)pHe->h_addr);
   return SUCCESS;
}

Error TSocketInterface::GetSelfParams(TNetworkClientParam& self_ip)
{  
  char namebuf[65];
  hostent* pHe;
  int err;

  if((err = gethostname(namebuf,sizeof(namebuf))) !=0 )
    return SubError(ERROR_SELF_PAR_FAIL, err);

  pHe = gethostbyname(namebuf);
  if(!pHe)
    return ERROR_SELF_PAR_FAIL;

  self_ip.dwAddress = *((unsigned long *)pHe->h_addr);
      
  return SUCCESS;
}

SOCKET TSocketInterface::CreateNonblockingSocket()
{
  int val = 1;

  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(INVALID_SOCKET != sock){
    SetNonblockingMode(sock);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val));
  }

  return sock;
}

const char* TSocketInterface::GetConnLogName(long sock)
{
  sprintf(__internal_buf, "sock%010lu.txt", sock);
  return __internal_buf;        
}


Error TSocketInterface::Call(const TNetworkHostParam& host, IConnection** ppConn)
{
  struct sockaddr_in  sAdrTerm;
  struct sockaddr *pA = (struct sockaddr*)&sAdrTerm;
  int len, err;

  *ppConn = NULL;
  SOCKET sock = CreateNonblockingSocket();
  if(INVALID_SOCKET == sock)
    return SubError(ERROR_SOCKET_CREATION, GetSystemError());

  sAdrTerm.sin_family      = AF_INET;
  sAdrTerm.sin_port        = htons(host.wPort);
  sAdrTerm.sin_addr.s_addr = htonl(host.dwAddress);

  len = sizeof(sAdrTerm);
  err = connect(sock, pA, len);

  if(err)
    err = GetSystemError();

  if((err == EINPROGRESS) || (err == WSAEWOULDBLOCK))
    err = 0;

  if(err)
  {
    closesocket(sock); 
    return SubError(ERROR_CONNECT_FAILED, err);
  }

  *ppConn = CreateSocketConnection(sock, TRUE, GetConnLogName((long)sock), pTraceDir);
  return SUCCESS;
}

Error TSocketInterface::Listen(WORD port, IConnection** ppConn)
{
  struct sockaddr_in  sAdrTerm;
  struct sockaddr *pA = (struct sockaddr*)&sAdrTerm;
  int len;

  *ppConn = NULL;
  SOCKET sock = CreateNonblockingSocket();
  if(INVALID_SOCKET == sock)
    return SubError(ERROR_SOCKET_CREATION, GetSystemError());

  sAdrTerm.sin_family      = AF_INET;
  sAdrTerm.sin_port        = htons(port);
  sAdrTerm.sin_addr.s_addr = INADDR_ANY;
  len = sizeof(sAdrTerm);

  if(bind(sock, pA, len) != 0)
  {
    closesocket(sock);
    return SubError(ERROR_SOCKET_BIND, GetSystemError());
  }

  if(listen(sock, SOMAXCONN) != 0)
  {
    closesocket(sock);
    return SubError(ERROR_SOCKET_LISTEN, GetSystemError());
  }


  *ppConn = CreateSocketConnection(sock, FALSE, GetConnLogName((long)sock), pTraceDir);
  return SUCCESS;
}

Error TSocketInterface::Ping(DWORD dwAddress)
{
  return ERROR_NO_SUPPORT;
}

void TSocketConnection::CheckSocketState()
{
  fd_set rdfds, wrfds, errfds;
  struct  timeval timeout;

  timeout.tv_sec  = 0;
  timeout.tv_usec = 0;

  FD_ZERO(&rdfds);
  FD_SET(Socket, &rdfds);

  FD_ZERO(&wrfds);
  FD_SET(Socket, &wrfds);

  FD_ZERO(&errfds);
  FD_SET(Socket, &errfds);

  SetDefState();
  if(select((int)Socket + 1, &rdfds, &wrfds, &errfds, &timeout) <= 0)
    return;

  bHasError       = FD_ISSET(Socket, &errfds);
  bReadyToSend    = FD_ISSET(Socket, &wrfds);
  bReadyToReceive = FD_ISSET(Socket, &rdfds);
}

int TSocketConnection::Send(const BYTE* dataOut, DWORD dataLen)
{
  CheckSocketState();
  if(bHasError)
  {
    LastError = ERROR_LINK_BROKEN;
    InternalState = CST_BROKEN;
    LogData(TConnError(LastError));
    return -1;
  }
  if(!bReadyToSend)
    return 0;

  int res = send(Socket, (const char*)dataOut, dataLen, 0);
  if(SOCKET_ERROR == res)
  {
    LastError = SubError(ERROR_LINK_PROBLEM, GetSystemError());
    InternalState = CST_BROKEN;
    return -1;
  }
  if(res > 0)
    LogData(TSent(T_BinaryData(dataOut, res)));
  return res;
}

int TSocketConnection::Receive(BYTE* dataIn,  DWORD maxLen)
{
  CheckSocketState();
  if(bHasError)
  {
    LastError = ERROR_LINK_BROKEN;
    InternalState = CST_BROKEN;
    LogData(TConnError(LastError));
    return -1;
  }

  if(!bReadyToReceive)
    return 0;

  int res = recv(Socket, (char*)dataIn, maxLen, 0);
  if(res == 0)
  {
    LastError = ERROR_LINK_BROKEN;
    InternalState = CST_BROKEN;
    return -1;
  };
  if(SOCKET_ERROR == res)
  {
    LastError = SubError(ERROR_LINK_PROBLEM, GetSystemError());
    InternalState = CST_BROKEN;
    return -1;
  }
  if(res > 0)
    LogData(TReceived(T_BinaryData(dataIn, res)));
  return res;
}

CONNSTATE TSocketConnection::GetState()
{
  if(InternalState != CST_BROKEN)
  {
    CheckSocketState();
    if(bHasError)
      InternalState = CST_BROKEN;
  
    if(InternalState == CST_CALLING)
    {
      if(bReadyToSend && bIsCalling)
      {
        InternalState = CST_ONLINE;
        LogData(TConnected(""));
      }
      else
      if(bReadyToReceive && !bIsCalling)
      {
        struct sockaddr_in sadr;
        socklen_t len = sizeof(sadr);
        SOCKET s = accept(Socket, (struct sockaddr*)&sadr, &len);
        if(INVALID_SOCKET != s)
        {
          dwPeerAddress = sadr.sin_addr.s_addr;
          closesocket(Socket);
          Socket = s;
          InternalState = CST_ONLINE;
          in_addr addr;
          addr.s_addr = dwPeerAddress;
          LogData(TConnected((const char*)inet_ntoa(addr)));
        }
      }
    }
  }
  return InternalState;
}

TSocketConnection::~TSocketConnection()
{
  shutdown(Socket, TRUE);
  closesocket(Socket);
  LogData(TDisconnected());
}
