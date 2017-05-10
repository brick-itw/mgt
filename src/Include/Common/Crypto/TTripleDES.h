#ifndef _U2_DES_H
#define _U2_DES_H

#include "Common/Utils/T_BinaryData.h"
#include "Common/Crypto/my_des.h"

#define DES_BLOCK_SIZE  8

class TTripleDES{
private:
  T_BinaryData Key;

  T_BinaryData _directDES (const T_BinaryData& data);
  T_BinaryData _reverseDES(const T_BinaryData& data);

public:
  TTripleDES(const T_BinaryData& key) : Key(key) {};

  T_BinaryData EncryptECB(const T_BinaryData& data);
  T_BinaryData EncryptCBC(const T_BinaryData& data);
  T_BinaryData DecryptECB(const T_BinaryData& data);
  T_BinaryData DecryptCBC(const T_BinaryData& data);
  T_BinaryData CalculateMAC_X9_19(const T_BinaryData& data);
};

#endif

