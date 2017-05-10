#include "Common/Utils/T_Trivial_TLV.h"
#include "Common/Utils/byteorder.h"

T_Trivial_TLV_Buffer::T_Trivial_TLV_Buffer() : T_TLV_Buffer()
{
}

T_Trivial_TLV_Buffer::T_Trivial_TLV_Buffer(const T_BinaryData& data) : T_TLV_Buffer(data)
{
}

TLVLENTYPE T_Trivial_TLV_Buffer::AddTagData(TAGTYPE tag, const unsigned char* p, TLVLENTYPE len, TLVADDMODE mode)
{
	int i=0, flag=0;
	unsigned long t=0;

	if(!tag)
		return Length;

	if (mode == TLV_REPLACE)
		DelTagData(tag);

	if((len == 0) && (mode != TLV_ADD_AS_LOG))
		return Length;

	unsigned char* pnew = new unsigned char[Length + 4 + 4 + len];

	memmove(pnew, pData, Length);
  DWORD_TO_MSB(tag, pnew+Length);
  Length += sizeof(DWORD);
 
  WORD_TO_MSB((WORD)len, pnew+Length);
  Length += sizeof(WORD);

	memmove(pnew + Length, p, len);
	Length += len;

	if (bOwnsBuffer && pData)
		delete[] pData;

	bOwnsBuffer = 1;
	pData = pnew;

	return Length;
}

T_TLV_Item T_Trivial_TLV_Buffer::DoFindTag(TAGTYPE tag, TLVSEARCHMODE mode) const
{
	unsigned long curTag = 0;
	long curLen = 0, lLen = 0, Limit = 0;

	unsigned char* mess = pData + SearchOffset;
	unsigned char* p = mess;
	unsigned char* pPrev = NULL;

	Limit = Length - SearchOffset;

	while ((p - mess + 6) <= Limit)
	{
    curTag = MSB_TO_DWORD(p);
    p += sizeof(DWORD);
		curLen = MSB_TO_WORD(p);
    p += sizeof(WORD);

    if (((p - mess + curLen) <= Limit) || (mode == TLV_DOL))
		{
			if ((curTag == tag) || !tag)
			{

				SearchOffset = (TLVLENTYPE)(p - pData);
				if (mode != TLV_DOL)
					SearchOffset += curLen;

				if (mode == TLV_DELETE)
				{
					memmove(pPrev, p + curLen, Limit - (p + curLen - mess));
					Length -= ((TLVLENTYPE)(p - pPrev) + curLen);
					break;
				}
				return T_TLV_Item((TAGTYPE)curTag, (unsigned char*)((mode == TLV_DOL) ? 0: p), (TLVLENTYPE)curLen, (TLVLENTYPE)(pPrev-pData), (TLVLENTYPE)((p+curLen)-pPrev));
			}
			else
			{
				if (IsConstructed(curTag) && (mode == TLV_RECURSE) && !bNoRecursiveSearch)
				{ // constructed object
					T_TLV_Item Item(T_TLV_Buffer(p, curLen).FindTag(tag, mode));
					if (Item.GetDataLength())
						return Item;
				}
			}
		}
		else
			break;

		if (mode != TLV_DOL)
			p += curLen;
	}
	return T_TLV_Item();
}

