#ifndef _MGT_WAY4_AUTH_LINK
#define _MGT_WAY4_AUTH_LINK

#include "Common/Iso8583/Way4Dialect.h"

class TWay4TerminalLink{
protected:
  IConnection* pLink;

public:
  TWay4TerminalLink(TConnectionParams* pPar, DWORD termID);
  ~TWay4TerminalLink();

  NETWORK_STATE GetConnectionState();

  Error SendAuthRequest(const T_TLV_Buffer& req_data);
  Error GetAuthReply(T_TLV_Buffer& resp_data);
  
};

#endif

