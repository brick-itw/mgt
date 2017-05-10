#ifndef T_CARD_READER_PAX_H
#define T_CARD_READER_PAX_H

#include "Platforms/Interfaces/ICardReader.h"

//realization for PAX s920
class TCardReader : public ICardReader
{
	BOOLEAN ProtocolT1Flag;

public:
	TCardReader();

	virtual Error Open();
	virtual void Close();

	virtual Error Enable(BOOLEAN bEnableMagn, BOOLEAN bEnableICC,	BOOLEAN bEnableCTLS);
	virtual void Disable();

	virtual BOOLEAN IsMagstripe()	{return true;}
	virtual BOOLEAN IsICC()		{return true;}
	virtual BOOLEAN IsCTLS()		{return false;}
	virtual BOOLEAN IsMotorized()	{return false;}

	virtual BOOLEAN CanCaptureCards(){return false;}

	virtual BOOLEAN HasLED(LEDTYPE ltp){return false;}
	virtual void ControlLED(LEDTYPE ltp, LEDCOLOR color){}

	virtual BOOLEAN MagnCardSwiped();
	virtual BOOLEAN CardInserted();
	virtual BOOLEAN CtlsCardTapped()	{return false;}

	virtual BOOLEAN CardInside(){
		return CardInserted();
	}

	virtual Error ReadMagnCard(TMagnCardInfo& Card);

	virtual Error ResetICC(TAtrData& ATR);
//	{
//		if(!IsCardInserted())
//			return ERROR_CARD_NOT_PRESENT;
//
//		PowerOffCard();
//
//		ProtocolT1Flag = FALSE;
//		OsIccOpen(0);
//		BYTE atr[33];
//		if(OsIccInit(0, 0, atr) != 0)
//			return ERROR_CARD_MUTE;
//
//		ATR.Length = atr[0];
//		memmove(ATR.AtrData, atr+1, ATR.Length);
//		if((atr[0] > 4) && (atr[2] & 0x80) && (atr[5] & 0x01))
//			ProtocolT1Flag = TRUE; // protocol T=1
//		return SUCCESS;
//	}
	virtual Error ActivateCTLSCard()	{return ERROR_NO_SUPPORT;}

	virtual Error ExchangeAPDU(TApdu& pApdu);
//	{
//		  ST_APDU_REQ APDU;
//		  ST_APDU_RSP RESP;
//		  int err;
//
//		  APDU.Cmd[0]   = pApdu->CLA;
//		  APDU.Cmd[1]   = pApdu->INS;
//		  APDU.Cmd[2]   = pApdu->P1;
//		  APDU.Cmd[3]   = pApdu->P2;
//		  APDU.LC       = pApdu->Lc;
//		  APDU.LE       = pApdu->Le;
//		  if((pApdu->Le == 0xFF) || ((pApdu->Le == 0) && !ProtocolT1Flag[cid]))
//		    APDU.LE = 256;
//		  memmove(APDU.DataIn, pApdu->DataIn, pApdu->Lc);
//
//			  err = OsIccExchange(0, 0, &APDU, &RESP);
//			  if(err == 0)
//			  {
//				  pApdu->sw1sw2 = ((WORD)RESP.SWA) << 8) | (WORD)(RESP.SWB);
//
//				  if ((RESP.SWA == 0x61) || (RESP.SWA == 0x6C))
//				  {
//					  pApdu->Le = RESP.SWB;
//					  if (RESP.SWA == 0x6C)
//					  {
//						  APDU.Cmd[0]   = pApdu->CLA;
//						  APDU.Cmd[1]   = pApdu->INS;
//						  APDU.Cmd[2]   = pApdu->P1;
//						  APDU.Cmd[3]   = pApdu->P2;
//					  }
//					  else
//					  {
//						  APDU.Cmd[0]   = 0;
//						  APDU.Cmd[1]   = 0xC0;
//						  APDU.Cmd[2]   = 0;
//						  APDU.Cmd[3]   = 0;
//					  }
//					  APDU.LC = 0;
//					  APDU.LE = pApdu->Le;
//					  memset(APDU.DataIn, 0, sizeof(APDU.DataIn));
//					  err = OsIccExchange(0, 0, &APDU, &RESP);
//				  }
//			  }
//			  if(err == 0)
//			  {
//				  pApdu->sw1sw2 = ((WORD)RESP.SWA) << 8) | (WORD)(RESP.SWB);
//				  pApdu->OutLength = RESP.LenOut;
//			      memmove(pApdu->DataOut, RESP.DataOut, pApdu->OutLength);
//			  }
//		  }
//		  return (err == 0) ? SUCCESS : ERROR_CARD_REMOVED;
//	}

	virtual void PowerOffCard();
//	{
//		OsIccClose(0);
//	}

	virtual void EjectCard(){}
	virtual void CaptureCard(){}

private:
	TMagnCardInfo card;
	int HandleMag;
	BYTE ICCOpened[ICC_CNT];       // ICC devices opened flags
	virtual WORD GetSlotID(BYTE CID);
	BYTE InitCardSlot(BYTE CID);
};

#endif
