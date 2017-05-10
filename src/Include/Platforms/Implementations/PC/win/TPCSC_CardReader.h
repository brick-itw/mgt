#ifndef _U2_WINDOWS_CARDREADER_H
#define _U2_WINDOWS_CARDREADER_H

#include "Platforms/Interfaces/ICardReader.h"
#include "Platforms/Interfaces/IFileSystem.h"
#include <windows.h>
#include <winscard.h>
#include <string>
#include <list>

class TPCSC_CardReader :  public ICardReader{
protected:
  SCARDCONTEXT hSC;
  SCARDHANDLE  hCard;
  DWORD        dwProtocol;
  TAtrData     ATR;
  IFileSystem* pTraces;
  std::string  Name;
  std::list<std::string> ReaderNames;
  BOOLEAN      IsContactless;

  std::string  TraceFileName() const;
  void LogAtr();
  void LogPowerOff();
  void LogApdu(TApdu& apdu);
  void GetReaderNames();
  BOOLEAN OpenCard();

public:
  TPCSC_CardReader(IFileSystem* ptr);
  ~TPCSC_CardReader();
  
  virtual Error Enable (BOOLEAN bEnableMagn, BOOLEAN bEnableICC, BOOLEAN bEnableCTLS);
  virtual void  Disable();

  virtual Error Open()
  {
    return Enable(FALSE, TRUE, FALSE);
  };

  virtual void Close()
  {
    Disable();
  };

  virtual BOOLEAN IsMagstripe()  {return FALSE;};
  virtual BOOLEAN IsICC()        {return TRUE; };
  virtual BOOLEAN IsCTLS()       {return FALSE;};
  virtual BOOLEAN IsMotorized()  {return FALSE;};

  virtual BOOLEAN CanCaptureCards() {return FALSE;};

  virtual BOOLEAN HasLED    (LEDTYPE ltp) {return FALSE;};
  virtual void ControlLED(LEDTYPE ltp, LEDCOLOR color) {};

  virtual BOOLEAN MagnCardSwiped(); 

  virtual BOOLEAN CtlsCardTapped();  
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

#endif

