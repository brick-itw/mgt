#include "Platforms/Implementations/PC/win/Simulation/TSimulation_CardReader.h"
#include "Platforms/Interfaces/ISystem.h"
#include "Common/Utils/FormatConverters.h"
#include "Platforms/Implementations/PC/win/TIniFile.h"
#include <windows.h>

const char* cMagstripeFileName = "magncard.ini";
const char* cCard  = "Card";
const char* cTrack1 = "Track1";
const char* cTrack2 = "Track2";

const char* cReqMarker  = "Request:";
const char* cRespMarker = "Response:";

TSimulation_CardReader::TSimulation_CardReader(const std::string& cScriptName)
                       :State(SS_BEFORE_CARD_INSERTED), pScript(NULL), ScriptFileName(cScriptName)
{
  bEnableMagStripe = FALSE;
  bEnableICC = FALSE;
  bEnableCTLS = FALSE;
}

TSimulation_CardReader::~TSimulation_CardReader()
{
  Disable();
}

const char* TSimulation_CardReader::GetScriptFileName()
{
  return ScriptFileName.c_str();
}

bool TSimulation_CardReader::IsScriptFilePresent()
{
  return (ISystem::Instance()->GetDefaultFileSystem()->GetFileLength(GetScriptFileName()) > 0);
}

bool TSimulation_CardReader::OpenScriptFile()
{
  CloseScriptFile();
  pScript = ISystem::Instance()->GetDefaultFileSystem()->OpenForRead(GetScriptFileName());
  return (pScript != NULL);
}

void TSimulation_CardReader::CloseScriptFile()
{
  if(pScript)
  {
    delete pScript;
    pScript = NULL;
  }
}

BOOLEAN  TSimulation_CardReader::CardInserted()
{
  if(!IsScriptFilePresent() || !bEnableICC)
    return false;

  switch(State)
  {
    case SS_BEFORE_CARD_INSERTED: State = SS_CARD_INSIDE; return false;
    case SS_CARD_INSIDE: return true;
    case SS_CARD_REMOVING: State = SS_CARD_REMOVED; return true;
    case SS_CARD_REMOVED: return false;
    default: return false;
  };
}

Error TSimulation_CardReader::ResetICC(TAtrData& atr)
{
  atr.AtrLength = 0;
  State = SS_CARD_INSIDE;

  if(!OpenScriptFile())
    return ERROR_SCRIPT_FAILED_TO_LOAD;

  T_BinaryData d(ReadATR());
  atr.AtrLength = d.Length();
  memmove(atr.AtrData, d.GetData(), atr.AtrLength);
  return  SUCCESS;
}

Error TSimulation_CardReader::ExchangeAPDU(TApdu& Apdu)
{
  DWORD dwSend;
  BYTE apdu_in [270];

  if(!pScript)
    return ERROR_CARD_REMOVED;
  
  dwSend = Apdu.BuildCmdBuf(apdu_in);
  T_BinaryData d = ReadNextRequest();
  if(d != T_BinaryData(apdu_in, dwSend))
    return ERROR_REQUEST_DATA_MISMATCH;

  T_BinaryData reply = ReadNextResponse();
  if(reply.Length() < 2)
    return ERROR_CARD_MUTE;

  Apdu.ParseReplyBuf(reply.GetData(), (WORD)reply.Length());
  return SUCCESS;
}

void  TSimulation_CardReader::PowerOffCard()
{
  State = SS_CARD_REMOVING;
  CloseScriptFile();
}

Error TSimulation_CardReader::Enable (BOOLEAN enableMagn, BOOLEAN enableICC, BOOLEAN enableCTLS)
{
  Disable();
  bEnableMagStripe = enableMagn;
  bEnableICC = enableICC;
  bEnableCTLS = enableCTLS;
  return SUCCESS;
}

void  TSimulation_CardReader::Disable()
{
  PowerOffCard();
  bEnableMagStripe = FALSE;
  bEnableICC = FALSE;
  bEnableCTLS = FALSE;
  State = SS_BEFORE_CARD_INSERTED;
}

BOOLEAN TSimulation_CardReader::MagnCardSwiped() 
{
  TMagnCardInfo Card;

  return (ReadMagnCard(Card) == SUCCESS);
} 

