#include "Platforms/Implementations/PaxProlin/TPaxProlinCardReader.h"

#include <osal.h>
#include <string.h>

TPaxProlinCardReader::TPaxProlinCardReader(){
	ProtocolT1Flag = FALSE;
}

Error TPaxProlinCardReader::Enable(BOOLEAN bEnableMagn, BOOLEAN bEnableICC,	BOOLEAN bEnableCTLS)
{
	int res;
	if(bEnableMagn){
	  OsMsrOpen();
	  OsMsrReset();
	}
	if(bEnableICC)
  	  OsIccOpen(0);
    return SUCCESS;
}

BOOLEAN TPaxProlinCardReader::MagnCardSwiped(){
  return OsMsrSwiped();
}

void TPaxProlinCardReader::Disable()
{
   Close();
}


Error TPaxProlinCardReader::Open()
{
	return SUCCESS;
}

void TPaxProlinCardReader::Close()
{
	OsMsrClose();
	OsPiccClose();//power off ctls
}

Error TPaxProlinCardReader::ResetICC(TAtrData& ATR){
	if(!CardInserted())
		return ERROR_CARD_NOT_PRESENT;

	PowerOffCard();

	ProtocolT1Flag = FALSE;
	OsIccOpen(0);
	BYTE atr[33];
	if(OsIccInit(0, 0, atr) != 0)
		return ERROR_CARD_MUTE;

	ATR.AtrLength = atr[0];
	memmove(ATR.AtrData, atr+1, ATR.AtrLength);
	if((atr[0] > 4) && (atr[2] & 0x80) && (atr[5] & 0x01))
		ProtocolT1Flag = TRUE; // protocol T=1
	return SUCCESS;
}

Error TPaxProlinCardReader::ExchangeAPDU(TApdu& pApdu)
{
	ST_APDU_REQ APDU;
	ST_APDU_RSP RESP;
	int err;

	APDU.Cmd[0]   = pApdu.CLA;
	APDU.Cmd[1]   = pApdu.INS;
	APDU.Cmd[2]   = pApdu.P1;
	APDU.Cmd[3]   = pApdu.P2;
	APDU.LC       = pApdu.Lc;
	APDU.LE       = pApdu.Le;
	if((pApdu.Le == 0xFF) || ((pApdu.Le == 0) && !ProtocolT1Flag))
		APDU.LE = 256;
	memmove(APDU.DataIn, pApdu.DataIn, pApdu.Lc);

	err = OsIccExchange(0x00, 0, &APDU, &RESP);
	if(err == 0)
	{
		 pApdu.sw1sw2 = (((WORD)RESP.SWA) << 8) | (WORD)(RESP.SWB);

		if ((RESP.SWA == 0x61) || (RESP.SWA == 0x6C))
		{
			pApdu.Le = RESP.SWB;
			if (RESP.SWA == 0x6C)
			{
				APDU.Cmd[0]   = pApdu.CLA;
				APDU.Cmd[1]   = pApdu.INS;
				APDU.Cmd[2]   = pApdu.P1;
				APDU.Cmd[3]   = pApdu.P2;
			}
			else
			{
				APDU.Cmd[0]   = 0;
				APDU.Cmd[1]   = 0xC0;
				APDU.Cmd[2]   = 0;
				APDU.Cmd[3]   = 0;
			}
			APDU.LC = 0;
			APDU.LE = pApdu.Le;
			memset(APDU.DataIn, 0, sizeof(APDU.DataIn));
			err = OsIccExchange(0, 0, &APDU, &RESP);
		}
	}
	if(err == 0)
	{
	  pApdu.sw1sw2 = (((WORD)RESP.SWA) << 8) | (WORD)(RESP.SWB);
		pApdu.OutLength = RESP.LenOut;
		memmove(pApdu.DataOut, RESP.DataOut, pApdu.OutLength);
	}
	return (err == 0) ? SUCCESS : ERROR_CARD_REMOVED;
}


BOOLEAN TPaxProlinCardReader::CardInserted(){
	//Result = FALSE;
	if(OsIccDetect(0) == 0){
		//bCtlsEmulation = FALSE;
		//Result = ERR_OK;
		return TRUE;
	}
	return FALSE;
}


Error TPaxProlinCardReader::ReadMagnCard(TMagnCardInfo& Card)
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

void TPaxProlinCardReader::PowerOffCard()
{
	OsIccClose(0);
}
