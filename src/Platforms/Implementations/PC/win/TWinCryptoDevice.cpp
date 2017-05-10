#include "Platforms/Implementations/Pc/win/TWinCryptoDevice.h"
#include "Platforms/Interfaces/ISystem.h"
#ifdef NEED_PIN
  #include "UI/IDesktop.h"
  #include "Common/Parameters/TRefManager.h"
  #include "Common/Emv/TPinPromptHandler.h"
#endif
#include "Common/Crypto/TPublicRsa.h"
#include "Common/Crypto/TTripleDES.h"
#include "Common/Utils/T_BinaryData.h"
#include "Common/Utils/FormatConverters.h"
#include <Windows.h>

const char* cKeyFileName = "softkeys.ini";
const char* cKeys = "Keys";

const char* cKeyNames[3] = {"MPIN", "MMAC", "KLK"};

class TWin_KeyStorage{
protected:
  char FileName[260];
public:
  TWin_KeyStorage(const char* fname)
  {
    GetCurrentDirectory(sizeof(FileName), FileName);
    sprintf(FileName+strlen(FileName), "\\%s", fname);
  };

  T_BinaryData GetKey(BYTE keyidx);
  void SetKey(BYTE keyidx, const T_BinaryData& key);
};

T_BinaryData TWin_KeyStorage::GetKey(BYTE keyidx)
{
  char key[256];
  GetPrivateProfileString(cKeys, cKeyNames[keyidx], "", key, sizeof(key), FileName);
  return GetBcdDataFromString(std::string(key));
}

void TWin_KeyStorage::SetKey(BYTE keyidx, const T_BinaryData& key)
{
  WritePrivateProfileString(cKeys, cKeyNames[keyidx], key.ToAscii().c_str(), FileName);
}

Error TWin_CryptoDevice::StartPINentry(const TPinInfo& pinInfo)
{
#ifdef NEED_PIN
  PinInfo = pinInfo;
  TScreenOptions opt;
  TPinPromptMaker(pinInfo).FillScreenOptions(opt, &EnteredPin);
  DialogResult = IDesktop::Instance()->Execute(IDS_ENTER_PIN, &opt);
  return SUCCESS;
#else
  return ERROR_NO_SUPPORT;
#endif
}

Error TWin_CryptoDevice::OnlineProcessing(BYTE* pPinblock8)
{
  std::string s1 (TwoHexChars((BYTE)EnteredPin.length()) + EnteredPin + std::string(14 - EnteredPin.length(), 'F'));
  std::string s2 = std::string("0000") + std::string(PinInfo.Pan+strlen(PinInfo.Pan)-13, 12);
  
  T_BinaryData mkey(TWin_KeyStorage(cKeyFileName).GetKey(KEYIDX_MPIN));
  if(mkey.Length() != 16)
    return ERROR_PINPAD_NO_MKEY;

  T_BinaryData skey(GetBcdDataFromString(std::string(PinInfo.SessionKey)));
  if(skey.Length() != 16)
    return ERROR_PINPAD_NO_SKEY;

  memmove(pPinblock8, TTripleDES(TTripleDES(mkey).DecryptECB(skey)).EncryptECB(GetBcdDataFromString(s1) ^ GetBcdDataFromString(s2)).GetData(), 8);
  return SUCCESS;
}

Error TWin_CryptoDevice::OfflinePlainProcessing(BYTE* pPinblock8)
{
  std::string s1 (TwoHexChars(0x20 | (BYTE)EnteredPin.length()) + EnteredPin + std::string(14 - EnteredPin.length(), 'F'));
  memmove(pPinblock8, GetBcdDataFromString(s1).GetData(), 8);
  return SUCCESS;
}

Error TWin_CryptoDevice::OfflineEncryptedProcessing(BYTE* pPinblock8)
{
  return OfflinePlainProcessing(pPinblock8);
}

