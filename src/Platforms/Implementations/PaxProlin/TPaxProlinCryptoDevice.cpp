#include "Platforms/Implementations/PaxProlin/TPaxProlinCryptoDevice.h"
#include "Common/Utils/FormatConverters.h"
#include "Common/Application/TTerminalEnvironment.h"

#include "UI/IDesktop.h"
#include "Common/Emv/TPinPromptHandler.h"

#include <osal.h>
#include <string.h>

#include <string>

#pragma pack(1)
typedef struct{
  BYTE  Format;
  BYTE  MasterKeyType;
  BYTE  MasterKeyCell;
  BYTE  TargetKeyCell;
  BYTE  RandomData[7];
  BYTE  TargetKeyType;
  BYTE  KeyLength;
  BYTE  KeyValue[16];
  BYTE  KcvMode;
  BYTE  KcvData[128];
  BYTE  Padding [18];
}TPaxKeyDescriptor;
#pragma pack()


#define PAX_KLK_KEY_ID      1
#define PAX_MM_KEY_ID       3
#define PAX_PM_KEY_ID       5
#define PAX_MS_KEY_ID       7
#define PAX_PS_KEY_ID       9
#define PAX_DK_KEY_ID      11


using namespace std;

TPaxProlinCryptoDevice::TPaxProlinCryptoDevice(){
	int res = 0;
	res = OsPedOpen();
	res = OsPedSetInterval(5000);
}

TPaxProlinCryptoDevice::~TPaxProlinCryptoDevice(){
	OsPedClose();
}

Error TPaxProlinCryptoDevice::PaxInternalLoadKey(BYTE cell, BYTE key_type, BYTE master_cell, BYTE master_key_type, const char* pValue32)
{
  TPaxKeyDescriptor descr = {0};

  descr.Format = 0x03;
  descr.MasterKeyType = master_key_type;
  descr.MasterKeyCell = master_cell;
  descr.TargetKeyCell = cell;
  OsGetRandom(descr.RandomData, sizeof(descr.RandomData));
  descr.TargetKeyType = key_type;
  descr.KeyLength     = sizeof(descr.KeyValue);

  HexToBytes(pValue32, descr.KeyValue, sizeof(descr.KeyValue)*2);
  descr.KcvMode = 0;
  OsGetRandom(descr.Padding, sizeof(descr.Padding));

  int rc = OsPedWriteKey((unsigned char*)&descr);
  if(rc)
    return Error(ERROR_WRITE_KEY).AddSubError(rc);

  return SUCCESS;
}

Error TPaxProlinCryptoDevice::PaxLoadKey(BYTE UposKeyIdx, BOOLEAN IsEncrypted, const char* pValue32)
{
  return PaxInternalLoadKey(
                    PaxGetKeyCellForKeyType(UposKeyIdx), 
                    PaxGetKeyTypeForKeyType(UposKeyIdx), 
                    IsEncrypted ? PaxGetMasterKeyCellForKeyType(UposKeyIdx) : 0,
                    IsEncrypted ? PaxGetMasterKeyTypeForKeyType(UposKeyIdx) : 0,
                    pValue32);

}

BYTE TPaxProlinCryptoDevice::PaxGetKeyCellForKeyType(BYTE keyIdx)
{
  switch(keyIdx)
  {
    case KEYIDX_MPIN: return PAX_PM_KEY_ID;
    case KEYIDX_MMAC: return PAX_MM_KEY_ID;
    case KEYIDX_KLK:  return PAX_KLK_KEY_ID;
    case KEYIDX_SPIN: return PAX_PS_KEY_ID;
    case KEYIDX_SMAC: return PAX_MS_KEY_ID;
    case KEYIDX_DK:   return PAX_DK_KEY_ID;
    default:          return 0xFF;
  }
}

BYTE TPaxProlinCryptoDevice::PaxGetKeyTypeForKeyType(BYTE keyIdx)
{
  switch(keyIdx)
  {
    case KEYIDX_MPIN: 
    case KEYIDX_MMAC: return PED_TMK;
    case KEYIDX_KLK:  return PED_TLK;
    case KEYIDX_SPIN: return PED_TPK;
    case KEYIDX_SMAC: return PED_TAK;
    case KEYIDX_DK:   return PED_TDK;
    default:          return 0xFF;
  }
}

BYTE TPaxProlinCryptoDevice::PaxGetMasterKeyCellForKeyType(BYTE keyIdx)
{
  switch(keyIdx)
  {
    case KEYIDX_MPIN: 
    case KEYIDX_MMAC: 
    case KEYIDX_KLK:  return PAX_KLK_KEY_ID;

    case KEYIDX_SPIN: return PAX_PM_KEY_ID;

    case KEYIDX_SMAC: return PAX_MM_KEY_ID;
    case KEYIDX_DK:   return PAX_MM_KEY_ID;

    default:          return 0xFF;
  }
}

BYTE TPaxProlinCryptoDevice::PaxGetMasterKeyTypeForKeyType(BYTE keyIdx)
{
  switch(keyIdx)
  {
    case KEYIDX_MPIN: 
    case KEYIDX_MMAC: 
    case KEYIDX_KLK:  return PED_TLK;

    case KEYIDX_SPIN: 
    case KEYIDX_SMAC: 
    case KEYIDX_DK:   return PED_TMK;

    default:          return 0xFF;
  }
}

