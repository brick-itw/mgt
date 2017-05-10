#ifndef _U2_WIN_CRYPTO_DEVICE_H
#define _U2_WIN_CRYPTO_DEVICE_H

#include "Platforms/Interfaces/ICryptoDevice.h"


class TWin_CryptoDevice : public ICryptoDevice {
protected:
  TPinInfo PinInfo;
  Error DialogResult;
  std::string EnteredPin;

  Error OnlineProcessing(BYTE* pPinblock8);
  Error OfflinePlainProcessing(BYTE* pPinblock8);
  Error OfflineEncryptedProcessing(BYTE* pPinblock8);


public:

  TWin_CryptoDevice();

  virtual Error Open(){return SUCCESS;};
  virtual void  Close(){};

  virtual Error   GetSerialNumber (char* value);
  virtual BOOLEAN GetKeyCheckValue(BYTE keyIdx, BYTE* pKCV3); // returns TRUE if key exists, FALSE otherwise
  virtual Error   LoadMasterKey   (BYTE keyIdx, const char* pKey, BOOLEAN IsEncrypted);
  virtual Error   DeleteKey       (BYTE keyIdx);
  virtual Error   CalculateMAC    (const TMacInfo& MacInfo, BYTE* pMac8);
  virtual Error   EncryptData     (const TMacInfo& MacInfo, BYTE* pDest);

  virtual Error RemoteKeyLoading(IRemoteKeyLoading**);

  virtual Error StartPINentry(const TPinInfo& pinInfo);
  virtual Error TestPINentry(WORD* pNumDigitsEntered, BYTE* pPinblock8);
  virtual void  StopPINentry();


  virtual BOOLEAN IsOfflinePINchecked() {return FALSE;};
};


#endif
