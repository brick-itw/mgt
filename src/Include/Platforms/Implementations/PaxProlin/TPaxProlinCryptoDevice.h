#ifndef T_PIN_ENTRY_DEVICE_H
#define T_PIN_ENTRY_DEVICE_H

#include "Platforms/Interfaces/ICryptoDevice.h"

#include <string>

class TPaxProlinCryptoDevice : public ICryptoDevice
{
private:
#define PIN_BLOCK_LEN 8

  int num_digits_entered;
  BYTE pinblock[100];

  Error PaxInternalLoadKey(BYTE  cell, BYTE  key_type, BYTE  master_cell, BYTE  master_key_type, const char* pValue32);
  BYTE PaxGetKeyCellForKeyType(BYTE uposkeyIdx);
  BYTE PaxGetKeyTypeForKeyType(BYTE uposkeyIdx);
  BYTE PaxGetMasterKeyCellForKeyType(BYTE uposkeyIdx);
  BYTE PaxGetMasterKeyTypeForKeyType(BYTE uposkeyIdx);

public:
	TPaxProlinCryptoDevice();
	~TPaxProlinCryptoDevice();

	virtual Error Open(){return SUCCESS;}
	virtual void Close(){}

	virtual Error   GetSerialNumber (char* value);
	virtual BOOLEAN GetKeyCheckValue(BYTE keyIdx, BYTE* pKCV3); // returns TRUE if key exists, FALSE otherwise
	virtual Error   LoadMasterKey   (BYTE keyIdx, const char* pKey, BOOLEAN IsEncrypted); // key in ASCII or GISKE format, NULL-terminated
	virtual Error   DeleteKey       (BYTE keyIdx);
	virtual Error   CalculateMAC    (const TMacInfo& MacInfo, BYTE* pMac8);
	virtual Error   EncryptData     (const TMacInfo& MacInfo, BYTE* pDest);

	Error PaxLoadKey(BYTE UposKeyIdx, BOOLEAN IsEncrypted, const char* pValue32);
    
  virtual Error TestPINentry(WORD* pNumDigitsEntered, BYTE* pPinblock8);
  virtual void  StopPINentry(){}

	virtual Error RemoteKeyLoading(IRemoteKeyLoading**){return -1;}
	//virtual Error PinCapturing(IPinCapturing**){return -1;}

	virtual BOOLEAN IsOfflinePINchecked(){return false;}

	virtual Error StartPINentry(const TPinInfo& PinInfo);
	virtual Error Test(WORD* pNumDigitsEntered, BYTE* pPinblock8);
  virtual void Stop();

};










#endif
