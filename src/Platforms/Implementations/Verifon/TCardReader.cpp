#include "Platforms/Implementations/Verifon/TCardReader.h"

#include <string.h>

TCardReader::TCardReader(){
	ProtocolT1Flag = FALSE;
}
DWORD TCardReader::GetSlotID(BYTE CID)
{
  #ifdef EXT_CARD_SUPPORT
  switch (CID)
  {
    case 0:  return EXT_CUSTOMER_CARD;
    case 1:  return EXT_MERCHANT_SLOT_1;
    case 2:  return EXT_MERCHANT_SLOT_2;
    case 3:  return EXT_MERCHANT_SLOT_3;
    case 4:  return EXT_MERCHANT_SLOT_4;
    default: return EXT_MERCHANT_SLOT_1;
  }
#else

  switch (CID)
  {
    case 0:  return CUSTOMER_CARD;

    case 1:  if (drvModelNo == OMNI_3350)       return MERCHANT_SLOT_4;
             else                               return MERCHANT_SLOT_1;

    case 2:  if (drvModelNo == OMNI_3350)       return MERCHANT_SLOT_1;
             else                               return MERCHANT_SLOT_2;

    case 3:  if (drvModelNo == OMNI_3350)       return MERCHANT_SLOT_3;
             else                               return MERCHANT_SLOT_3;

    case 4:  if (drvModelNo == OMNI_3350)       return MERCHANT_SLOT_2;
             else                               return MERCHANT_SLOT_4;

    case 5:  return MERCHANT_FULL_SIZE_CARD;

    default: return (((DWORD)1)<<(get_icc_device(CID+1)-1));
  }

#endif
}
BYTE TCardReader::InitCardSlot(BYTE CID)
{
  unsigned long ulSlot;

  if (CID >= ICC_CNT)
    return ERROR_CRD_OPEN_FAILURE;

  if (ICCOpened[CID])
    return SUCCESS;

  ulSlot = drvGetSlotID(CID);

  if ( Init_CardSlot(ulSlot) != CARDSLOT_SUCCESS )
    return ERROR_CRD_OPEN_FAILURE;

  ICCOpened[CID] = 1;

  return SUCCESS;
}

Error TCardReader::Enable(BOOLEAN bEnableMagn, BOOLEAN bEnableICC,	BOOLEAN bEnableCTLS)
{
	int res;
	DWORD ulSlot;
	if(bEnableMagn){
	  HandleMag = open(DEV_CARD, 0);
	  if(!HandleMag)
	    return ERROR_CRD_OPEN_FAILURE;
	}
	if(bEnableICC){
	  ulSlot = GetSlotID(0);
  	  if(InitCardSlot(ulSlot))
	    return ERROR_CRD_OPEN_FAILURE;
    }
	
    return SUCCESS;
}

BOOLEAN TCardReader::MagnCardSwiped(){
  return TRUE;
}

void TCardReader::Disable()
{
   Close();
}


Error TCardReader::Open()
{
	return SUCCESS;
}

void TCardReader::Close()
{
  if(HandleMag)
    close(HandleMag);
  HandleMag = 0;
}

Error TCardReader::ResetICC(TAtrData& ATR){

  unsigned long ulSlot;
  unsigned char ucProtocol;
  DWORD Value;
  WORD ATRLen;
  BYTE  CID,atr[33];
  
  CID = 0;

  if(!CardInserted())
    return ERROR_CARD_NOT_PRESENT;

  PowerOffCard();
  ProtocolT1Flag = FALSE;
  
  if (!ICCOpened[CID])
    if (drvInitCardSlot(CID) != ERR_OK)
      return ERROR_CRD_OPEN_FAILURE;

  ulSlot = GetSlotID(CID);

  Value = CLASS_A;
  if ( Set_Capability(ulSlot,Tag_Class_Management, (BYTE*)(&Value)) != CARDSLOT_SUCCESS )
    return ERROR_CRD_OPEN_FAILURE;

  if ( Reset_CardSlot(ulSlot, RESET) != CARDSLOT_SUCCESS )
    return ERROR_CRD_OPEN_FAILURE;

  ICCOpened[CID] = 2; // карта вставлена в ридер, значит на нее подали питание -
                      // признак того, что можно будет делать Terminate_CardSlot
                      // если же вызвать Terminate_CardSlot, когда питание не подавалось
                      // то будет "синька" - честь и хвала VeriFone!!! :-)

  ucProtocol = Get_Protocol(ulSlot);

  if ((ucProtocol != PROTOCOL_T0) && (ucProtocol != PROTOCOL_T1))
    return ERROR_CRD_OPEN_FAILURE;

  if ( (Get_Capability(ulSlot, Tag_ATR_Length, (unsigned char*)&ATRLen) == CARDSLOT_SUCCESS) &&
       (ATRLen) &&
       (Get_Capability(ulSlot, Tag_ATR_String, (unsigned char*)atr) == CARDSLOT_SUCCESS)
     ) {
    ATR.AtrLength = atr[1] & 0x0F;
    memmove(ATR.AtrData, (char*)(atr+(ATRLen-ATR.AtrLength)), ATR.AtrLength);
  }

	
  ProtocolT1Flag = TRUE;
  return SUCCESS;
}

