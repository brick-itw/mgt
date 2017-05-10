#include "Common/Application/porthandler.h"
#include "Platforms/Interfaces/ISystem.h"
#include "Common/Utils/byteorder.h"

#define LEN_HEADER_SIZE  2

static DWORD dwHandlerID = 0;

TMgtPortHandler::TMgtPortHandler(IConnection* plink) : pLink(plink), CurrentChunk(), id(++dwHandlerID)
{
}

TMgtPortHandler::~TMgtPortHandler()
{
  if(pLink)
    delete pLink;
}

BOOLEAN TMgtPortHandler::IsConnected()
{
  if(!pLink)
    return FALSE;
  return (pLink->GetState() == CST_ONLINE);
}

BOOLEAN TMgtPortHandler::IsLinkBroken()
{
  if(!pLink)
    return FALSE;
  return (pLink->GetState() == CST_BROKEN);
}

BOOLEAN TMgtPortHandler::IsCurrentChunkComplete()
{
  if(CurrentChunk.Length() < LEN_HEADER_SIZE)
    return FALSE;
  return ((MSB_TO_WORD(CurrentChunk.GetData())+LEN_HEADER_SIZE) <= (WORD)CurrentChunk.Length());
}

void TMgtPortHandler::ReadNextDataPortionIntoCurrentChunk()
{
#define READ_PORTION_SIZE 1024
  BYTE  buf[READ_PORTION_SIZE];
  int Count = pLink->Receive(buf, sizeof(buf));
  if(Count > 0)
    CurrentChunk += T_BinaryData(buf, Count);
}


BOOLEAN TMgtPortHandler::GetIncomingRequest(T_Trivial_TLV_Buffer& Request)
{
  if(!IsConnected())
    return FALSE;

  ReadNextDataPortionIntoCurrentChunk();

  if(IsCurrentChunkComplete())
  {
    Request = T_Trivial_TLV_Buffer(CurrentChunk.Copy(LEN_HEADER_SIZE, MSB_TO_WORD(CurrentChunk.GetData())));
    CurrentChunk.Cut(0, LEN_HEADER_SIZE+MSB_TO_WORD(CurrentChunk.GetData()));
    return TRUE;
  }
  return FALSE;
}

BOOLEAN TMgtPortHandler::SendOutgoingReply(const T_Trivial_TLV_Buffer& Reply)
{
  if(!IsConnected())
    return FALSE;

  BYTE lenbuf[2];
  WORD_TO_MSB((WORD)Reply.GetLength(), lenbuf);
  T_BinaryData FullMessage(T_BinaryData(lenbuf, sizeof(lenbuf)) + Reply.AsBinary());
  return (pLink->Send(FullMessage.GetData(), FullMessage.Length()) == FullMessage.Length());
}

