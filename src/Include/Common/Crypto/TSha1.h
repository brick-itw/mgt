#ifndef _U2_SHA_1_H
#define _U2_SHA_1_H

#include "Common/Utils/T_BinaryData.h"
#include "Common/Crypto/my_sha1.h"

class TSha1{
private:

  SHA_CTX ctx;

public:

  TSha1()
  {
    Clear();
  };

  TSha1(const T_BinaryData& data)
  {
    Clear();
    AddDataToHash(data);
  };

  void Clear()
  {
    SHAInit (&ctx);
  };

  void AddDataToHash(const T_BinaryData& data)
  {
    SHAUpdate (&ctx, data.GetData(), data.Length());
  };

  T_BinaryData OutputHash()
  {
    BYTE buf[SHA_DIGESTSIZE];
    SHAFinal(buf, &ctx);
    return T_BinaryData(buf, sizeof(buf));
  };

  T_BinaryData HashData(const T_BinaryData& data)
  {
    Clear();
    AddDataToHash(data);
    return OutputHash(); 
  }
};

#endif
