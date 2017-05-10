#ifndef _U2_PUBLIC_RSA_H
#define _U2_PUBLIC_RSA_H

#include "Base/Types/types.h"
#include "Common/Utils/T_BinaryData.h"
#include "Common/Crypto/my_rsa.h"

class TPublicRsa{
protected:
  T_BinaryData  Modulus;
  T_BinaryData  Exponent;

public:
  TPublicRsa(const T_BinaryData& mod, const T_BinaryData& exp)
    :Modulus(mod), Exponent(exp)
  {};

  T_BinaryData Decrypt(const T_BinaryData& data);
  void FillRsaKeyStruct(RRSA_PUBLIC_KEY& RsaKey);
};


#endif

