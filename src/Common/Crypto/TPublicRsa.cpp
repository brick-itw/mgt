#include "Common/Crypto/TPublicRsa.h"
#include <string.h>

void TPublicRsa::FillRsaKeyStruct(RRSA_PUBLIC_KEY& RsaKey)
{
  memset(&RsaKey, 0, sizeof(RRSA_PUBLIC_KEY));
  RsaKey.bits = (WORD)(Modulus.Length()*8);
  memmove(RsaKey.modulus  + (MAX__RSA_MODULUS_LEN - Modulus.Length()),  Modulus.GetData(),  Modulus.Length());
  memmove(RsaKey.exponent + (MAX__RSA_MODULUS_LEN - Exponent.Length()), Exponent.GetData(), Exponent.Length());
}

T_BinaryData TPublicRsa::Decrypt(const T_BinaryData& data)
{
  RRSA_PUBLIC_KEY key;
  BYTE buf[MAX__RSA_MODULUS_LEN];
  unsigned int uiOutLen;

  FillRsaKeyStruct(key);
  if(0 == RRSAPublicDecrypt (buf, &uiOutLen, data.GetData(), Modulus.Length(), &key))
    return T_BinaryData(buf, uiOutLen);
  else
    return T_BinaryData();
}
