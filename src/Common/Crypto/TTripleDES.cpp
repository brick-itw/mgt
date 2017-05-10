#include "Common/Crypto/TTripleDES.h"

T_BinaryData TTripleDES::_directDES (const T_BinaryData& data)
{
  BYTE buf[DES_BLOCK_SIZE];

  DirectDES(data.GetData(), Key.GetData(), buf);
  if(Key.Length() >= 16)
  {
    ReverseDES(buf, Key.GetData()+8, buf);
    DirectDES (buf, Key.GetData()+((Key.Length() >= 24) ? 16:0), buf);
  }
  return T_BinaryData(buf, sizeof(buf));
}

T_BinaryData TTripleDES::_reverseDES(const T_BinaryData& data)
{
  BYTE buf[8];
  ReverseDES(data.GetData(), Key.GetData(), buf);
  if(Key.Length() >= 16)
  {
    DirectDES (buf, Key.GetData()+8, buf);
    ReverseDES(buf, Key.GetData()+((Key.Length() >= 24) ? 16:0), buf);
  }
  return T_BinaryData(buf, sizeof(buf));
}

T_BinaryData TTripleDES::EncryptECB(const T_BinaryData& data)
{
  T_BinaryData d;
  for(DWORD i=0; i<data.Length(); i+= DES_BLOCK_SIZE)
    d += _directDES(data.CopyZeroPadded(i, DES_BLOCK_SIZE)); 
  return d;
}

T_BinaryData TTripleDES::EncryptCBC(const T_BinaryData& data)
{
  T_BinaryData d, r(NULL, DES_BLOCK_SIZE);
  for(DWORD i=0; i<data.Length(); i+= DES_BLOCK_SIZE)
  {
    r = _directDES(data.CopyZeroPadded(i, DES_BLOCK_SIZE) ^ r); 
    d += r;
  }
  return d;
}

T_BinaryData TTripleDES::DecryptECB(const T_BinaryData& data)
{
  T_BinaryData d;
  for(DWORD i=0; i<data.Length(); i+= DES_BLOCK_SIZE)
    d += _reverseDES(data.CopyZeroPadded(i, DES_BLOCK_SIZE)); 
  return d;
}

T_BinaryData TTripleDES::DecryptCBC(const T_BinaryData& data)
{
  T_BinaryData d, r(NULL, DES_BLOCK_SIZE);
  for(DWORD i=0; i<data.Length(); i+= DES_BLOCK_SIZE)
  {
    d += _reverseDES(data.CopyZeroPadded(i, DES_BLOCK_SIZE)) ^ r; 
    r = data.CopyZeroPadded(i, DES_BLOCK_SIZE);
  }
  return d;
}

T_BinaryData TTripleDES::CalculateMAC_X9_19(const T_BinaryData& data)
{
  T_BinaryData r(NULL, DES_BLOCK_SIZE);
  TTripleDES subDes1(Key.Copy(0,8));
  TTripleDES subDes2(Key.Copy(8,8));

  for(DWORD i=0; i<data.Length(); i+= DES_BLOCK_SIZE)
    r = subDes1.EncryptECB(data.CopyZeroPadded(i, DES_BLOCK_SIZE) ^ r); 

  return subDes1.EncryptECB(subDes2.DecryptECB(r)); 
}