Error TWin_CryptoDevice::TestPINentry(WORD* pNumDigitsEntered, BYTE* pPinblock8)
{
  if(EnteredPin.length() > 12)
    EnteredPin.erase(12, std::string::npos);

  *pNumDigitsEntered = (WORD)EnteredPin.length();
  if((SUCCESS == DialogResult) && (EnteredPin.length() >= 4))
  {
    switch(PinInfo.Mode)
    {
      case PINMODE_ONLINE:        return OnlineProcessing(pPinblock8);
      case PINMODE_OFFLINE_PLAIN: return OfflinePlainProcessing(pPinblock8);
      case PINMODE_OFFLINE_ENCR:  return OfflineEncryptedProcessing(pPinblock8);
      default:                    return ERROR_NO_SUPPORT;
    }
  }
  else
    return ERROR_USER_CANCELLED;
}

void TWin_CryptoDevice::StopPINentry()
{
}

TWin_CryptoDevice::TWin_CryptoDevice() : DialogResult(ERROR_PROCESSING)
{
}

Error TWin_CryptoDevice::GetSerialNumber (char* value)
{
  DWORD serNum, mcLen, dwFlags;
  char vName[50], fName[50];

  GetVolumeInformation(NULL,vName,sizeof(vName), &serNum,&mcLen,&dwFlags, fName,sizeof(fName));
  sprintf(value,"%012lu",  serNum);
  return SUCCESS;
}

BOOLEAN TWin_CryptoDevice::GetKeyCheckValue(BYTE keyIdx, BYTE* pKCV3)
{
  T_BinaryData mkey(TWin_KeyStorage(cKeyFileName).GetKey(keyIdx));
  if(mkey.Length() != 16)
    return FALSE;

  memmove(pKCV3, (TTripleDES(mkey).EncryptECB(T_BinaryData(NULL, 8))).GetData(), 3);
  return TRUE;
}

Error TWin_CryptoDevice::LoadMasterKey (BYTE keyIdx, const char* pKey, BOOLEAN IsEncrypted)
{
  T_BinaryData klk(TWin_KeyStorage(cKeyFileName).GetKey(KEYIDX_KLK));
  TWin_KeyStorage(cKeyFileName).SetKey(keyIdx, TTripleDES(klk).DecryptECB(GetBcdDataFromString(std::string(pKey))));
  return SUCCESS;
}

Error TWin_CryptoDevice::DeleteKey(BYTE keyIdx)
{
  LoadMasterKey(keyIdx, "", TRUE);
  return SUCCESS;
}

Error TWin_CryptoDevice::CalculateMAC(const TMacInfo& MacInfo, BYTE* pMac8)
{
  T_BinaryData mkey(TWin_KeyStorage(cKeyFileName).GetKey(KEYIDX_MMAC));
  if(mkey.Length() != 16)
    return ERROR_PINPAD_NO_MKEY;

  T_BinaryData skey(GetBcdDataFromString(std::string(MacInfo.SessionKey)));
  if(skey.Length() != 16)
    return ERROR_PINPAD_NO_SKEY;

  memmove(pMac8, TTripleDES(TTripleDES(mkey).DecryptECB(skey)).CalculateMAC_X9_19(T_BinaryData(MacInfo.pData, MacInfo.Length)).GetData(), 8);
  return SUCCESS;
}

Error TWin_CryptoDevice::EncryptData(const TMacInfo& MacInfo, BYTE* pDest)
{
  T_BinaryData mkey(TWin_KeyStorage(cKeyFileName).GetKey(KEYIDX_MMAC));
  if(mkey.Length() != 16)
    return ERROR_PINPAD_NO_MKEY;

  T_BinaryData skey(GetBcdDataFromString(std::string(MacInfo.SessionKey)));
  if(skey.Length() != 16)
    return ERROR_PINPAD_NO_SKEY;

  memmove(pDest, TTripleDES(TTripleDES(mkey).DecryptECB(skey)).EncryptECB(T_BinaryData(MacInfo.pData, MacInfo.Length)).GetData(), (MacInfo.Length+7)/8*8);
  return SUCCESS;
}

Error TWin_CryptoDevice::RemoteKeyLoading(IRemoteKeyLoading** p)
{
  *p = NULL;
  return ERROR_NO_SUPPORT;
}


