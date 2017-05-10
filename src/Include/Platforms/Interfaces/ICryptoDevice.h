/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Crypto device interface declaration
 */

#ifndef _U2_CRYPTODEVICE_H
#define _U2_CRYPTODEVICE_H

#include "Base/Types/types.h"
#include "Platforms/Interfaces/IDevice.h"
#include "Common/Crypto/rsaref.h"

#include <string>

#define KEYIDX_MPIN      0
#define KEYIDX_MMAC      1
#define KEYIDX_KLK       2
#define KEYIDX_SPIN      3
#define KEYIDX_SMAC      4
#define KEYIDX_DK        5

#define PINPAD_ERROR(code) REGISTER_ERROR(MODULE_CRYPTO, (code))
enum PinpadErrorCode
{
  ERROR_WRONG_OFFLINE_PIN   = PINPAD_ERROR(1),
  ERROR_OFFLINE_PIN_BLOCKED = PINPAD_ERROR(2),
  ERROR_PINPAD_FAILURE      = PINPAD_ERROR(3),
  ERROR_PINPAD_NO_MKEY      = PINPAD_ERROR(4),
  ERROR_PINPAD_NO_SKEY      = PINPAD_ERROR(5),
  ERROR_GET_SERIAL          = PINPAD_ERROR(6),
  ERROR_WRITE_KEY           = PINPAD_ERROR(7),
  ERROR_PINPAD_USER_CANCEL  = PINPAD_ERROR(8),
  ERROR_PINPAD_UNKNOWN_ERR  = PINPAD_ERROR(9),
  ERROR_WRONG_KEY_TYPE      = PINPAD_ERROR(10),
  ERROR_CALCULATE_MAC       = PINPAD_ERROR(11),
  ERROR_ENCRYPT_DATA        = PINPAD_ERROR(12),
};

#define MAX_SKEY_LEN       (120+1)

struct TMacInfo {
  const BYTE* pIV8;
  const BYTE* pData;
  DWORD Length;
  char  SessionKey[MAX_SKEY_LEN]; // ASCII or GISKE, null-terminated

  TMacInfo(const BYTE* piv, const BYTE* pdata, DWORD length, const char* skey)
    : pIV8(piv), pData(pdata), Length(length)
  {
    _safecopy(SessionKey, skey);
  };
};


typedef enum{
  PINMODE_ONLINE        = 0,
  PINMODE_OFFLINE_PLAIN = 1,
  PINMODE_OFFLINE_ENCR  = 2
}PINMODE;

#define MAX_PIN_PAN_LEN    (19+1)
#define MAX_AMOUNT_LEN     6
struct TPinInfo {
  PINMODE        Mode;

  WORD           CurrencyCode;  // 643, 840, 978, etc.
  LANGUAGE_CODE  LanguageCode;  
  BYTE           NumTriesLeft;  
  BYTE           AmountHex [MAX_AMOUNT_LEN];

  char Pan       [MAX_PIN_PAN_LEN];
  char SessionKey[MAX_SKEY_LEN]; // ASCII or GISKE, null-terminated
  RRSA_PUBLIC_KEY PubKey;

  TPinInfo() : Mode(PINMODE_ONLINE), CurrencyCode(0), LanguageCode(L_RUSSIAN), NumTriesLeft(0)
  {
      memset(AmountHex, 0, sizeof(AmountHex));
      memset(Pan, 0, sizeof(Pan));
      memset(SessionKey, 0, sizeof(SessionKey));
      memset(&PubKey, 0, sizeof(PubKey));
  };

  TPinInfo(
    PINMODE mode,
    const BYTE* amount6,
    WORD  curr,
    LANGUAGE_CODE  lang,
    BYTE  tries,
    const char* pan,
    const char* skey,
    const RRSA_PUBLIC_KEY* pubkey
  ) : Mode(mode), CurrencyCode(curr), LanguageCode(lang), NumTriesLeft(tries)
  {
    if(amount6 != 0)
    	memmove(AmountHex, amount6, sizeof(AmountHex));
    else
      memset(AmountHex, 0, sizeof(AmountHex));

    _safecopy(Pan, pan);
    _safecopy(SessionKey, skey);

    if(pubkey)
      PubKey = *pubkey;
    else
      memset(&PubKey, 0, sizeof(PubKey));
  };
};

class IRemoteKeyLoading
{
  virtual Error        GetDeviceCertificate(BYTE* pDeviceCert, DWORD* pCertLen) = 0;
  virtual Error        SetHostCertificate(const BYTE* pHostCert, DWORD CertLen) = 0;
  virtual Error        LoadKlk(
    const BYTE* pEncryptedKey, DWORD EncKeyLen,
    const BYTE* pSignature,    DWORD SigLen,
    const BYTE* pExtraData,    DWORD ExtraLen
  ) = 0;

  virtual ~IRemoteKeyLoading() {}
};

class ICryptoDevice : public IDevice {
public:

  virtual Error   GetSerialNumber (char* value) = 0;
  virtual BOOLEAN GetKeyCheckValue(BYTE keyIdx, BYTE* pKCV3) = 0; // returns TRUE if key exists, FALSE otherwise
  virtual Error   LoadMasterKey   (BYTE keyIdx, const char* pKey, BOOLEAN IsEncrypted) = 0; // key in ASCII or GISKE format, NULL-terminated
  virtual Error   DeleteKey       (BYTE keyIdx) = 0;
  virtual Error   CalculateMAC    (const TMacInfo& MacInfo, BYTE* pMac8) = 0;
  virtual Error   EncryptData     (const TMacInfo& MacInfo, BYTE* pDest) = 0;

  virtual Error RemoteKeyLoading(IRemoteKeyLoading**) = 0;
  //virtual Error PinCapturing(IPinCapturing**) = 0;

  virtual BOOLEAN IsOfflinePINchecked() = 0;

  virtual Error StartPINentry(const TPinInfo& PinInfo) = 0;

  /*
  	  Returns ERR_OK if PIN entry successfully completed (and verified in case of offline PIN).
  	    If online  PIN was entered, pPinblock8 must be filled.
  	    If offline PIN was entered, it must be verified by ICC internally. If verification fails, error code 5043 must be returned.

  	  Returns ERROR_PROCESSING and fills pNumDigitsEntered if PIN entry is still in progress.

  	  Returns ERROR_USER_CANCELLED if PIN entry is cancelled by RED button.

  	  Returns an error code in case of hardware error.
  */
  virtual Error TestPINentry(WORD* pNumDigitsEntered, BYTE* pPinblock8)  = 0;

  virtual void  StopPINentry() = 0;

};

#endif
