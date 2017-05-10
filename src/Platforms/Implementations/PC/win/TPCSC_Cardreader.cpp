#include "Platforms/Implementations/PC/win/TPCSC_CardReader.h"
#include "Platforms/Interfaces/ISystem.h"
#include "Common/Utils/FormatConverters.h"
#include "Common/Loggers/TLoggerToFile.h"
#include "Common/Loggers/ILogger.h"

const char* RegPathPCSC = "SOFTWARE\\Microsoft\\Cryptography\\Calais\\Readers";

TPCSC_CardReader::TPCSC_CardReader(IFileSystem* ptr) : IsContactless(false)
{
  hSC   = 0;
  hCard = 0;
  ATR.AtrLength = 0;
  dwProtocol = 0;
  pTraces = ptr;
  GetReaderNames();
}

TPCSC_CardReader::~TPCSC_CardReader()
{
  Disable();
}

std::string TPCSC_CardReader::TraceFileName() const
{
  DWORD dwDate;
  ISystem::Instance()->GetDateTime(&dwDate, 0);
  return Name + "_" + IntToStdString(dwDate, 8) + "_trace.txt";
}

void TPCSC_CardReader::GetReaderNames()
{
  HKEY  hRegKey;
  DWORD dwSize, dwIndex;
  FILETIME ftime;
  char namebuf[256];

  ReaderNames.clear();

  if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                  RegPathPCSC,
                  0L,
                  KEY_READ,
                  &hRegKey) == ERROR_SUCCESS){
    for(dwIndex=0 ; ; ++dwIndex)
    {
      dwSize = sizeof(namebuf);
      namebuf[0] = 0;
      if(ERROR_SUCCESS != RegEnumKeyEx(hRegKey, dwIndex, namebuf, &dwSize, NULL, NULL, NULL, &ftime))
        break;
      ReaderNames.push_back(namebuf);
    }
    RegCloseKey(hRegKey);
  }
}

BOOLEAN TPCSC_CardReader::CtlsCardTapped()
{
  if(!hSC)
    return FALSE;

  if(!hCard)
    if(!OpenCard())
      return FALSE;

  return IsContactless;
} 

void TPCSC_CardReader::LogAtr()
{  
  TLoggerToFile(TraceFileName().c_str(), pTraces).LogData(TLogEvent(TLogEvent::ICC_ATR, ATR.AtrData, (WORD)ATR.AtrLength));
}

void TPCSC_CardReader::LogPowerOff()
{
  TLoggerToFile(TraceFileName().c_str(), pTraces).LogData(TLogEvent(TLogEvent::CONN_DISCONNECT));
}

void TPCSC_CardReader::LogApdu(TApdu& apdu)
{
  TLoggerToFile Logger(TraceFileName().c_str(), pTraces);
  char buf[128];
  sprintf(buf, "CLA=%02X INS=%02X P1=%02X P2=%02X Lc=%3d", apdu.CLA, apdu.INS, apdu.P1, apdu.P2, apdu.Lc);  
  Logger.LogData(TLogEvent(TLogEvent::ICC_APDU_CMD, (BYTE*)buf, strlen(buf)));
  if(apdu.Lc)    
    Logger.LogData(TLogEvent(TLogEvent::ICC_APDU_DATA,  apdu.DataIn, apdu.Lc));
  if(apdu.GetReplyLength())    
    Logger.LogData(TLogEvent(TLogEvent::ICC_APDU_REPLY, apdu.DataOut, apdu.GetReplyLength()));
  sprintf(buf, "SW=%04X\r\n", apdu.sw1sw2);  
  Logger.LogData(TLogEvent(TLogEvent::ICC_APDU_SW, (BYTE*)buf, strlen(buf)));
}

