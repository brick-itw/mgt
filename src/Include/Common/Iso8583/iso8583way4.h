//=============================================================================================================================
// * Name      : iso8583way4.h
// * Author    : Alexey Brik
// * Created on: 31 дек. 2015 г.
//=============================================================================================================================

#pragma once

#include "iso8583main.h"
#include "trxtypes.h"

namespace ump
{

#define MIN_W4_MESSAGE_LEN      10
#define MAX_W4_MESSAGE_LEN    1024

enum WAY4MSG_STATE
{
  W4M_IDLE,
  W4M_WAIT_MSG,
  W4M_GOT_HEADER,
  W4M_GOT_MESSAGE,
  W4M_SENT_MESSAGE,

  W4M_WAIT_MAC_ON_REQUEST,
  W4M_WAIT_MAC_ON_REPLY,
  W4M_WAIT_RESPCODE_FROM_DB,
  W4M_WAIT_INFO_FROM_DB,

  W4M_WAIT_DISCONNECT,
  W4M_DISCONNECTING,
};

enum WAY4_MAC_MODE
{
	W4MM_NONE        = 0,
	W4MM_MAC         = 1,
	W4MM_ENCRYPT     = 3
};

class T_Way4Session : public T_Iso8583_Session
{
protected:

	WAY4MSG_STATE 			Way4State;
	T_Iso8583_StaticDialect Dialect;
	bool               		bHsmUnavailable;
	bool               		bDbUnavailable;
	UMPTRXTYPE        		 TrxType;

	T_Iso8583_Message  RequestMsg;
	T_Iso8583_Message  ReplyMsg;

	std::string        TerminalID;
	WAY4_MAC_MODE      MacMode;
	T_TLV_Buffer       RequestData;
	T_TLV_Buffer       ReplyData;
	T_TLV_Buffer       TerminalInfoFromDB;

	T_BinaryData       OutgoingMsg;


	void HandleMacReady(const T_Message& Mess);
	bool CheckInputMsg();
	void NotifyTransactionFailure(DWORD errCode, DWORD sessionID);
	void LogTerminalError(DWORD err);
	bool NeedAMac() const;
	bool AskMacFromHsm(WAY4MSG_STATE newState, T_TLV_Buffer& data);

	virtual void Init();
	virtual void HandleInitMessage();
	void SendOutgoingMsg();
	void NewState(WAY4MSG_STATE newState);
	void Disconnect();
	bool AskFromDB(const T_Message& Mess);
	void LoadTerminalID(const std::string& s);

	virtual void ProcessInputMsg(const T_BinaryData& body) = 0;
	virtual void HandleIncomingData(const T_Message& Mess) = 0;
	virtual void MacReceived(T_TLV_Item& Mac) = 0;
	virtual void HsmMacFailed(DWORD err) = 0;

public:

	T_Way4Session(DWORD dwHostID, DWORD dwInstanceID, T_MessageHandler* pM, BYTE logLevel);

	virtual ~T_Way4Session()
	{
	};

	virtual void HandleMessage(const T_Message& Mess);
	virtual void Idle();

};

class T_Way4HostSession : public T_Way4Session
{
protected:
	virtual void ProcessInputMsg(const T_BinaryData& body);
	virtual void HandleIncomingData(const T_Message& Mess);
	virtual void MacReceived(T_TLV_Item& Mac);
	virtual void HsmMacFailed(DWORD err);

	void NotifyTransactionFailedAtFinalStage(DWORD errCode);
	bool GetDataFromRequest(const T_BinaryData& body);
	bool AskTerminalInfoFromDB();
	bool AskResponseCodeFromDB(DWORD err);
	std::string GetDirectRespCode(DWORD err);
	bool PrepareMacForReply();
	void SendReplyMaybeWithMac();
	void SendDirectReply(DWORD err);
	void SendDirectReplyStage1(DWORD err);
	void SendDirectReplyStage2(std::string& respcode);
	void HandleTermInfoFromDB(const T_Message& Mess);
	void HandleRespCodeFromDB(const T_Message& Mess);
	void DeliverReply(const T_Message& Mess);
	UMPTRXTYPE GetUmpTrxTypeFromRequest(const T_BinaryData& msgtype, const T_BinaryData& proccode);
	T_BinaryData FormatReply();
	void ProceedWithRequest();
	void ProcessGoodRequestFromTerminal();


public:
	T_Way4HostSession(DWORD dwHostID, DWORD dwInstanceID, T_MessageHandler* pM, BYTE logLevel) :
		T_Way4Session(dwHostID, dwInstanceID, pM, logLevel)
	{
	};

	virtual ~T_Way4HostSession()
	{
	};

	virtual void HandleMessage(const T_Message& Mess);
	virtual void Idle();
};

}

//-----------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------