Error TSimulation_CardReader::ReadMagnCard (TMagnCardInfo& Card) 
{
  TIniFile CStore(cMagstripeFileName);

  std::string tr1 = CStore.GetString(cCard, cTrack1);
  std::string tr2 = CStore.GetString(cCard, cTrack2);

  if((tr2.length() < 20) || !bEnableMagStripe)
    return ERROR_CARD_NOT_PRESENT;

  strcpy(Card.Track1, tr1.c_str());
  strcpy(Card.Track2, tr2.c_str());
  return SUCCESS;
}



TCollis_CardReader::TCollis_CardReader(const std::string& cScriptName) 
  : TSimulation_CardReader(cScriptName)
{
}

std::string TCollis_CardReader::ReadNextLineFromScript()
{
  char c;
  std::string s;

  if(pScript)
  {
    for(;;)
    {
      if(pScript->Read((BYTE*)&c, 1) != 1)
      {
        CloseScriptFile();
        break;
      }
      if(c == 0x0A)
        break;
      else
      if((c != 0x0D) && (c != ' ') && (c != 9))
        s += c;
    }
  }

  return s;
}

T_BinaryData TCollis_CardReader::ReadNextDataAfterMarker(const char* pMarker)
{
  std::string s;
  while(pScript)
  {
    s = ReadNextLineFromScript();
    size_t idx = s.find(pMarker);
    if(idx != std::string::npos)
      return GetBcdDataFromString(s.substr(idx+strlen(pMarker)));
  }
  return T_BinaryData();
}

T_BinaryData TCollis_CardReader::ReadATR()
{
  return T_BinaryData();
}

T_BinaryData TCollis_CardReader::ReadNextRequest()
{
  return ReadNextDataAfterMarker(cReqMarker);
}

T_BinaryData TCollis_CardReader::ReadNextResponse()
{
  return ReadNextDataAfterMarker(cRespMarker);
}



TEval_CardReader::TEval_CardReader(const std::string& cScriptName) 
  : TSimulation_CardReader(cScriptName)
{
}

std::string TEval_CardReader::ReadNextLineFromScript()
{
  char c;
  std::string s;

  if(pScript)
  {
    for(;;)
    {
      if(pScript->Read((BYTE*)&c, 1) != 1)
      {
        CloseScriptFile();
        break;
      }
      if(c == 0x0A)
        break;
      else
      if(c != 0x0D)
        s += c;
    }
  }

  return s;
}

T_BinaryData TEval_CardReader::GetDataFromString(const std::string& s)
{
  int i=0;
  int flag=0;
  int len = s.length();
  std::string data;

  for(i=0;i<len;++i)
  {
    if(s[i] == '\t')
    {
      if(flag == 1)
        break;
      flag = 1;
    }
    else
    if(IS_HEX_(s[i]))
      data += s[i];
  }
  return GetBcdDataFromString(data);
}

T_BinaryData TEval_CardReader::ReadNextDataAfterMarker(const char* pMarker)
{
  std::string s;
  while(pScript)
  {
    s = ReadNextLineFromScript();
    size_t idx = s.find(pMarker);
    if(idx != std::string::npos)
      return GetDataFromString(s.substr(idx+strlen(pMarker)));
  }
  return T_BinaryData();
}

T_BinaryData TEval_CardReader::ReadATR()
{
  return ReadNextDataAfterMarker("Physical");
}

T_BinaryData TEval_CardReader::ReadHexData(const char* pMarker)
{
  T_BinaryData d(ReadNextDataAfterMarker(pMarker));
  std::string s;

  if(d.Length())
  {
    for(;;)
    {
      s = ReadNextLineFromScript();
      size_t idx = s.find(pMarker);
      if(idx != std::string::npos)
        d += GetDataFromString(s.substr(idx+strlen(pMarker)));
      else
        break;
    }
  }
  return d;
}

T_BinaryData TEval_CardReader::ReadNextRequest()
{
  return ReadHexData("C-APDU");
}

T_BinaryData TEval_CardReader::ReadNextResponse()
{
  return ReadHexData("R-APDU");
}
