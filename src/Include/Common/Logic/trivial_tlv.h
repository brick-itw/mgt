#ifndef _MGT_TRIVIAL_TLV_H
#define _MGT_TRIVIAL_TLV_H

#include "Common/Utils/T_TLV_Buffer.h"

class TTrivialTlvBuffer : public T_TLV_Buffer{
protected:

  virtual T_TLV_Item DoFindTag(DWORD tag);
  virtual void AddTagData();

public:
  TTrivialTlvBuffer(const TBinaryData& data);
  TTrivialTlvBuffer(const TTrivialTlvBuffer& other);

  TTrivialTlvBuffer& opeartor=(const TTrivialTlvBuffer& other);
  
};

#endif

