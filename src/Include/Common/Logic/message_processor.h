#ifndef _MGT_MESSAGE_PROCESSOR_H
#define _MGT_MESSAGE_PROCESSOR_H

#include "Common/Logic/trivial_tlv.h"
#include "Common/Iso8583/Way4Dialect.h"

class TMessageProcessor{
protected:
  IConnection* pConnection;

  T_BinaryData PickFreeTerminalID();

  TWay4Message BuildAuthRequest(const T_TLV_Buffer& trxdata);

  T_TLV_Buffer DecryptCardData(const T_BinaryData& encrypted_data);

  TTrivialTlvBuffer GetMessage();
  void ProcessMessage(const TTrivialTlvBuffer& msg);

public:
  TMessageProcessor(IConnection* pConn);
  void Run();
  void Stop();
};

#endif