Error TPaxProlinCryptoDevice::LoadMasterKey(BYTE UposkeyIdx, const char* pKey, 
                                            BOOLEAN IsEncrypted)
{
  if(UposkeyIdx > KEYIDX_KLK)
    return ERROR_WRONG_KEY_TYPE;

  return PaxLoadKey(UposkeyIdx, IsEncrypted, pKey);
}

Error TPaxProlinCryptoDevice::TestPINentry(WORD* pNumDigitsEntered, BYTE* pPinblock8)
{
	*pNumDigitsEntered = num_digits_entered;
	for(int i=0; i<PIN_BLOCK_LEN; ++i){
		pPinblock8[i] = pinblock[i];
	}
	return SUCCESS;
}

Error TPaxProlinCryptoDevice::StartPINentry(const TPinInfo& PinInfo)
{
  Error e = PaxLoadKey(KEYIDX_SPIN, TRUE, PinInfo.SessionKey);
  if(e != SUCCESS)
    return e;

  std::string out_str;
  TScreenOptions opt;
  TPinPromptMaker(PinInfo).FillScreenOptions(opt, &out_str);
  IDesktop::Instance()->ShowWindow(IDS_ENTER_PIN, &opt);

  WORD screen_w = 0, screen_h = 0;
  ISystem::Instance()->GetScreenSize(screen_w, screen_h);
  OsPedSetAsteriskLayout(screen_w/2, screen_h/2, 24, 0, PED_ASTERISK_ALIGN_CENTER);

  char panbuf[17];
  memset(panbuf,'0',16);
  memmove(panbuf + 4, PinInfo.Pan+strlen(PinInfo.Pan)-13, 12);
  panbuf[16]=0;

  memset(pinblock, 0, sizeof(pinblock));
  DWORD timeout = TTerminalEnvironment::Instance().GetPinEntryTimeout();

  int res = OsPedGetPinBlock(PaxGetKeyCellForKeyType(KEYIDX_SPIN),
                         (unsigned char*)panbuf, "4,5,6,7", 0x00, timeout, pinblock);

  num_digits_entered = 0;//don't know how to get this using pax api

	if (res == ERR_PED_PIN_INPUT_CANCEL)
	  return ERROR_PINPAD_USER_CANCEL;

	if(res == RET_OK)
 	  return SUCCESS;

	if(res == ERR_PED_INPUT_PIN_TIMEOUT)
	  return ERROR_USER_TIMEOUT;

	return ERROR_PINPAD_UNKNOWN_ERR;
}

Error TPaxProlinCryptoDevice::Test(WORD* pNumDigitsEntered, BYTE* pPinblock8)
{
  return ERROR_NO_SUPPORT;
}

void TPaxProlinCryptoDevice::Stop(){

}

Error TPaxProlinCryptoDevice::GetSerialNumber (char* value){
	int len = 0;
	len = OsRegGetValue("ro.fac.sn", value);
	if (len <= 0)
	{
		sprintf(value, "000123456789");
		len = 12;
		return ERROR_GET_SERIAL;
	}
	return SUCCESS;
}

BOOLEAN TPaxProlinCryptoDevice::GetKeyCheckValue(BYTE UposKeyIdx, BYTE* pKCV3)
{
  unsigned char KcvData[] = "\0\0\0\0\0\0\0\0";
  return OsPedGetKcv (PaxGetKeyTypeForKeyType(UposKeyIdx), 
                      PaxGetKeyCellForKeyType(UposKeyIdx), 
                      0x00, 
                      8, 
                      KcvData, pKCV3) ? FALSE : TRUE;
}


Error TPaxProlinCryptoDevice::DeleteKey(BYTE keyIdx)
{
  return  ERROR_NO_SUPPORT;
}

Error TPaxProlinCryptoDevice::CalculateMAC(const TMacInfo& MacInfo, BYTE* pMac8)
{
  Error e = PaxLoadKey(KEYIDX_SMAC, TRUE, MacInfo.SessionKey);
  if(e != SUCCESS)
    return e;

  T_BinaryData data(MacInfo.pData, MacInfo.Length, 8);
  int rc = OsPedGetMac(PaxGetKeyCellForKeyType(KEYIDX_SMAC), 
                    data.GetData(), 
                    data.Length(), 
                    pMac8, 
                    0x02);

  if(rc)
    return Error(ERROR_CALCULATE_MAC).AddSubError(rc);

  return SUCCESS;
}

Error TPaxProlinCryptoDevice::EncryptData(const TMacInfo& MacInfo, BYTE* pDest)
{
  Error e = PaxLoadKey(KEYIDX_DK, TRUE, MacInfo.SessionKey);
  if(e != SUCCESS)
    return e;

  T_BinaryData data(MacInfo.pData, MacInfo.Length, 8);
  int rc = OsPedDes(PaxGetKeyCellForKeyType(KEYIDX_DK), 
                    NULL,
                    data.GetData(), 
                    data.Length(), 
                    pDest, 
                    PED_ENCRYPT);
  if(rc)
    return Error(ERROR_ENCRYPT_DATA).AddSubError(rc);

  return SUCCESS;
}

