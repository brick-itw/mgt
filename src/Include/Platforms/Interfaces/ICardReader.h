/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Card reader device interface declaration
 */

#ifndef _U2_CARDREADER_H
#define _U2_CARDREADER_H

#include "Base/Types/types.h"
#include "Base/Types/errors.h"
#include "Platforms/Interfaces/IDevice.h"

#define CARD_READER_ERROR(code) REGISTER_ERROR(MODULE_CARD_READER, (code))
enum CardReaderErrorCode
{
  ERROR_CRD_OPEN_FAILURE       = CARD_READER_ERROR(1),
  ERROR_CARD_NOT_PRESENT       = CARD_READER_ERROR(2),
  ERROR_CRD_CTLS_NOT_SUPPORTED = CARD_READER_ERROR(3),
  ERROR_CARD_REMOVED           = CARD_READER_ERROR(4),
  ERROR_CARD_MUTE              = CARD_READER_ERROR(5),
};

#define MAX_TRACK1  120
#define MAX_TRACK2   40
#define MAX_TRACK3  180

struct TMagnCardInfo{
  char Track1[MAX_TRACK1];
  char Track2[MAX_TRACK2];
  char Track3[MAX_TRACK3];
};

#define MAX_ATR_LEN 33
struct TAtrData{
  BYTE  AtrData[MAX_ATR_LEN];
  DWORD AtrLength;
};

struct TApdu{
  BYTE CLA, INS, P1, P2, Lc, Le;
  const BYTE* DataIn;
  BYTE  DataOut[258];
  WORD  sw1sw2;
  WORD  OutLength;

  TApdu(BYTE cla, BYTE ins, BYTE p1, BYTE p2, 
        BYTE lc,  const BYTE* dataIn, 
        BYTE le)
  {
    sw1sw2 = 0;
    CLA = cla;
    INS = ins;
    P1  = p1;
    P2  = p2;
    Lc  = lc;
    Le  = le;
    DataIn  = dataIn;
    OutLength = 0;
  };

  WORD BuildCmdBuf(BYTE* p) const
  {
    WORD  cnt = 4;

    p[0] = CLA;
    p[1] = INS;
    p[2] = P1;
    p[3] = P2;
    if(Lc){
      p[cnt++] = Lc;
      memmove(p+cnt, DataIn, Lc);
      cnt += Lc;
    }
    if(Le)
      p[cnt++] = (Le == 0xFF) ? 0 : Le;
    
    return cnt;
  };

  void ParseReplyBuf(const BYTE* p, WORD len)
  {
    Le = 0;
    if(len >= 2){
      sw1sw2 = ((WORD)(p[len-2]) << 8) + (WORD)(p[len-1]);
      OutLength = (BYTE)(len-2);
      memmove(DataOut, p, OutLength);
    }
  };

  WORD  GetSWbytes()           const { return sw1sw2; };
  BYTE  GetSW1()               const { return (BYTE)(sw1sw2 >> 8); };
  BYTE  GetSW2()               const { return (BYTE)(sw1sw2); };
  WORD  GetReplyLength()       const { return OutLength;};
  const BYTE* GetReplyData()   const { return DataOut;    };
};


typedef enum{
  LED_OFF    = 0,
  LED_GREEN  = 1,
  LED_RED    = 2,
  LED_YELLOW = 3,
  LED_BLUE   = 4
}LEDCOLOR;

typedef enum{
  LED_MAIN   = 0,
  LED_CTLS_1 = 1, 
  LED_CTLS_2 = 2, 
  LED_CTLS_3 = 3, 
  LED_CTLS_4 = 4
}LEDTYPE;


class ICardReader : public IDevice {
public:

  virtual Error Enable (BOOLEAN bEnableMagn, BOOLEAN bEnableICC, BOOLEAN bEnableCTLS) = 0;  // returns 0 if OK, error code otherwise
  virtual void  Disable()  = 0;

  virtual BOOLEAN IsMagstripe() = 0;
  virtual BOOLEAN IsICC()       = 0;
  virtual BOOLEAN IsCTLS()      = 0;
  virtual BOOLEAN IsMotorized() = 0;

/*
  For non-motorized readers, this function must return FALSE.
  For motorized readers, this function must return TRUE if the reader is
  capable of capturing the card.
*/
  virtual BOOLEAN CanCaptureCards() = 0;

  virtual BOOLEAN HasLED    (LEDTYPE ltp) = 0;
  virtual void ControlLED(LEDTYPE ltp, LEDCOLOR color) = 0;

/*
  Must return TRUE if track2 data is ready for reading
*/
  virtual BOOLEAN MagnCardSwiped()  = 0; // returns TRUE if card was swiped,  FALSE otherwise

/*
  Must return TRUE only when the card is in its final position under ICC contacts.
*/
  virtual BOOLEAN CardInserted()    = 0; 

/*
  Must return TRUE when a CTLS card is detected in the radio field.
*/
  virtual BOOLEAN CtlsCardTapped()  = 0; 


/*
  For non-motorized readers, this function must return CardInserted().
  For motorized readers, this function must return TRUE if the card is somewhere
  inside the reader, maybe at the gate waiting to be picked up by the customer,
  or in the final position (in this case CardInserted() also returns TRUE),
  or somewhere between.
*/
  virtual BOOLEAN CardInside()      = 0;


/*
  Returns ERR_OK if track2 was successfully read. Other tracks are optional.
*/
  virtual Error ReadMagnCard     (TMagnCardInfo& Card) = 0;


/*
  ATR data must be stored in a local buffer inside TCardReader descendant.
  ATR.Set() must be called to set pointer to this buffer.
  Returns ERR_OK if the card has successfully returned an ATR, error code otherwise.
*/
  virtual Error ResetICC         (TAtrData& ATR)       = 0;


/*
  Returns ERR_OK if the card has been successfully activated, error code otherwise
*/
  virtual Error ActivateCTLSCard ()                    = 0;


/*
  This function is used for both ICC and CTLS cards.
  Returns ERR_OK if APDU was successfully transmitted and response received, error code otherwise.
*/
  virtual Error ExchangeAPDU(TApdu& Apdu) = 0;


/*
  This function is used for both ICC and CTLS cards.
  For ICC, it switches the power off.
  For CTLS, it turns the radio field off.
*/
  virtual void PowerOffCard() = 0;


/*
  For non-motorized readers, these functions must do nothing.
  For motorized readers, they should initiate the card movement 
  outside (EjectCard) or inside (CaptureCard).
  The success or failure of such movement will be checked by repeated calls to
  CardInside() function. When CardInside() returns FALSE, it is assumed that
  the card is either retrieved by the customer or successfully captured.
*/
  virtual void EjectCard() = 0;
  virtual void CaptureCard() = 0;
};

#endif
