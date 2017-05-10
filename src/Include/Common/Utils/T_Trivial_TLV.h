#ifndef _MGT_TRIVIAL_TLV_H
#define _MGT_TRIVIAL_TLV_H

#include "Common/Utils/T_TLV_Buffer.h"

class T_Trivial_TLV_Buffer : public T_TLV_Buffer{
protected:
  virtual T_TLV_Item DoFindTag(TAGTYPE tag, TLVSEARCHMODE mode = TLV_RECURSE) const;

public:
  T_Trivial_TLV_Buffer();
  T_Trivial_TLV_Buffer(const T_BinaryData& data);

  virtual TLVLENTYPE AddTagData(TAGTYPE tag, const unsigned char* p, TLVLENTYPE len, TLVADDMODE mode = TLV_REPLACE);
};

#endif
