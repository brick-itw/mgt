#include "Common/Crypto/TPublicRsa.h"
#include "Common/Utils/FormatConverters.h"
#include <string>
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

DWORD TPrivateRsa::ImportPKdataFromNextTag(BYTE* pKeyField, size_t size, T_TLV_Buffer& rootbuf)
{
  T_TLV_Item item = rootbuf.FindNextTag();   
  const BYTE* pSrc = item.GetDataPtr();
  DWORD len = item.GetDataLength();

  if(len > size)
  {
    pSrc += len-size;
    len = (DWORD)size;
  }

  if(size > len)
  {
    pKeyField += size-len;
    size = len;
  }

  memmove(pKeyField, pSrc, len);
  return len;
}

void TPrivateRsa::LoadPrivateKeyFromPemFile(IFileReadable* pFile)
{
  const char* cStartMarker = "-----BEGIN RSA PRIVATE KEY";
  const char* cEndMarker   = "-----END RSA PRIVATE KEY";

  DWORD dwLen = pFile->GetLength();
  char* pBuf = new char[dwLen+1];
  dwLen = pFile->Read((BYTE*)pBuf, dwLen); 
  pBuf[dwLen]=0;
  std::string filedata(pBuf);

  size_t start_pos = filedata.find(cStartMarker);
  size_t end_pos = filedata.find(cEndMarker);
  if((start_pos != std::string::npos) && (end_pos != std::string::npos))
  {
    std::string pem_data;
    for(size_t i=start_pos + strlen(cStartMarker); i<end_pos; ++i)
      if(IsPemChar(filedata[i]))
        pem_data += filedata[i];

    T_BinaryData data;
    data.DecodeFromBase64(pem_data);
    T_TLV_Buffer buf(data);
    T_TLV_Buffer rootbuf = buf.FindFirstTag().GetBuffer();   
    rootbuf.FindFirstTag();   
  
    ImportPKdataFromNextTag(PrivKey.modulus, sizeof(PrivKey.modulus), rootbuf);
    ImportPKdataFromNextTag(PrivKey.publicExponent, sizeof(PrivKey.publicExponent), rootbuf);
    ImportPKdataFromNextTag(PrivKey.exponent, sizeof(PrivKey.exponent), rootbuf);
    ImportPKdataFromNextTag(PrivKey.prime[0], sizeof(PrivKey.prime[0]), rootbuf);
    ImportPKdataFromNextTag(PrivKey.prime[1], sizeof(PrivKey.prime[1]), rootbuf);
    ImportPKdataFromNextTag(PrivKey.primeExponent[0], sizeof(PrivKey.primeExponent[0]), rootbuf);
    ImportPKdataFromNextTag(PrivKey.primeExponent[1], sizeof(PrivKey.primeExponent[1]), rootbuf);
    PrivKey.bits = (WORD)ImportPKdataFromNextTag(PrivKey.coefficient, sizeof(PrivKey.coefficient), rootbuf) * 2 * 8;
  }
  delete[] pBuf;
}

TPrivateRsa::TPrivateRsa(const std::string& pemFileName)
{
  memset(&PrivKey, 0, sizeof(PrivKey));
  IFileReadable* pFile = ISystem::Instance()->GetDefaultFileSystem()->OpenForRead(pemFileName.c_str());
  if(pFile)
  {
    LoadPrivateKeyFromPemFile(pFile);
    delete pFile;
  }
}

T_BinaryData TPrivateRsa::Encrypt(const T_BinaryData& data)
{
  BYTE buf[MAX__RSA_MODULUS_LEN];
  unsigned int uiOutLen;

  if(0 == RRSAPrivateEncrypt(buf, &uiOutLen, data.GetData(), data.Length(), &PrivKey))
    return T_BinaryData(buf, uiOutLen);
  else
    return T_BinaryData();
}