BOOLEAN TPCSC_CardReader::OpenCard()
{
  for(std::list<std::string>::const_iterator it = ReaderNames.cbegin();
      (it != ReaderNames.cend()) && (hCard == 0);
      ++it)
  {
    Name = *it;
    SCardConnect(hSC,
                 Name.c_str(),
                 SCARD_SHARE_EXCLUSIVE,
                 SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
                 &hCard,
                 &dwProtocol);
  }

  IsContactless = (Name.find("-CL") != std::string::npos);
  return (hCard != 0);
}

BOOLEAN  TPCSC_CardReader::CardInserted()
{
  DWORD   dwState, cch = 0;

  if(!hSC)
    return FALSE;

  if(!hCard)
    if(!OpenCard())
      return FALSE;

  if(IsContactless)
    return FALSE;

  ATR.AtrLength = MAX_ATR_LEN;
  if(SCARD_S_SUCCESS == SCardStatus(hCard,
                                    NULL,
                                    &cch,
                                    &dwState,
                                    &dwProtocol,
                                    ATR.AtrData,
                                    &ATR.AtrLength)) 
  {
    if(dwState >= SCARD_POWERED)
      return TRUE;
  }
  else
    PowerOffCard();

  return FALSE;
}

Error TPCSC_CardReader::ResetICC(TAtrData& atr)
{
  if(!CardInserted() && !IsContactless)
    return ERROR_CARD_REMOVED;

  atr = ATR;
  LogAtr();
  return SUCCESS;
}

Error TPCSC_CardReader::ExchangeAPDU(TApdu& Apdu)
{
  DWORD dwSend, dwRecv;
  BYTE apdu_in [270];
  BYTE apdu_out[270];

  if(!CardInserted() && !IsContactless)
    return ERROR_CARD_REMOVED;
  
  dwSend = Apdu.BuildCmdBuf(apdu_in);
  dwRecv = 258;

  if(SCARD_S_SUCCESS != SCardTransmit(hCard, NULL, apdu_in, dwSend, NULL, apdu_out, &dwRecv))
    return ERROR_CARD_MUTE;
  Apdu.ParseReplyBuf(apdu_out, (WORD)dwRecv);

  if(((apdu_out[dwRecv-2] == 0x61) || (apdu_out[dwRecv-2] == 0x6C)) && (dwProtocol != SCARD_PROTOCOL_T1)){
    apdu_in[4] = apdu_out[dwRecv-1];
    if(apdu_out[dwRecv-2] != 0x6C){
      apdu_in[0] = 0;
      apdu_in[1] = 0xC0;
      apdu_in[2] = 0;
      apdu_in[3] = 0;
      dwSend     = 5;
    }
    dwRecv = apdu_out[dwRecv-1] + 2;
    if(SCARD_S_SUCCESS != SCardTransmit(hCard, NULL, apdu_in, dwSend, NULL, apdu_out, &dwRecv))
      return ERROR_CARD_MUTE;
  }

  Apdu.ParseReplyBuf(apdu_out, (WORD)dwRecv);
  LogApdu(Apdu);
  return SUCCESS;
}

void  TPCSC_CardReader::PowerOffCard()
{
  if(hCard)
  {
    SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
    hCard = 0;
    LogPowerOff();
  }
  Name.clear();
  ATR.AtrLength = 0;
  dwProtocol = 0;
}

Error TPCSC_CardReader::Enable (BOOLEAN bEnableMagn, BOOLEAN bEnableICC, BOOLEAN bEnableCTLS)
{
  Disable();

  if(bEnableICC)
    SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hSC);

  return hSC ? SUCCESS : ERROR_CRD_OPEN_FAILURE;
}

void  TPCSC_CardReader::Disable()
{
  PowerOffCard();
  if(hSC){
    SCardReleaseContext(hSC);
    hSC = 0;
  }
}

BOOLEAN TPCSC_CardReader::MagnCardSwiped() 
{
  return false;
}; 

Error TPCSC_CardReader::ReadMagnCard (TMagnCardInfo& Card) 
{
  return ERROR_CARD_NOT_PRESENT;
};

