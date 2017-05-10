#ifndef _MGT_PORT_HANDLER_H
#define _MGT_PORT_HANDLER_H

#include "Common/Utils/T_Trivial_TLV.h"
#include "Platforms/Interfaces/IConnection.h"

class TMgtPortHandler{
protected:
  DWORD id;
  IConnection* pLink;
  T_BinaryData CurrentChunk;

  BOOLEAN IsCurrentChunkComplete();
  void ReadNextDataPortionIntoCurrentChunk();

public:
  TMgtPortHandler(IConnection* plink);
  ~TMgtPortHandler();

  BOOLEAN IsConnected();
  BOOLEAN IsLinkBroken();
  BOOLEAN GetIncomingRequest(T_Trivial_TLV_Buffer& Request);
  BOOLEAN SendOutgoingReply(const T_Trivial_TLV_Buffer& Reply);
  DWORD GetID(){return id;};
};

#endif