Error TCardReader::ExchangeAPDU(TApdu& pApdu)
{
	BYTE APDU[1024];
	unsigned short outlen,inlen;
    unsigned long ulSlot;
	int  err;
    BYTE CID;
	
	APDU[0]   = pApdu.CLA;
	APDU[1]   = pApdu.INS;
	APDU[2]   = pApdu.P1;
	APDU[3]   = pApdu.P2;
	inlen = 4;

    if(pApdu->Lc){
      APDU[inlen++] = pApdu->Lc;
      memmove(APDU+inlen,pApdu->pDataIn,pApdu->Lc);
      inlen += pApdu->Lc;
    }
    if(pApdu->Le){
      if(pApdu->Le == 0xFF)
        pApdu->Le = 0;
      buf[inlen++] = pApdu->Le;
    }

	err = Transmit_APDU(ulSlot, buf, inlen, pApdu->pDataOut, &outlen);
	if(err == CARDSLOT_SUCCESS)
	{
		pApdu.sw1sw2 = (((WORD)pApdu->pDataOut[outlen-2] << 8) | (WORD)(pApdu->pDataOut[outlen-1]);

		if((pApdu->SW1SW2[0] == 0x61) || (pApdu->SW1SW2[0] == 0x6C)){
		{
			pApdu->Le = pApdu->SW1SW2[1];
			if (pApdu->SW1SW2[0] == 0x6C)
			{
				APDU[0] = pApdu->CLA;
                APDU[1] = pApdu->INS;
                APDU[2] = pApdu->P1;
                APDU[3] = pApdu->P2;
                APDU[4] = pApdu->Le;
                inlen = 5;
			}
			else
			{
				buf[0] = 0;
                buf[1] = 0xC0;
                buf[2] = 0;
                buf[3] = 0;
                buf[4] = pApdu->Le;
				inlen = 5;
			}
			err = Transmit_APDU(ulSlot, buf, inlen, pApdu->pDataOut, &outlen);
		}
	}
	if(err == CARDSLOT_SUCCESS)
	{
	  pApdu.sw1sw2 = (((WORD)pApdu->pDataOut[outlen-2] << 8) | (WORD)(pApdu->pDataOut[outlen-1]);
	  pApdu.OutLength = outlen;
	}
	return (err == CARDSLOT_SUCCESS) ? SUCCESS : ERROR_CARD_REMOVED;
}


BOOLEAN TCardReader::CardInserted(){
  unsigned long ulSlot;
  ulSlot = GetSlotID(0);
  
  if (Get_Card_State(ulSlot) == CARD_PRESENT)
      return TRUE;
  return FALSE;
}


Error TCardReader::ReadMagnCard(TMagnCardInfo& Card)
{
	ST_MSR_DATA Tr1_Data, Tr2_Data, Tr3_Data;
	Error result = ERROR_CARD_NOT_PRESENT;//err
	int magRes = 0;

	int res = OsMsrSwiped();
	do{
		if (res == TRUE) {
			magRes = OsMsrRead(&Tr1_Data, &Tr2_Data, &Tr3_Data);
			if (magRes == RET_OK) {
				memset(card.Track1, 0, MAX_TRACK1);
				memset(card.Track2, 0, MAX_TRACK2);

				if ((Tr1_Data.Status) == RET_OK && Tr1_Data.DataLen <= MAX_TRACK1) {
					memcpy(card.Track1, Tr1_Data.TrackData, Tr1_Data.DataLen);
				} else {
					result = 1;//!!!
					break;
				}

				if ((Tr2_Data.Status) == RET_OK && (Tr2_Data.DataLen+1) <= MAX_TRACK2) {
					//int sz2 = Tr2_Data.DataLen + 2;
					card.Track2[0] = 'B';
					memcpy(card.Track2 + 1, Tr2_Data.TrackData, Tr2_Data.DataLen);
					for (int k = 0; k < Tr2_Data.DataLen; k++) {
						if (card.Track2[k] == '=')
							card.Track2[k] = 'D';
						if (card.Track2[k] == ' ') {
							card.Track2[k] = 'F';
							break;
						}
					}
					result = SUCCESS;
				} else {
					result = 2;
				}

				Card = card;
			} else {
				//	printf("magRes = %d\n", magRes);
			}
		}
	}
	while(0);
	return result;
}

void TCardReader::PowerOffCard()
{
  unsigned long ulSlot;
  BYTE CID;
  CID = 0;
  if (!ICCOpened[CID])
    return;

  ulSlot = GetSlotID(CID);

  if (ICCOpened[CID] == 2)
    Terminate_CardSlot(ulSlot, SWITCH_OFF_CARD);

  Close_CardSlot(ulSlot);

  ICCOpened[CID] = 0;
}
