#ifndef _U2_SIMULATION_CARDREADER_H
#define _U2_SIMULATION_CARDREADER_H

#include "Platforms/Interfaces/ICardReader.h"
#include "Platforms/Interfaces/IFileSystem.h"
#include "Common/Utils/T_BinaryData.h"

#include <string>

#define SIMULATION_READER_ERROR(code) REGISTER_ERROR(MODULE_SIMULATION_READER, (code))
enum SimulationReaderErrorCode
{
  ERROR_SCRIPT_FAILED_TO_LOAD = SIMULATION_READER_ERROR(1),
  ERROR_REQUEST_DATA_MISMATCH = SIMULATION_READER_ERROR(2),
};

class TSimulation_CardReader :  public ICardReader{
protected:

  enum SIMULATION_STATE{
    SS_BEFORE_CARD_INSERTED,
    SS_CARD_INSIDE,
    SS_CARD_REMOVING,
    SS_CARD_REMOVED
  } State;

  IFileReadable* pScript;
  std::string ScriptFileName;
  BOOLEAN bEnableMagStripe;
  BOOLEAN bEnableICC;
  BOOLEAN bEnableCTLS;

  bool IsScriptFilePresent();
  bool LoadScript();

  virtual T_BinaryData ReadATR() = 0;
  virtual T_BinaryData ReadNextRequest() = 0;
  virtual T_BinaryData ReadNextResponse() = 0;
  
  bool OpenScriptFile();
  void CloseScriptFile();
  const char* GetScriptFileName();

public:
  TSimulation_CardReader(const std::string& cScriptName);
  ~TSimulation_CardReader();
  
  virtual Error Enable (BOOLEAN enableMagn, BOOLEAN enableICC, BOOLEAN enableCTLS);
  virtual void  Disable();

  virtual Error Open()
  {
    return Enable(TRUE, TRUE, TRUE);
  };

  virtual void Close()
  {
    Disable();
  };

  virtual BOOLEAN IsMagstripe()  {return TRUE;};
  virtual BOOLEAN IsICC()        {return TRUE;};
  virtual BOOLEAN IsCTLS()       {return TRUE;};
  virtual BOOLEAN IsMotorized()  {return FALSE;};

  virtual BOOLEAN CanCaptureCards() {return FALSE;};

  virtual BOOLEAN HasLED    (LEDTYPE ltp) {return FALSE;};
  virtual void ControlLED(LEDTYPE ltp, LEDCOLOR color) {};

  virtual BOOLEAN MagnCardSwiped(); 

  virtual BOOLEAN CtlsCardTapped(){return FALSE;};  
  virtual BOOLEAN CardInside() {return CardInserted();};
  virtual Error ReadMagnCard (TMagnCardInfo& Card);
  virtual Error ActivateCTLSCard (){return ERROR_CRD_CTLS_NOT_SUPPORTED;};

  virtual void EjectCard(){};
  virtual void CaptureCard(){};

  virtual BOOLEAN  CardInserted(); 
  virtual Error ResetICC(TAtrData& ATR);
  virtual Error ExchangeAPDU(TApdu& Apdu);
  virtual void  PowerOffCard();
};

class TCollis_CardReader :  public TSimulation_CardReader{
protected:

  std::string  ReadNextLineFromScript();
  T_BinaryData ReadNextDataAfterMarker(const char* pMarker);

  virtual T_BinaryData ReadATR();
  virtual T_BinaryData ReadNextRequest();
  virtual T_BinaryData ReadNextResponse();

public:

  TCollis_CardReader(const std::string& cScriptName);
};

class TEval_CardReader :  public TSimulation_CardReader{
protected:

  T_BinaryData GetDataFromString(const std::string& s);
  T_BinaryData ReadHexData(const char* pMarker);
  T_BinaryData ReadNextDataAfterMarker(const char* pMarker);
  std::string  ReadNextLineFromScript();

  virtual T_BinaryData ReadATR();
  virtual T_BinaryData ReadNextRequest();
  virtual T_BinaryData ReadNextResponse();

public:

  TEval_CardReader(const std::string& cScriptName);
};

#endif

