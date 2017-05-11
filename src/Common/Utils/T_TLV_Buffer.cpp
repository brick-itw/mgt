#include <string.h>
#include <stdlib.h>

#include "Common/Utils/T_TLV_Buffer.h"
#include "Common/Utils/byteorder.h"
#include "Common/Utils/FormatConverters.h"
//-----------------------------------------------------------------------------------------------------------------------------

T_TLV_Buffer::T_TLV_Buffer(const unsigned char* p, TLVLENTYPE len)
{
	bOwnsBuffer = 1;
	Length = len;
	pData = 0;
  bNoRecursiveSearch = 0;
	if (p && len)
	{
		pData = new unsigned char[len];
		memmove(pData, p, len);
	}
	SearchOffset = 0;
}

T_TLV_Buffer::T_TLV_Buffer(const T_TLV_Buffer& copy)
{
	bOwnsBuffer = 1;
	Length = 0;
	pData = 0;
  bNoRecursiveSearch = 0;
	CopyFrom(copy);
}

T_TLV_Buffer::T_TLV_Buffer(const T_BinaryData& copy, int bNoRecurr)
{
	bOwnsBuffer = 1;
	Length = 0;
	pData = 0;
  bNoRecursiveSearch = bNoRecurr;
	CopyFrom(copy);
}

T_TLV_Buffer::T_TLV_Buffer(IFileSystem* pfsys, const std::string& cfname, int bNoRecurr)
{
	bOwnsBuffer = 1;
	bNoRecursiveSearch = bNoRecurr;
	Length = 0;
	pData = 0;

  IFileReadable* pFile = pfsys->OpenForRead(cfname.c_str());
  if(pFile)
  {
   	DWORD len = pFile->GetLength();
	  if(len)
    {
		  pData = new unsigned char[len];
		  Length = pFile->Read(pData, len);
    }
    delete pFile;
	}
}


void T_TLV_Buffer::Clear()
{
	if (pData && bOwnsBuffer)
	{
		delete[] pData;
	}

	Length = 0;
	SearchOffset = 0;
	pData = 0;
	bOwnsBuffer = 0;
}

T_TLV_Buffer::~T_TLV_Buffer()
{
	Clear();
}

int T_TLV_Buffer::IsConstructed(TAGTYPE tag) const
{
	if (!tag)
		return 0;
	while (!(tag & 0xFF000000))
		tag <<= 8;
	return ((tag & 0x20000000) != 0);
}

T_TLV_Item T_TLV_Buffer::DoFindTag(TAGTYPE tag, TLVSEARCHMODE mode) const
{
	unsigned long curTag = 0;
	long curLen = 0, lLen = 0, Limit = 0;

	unsigned char* mess = pData + SearchOffset;
	unsigned char* p = mess;
	unsigned char* pPrev = NULL;

	Limit = Length - SearchOffset;

	while ((p - mess + 2) <= Limit)
	{

		while(((p - mess + 2) < Limit) && !(*p))  // skip leading zeroes if any
			p++;

		pPrev = p;
		curTag = p[0];
		if ((p[0] & 0x1F) == 0x1F)
		{
			do
			{
				p++;
				curTag = (curTag << 8) | p[0];
			}
			while (((p - mess + 1) < Limit) && (p[0] & 0x80));
			if ((p - mess + 1) >= Limit)
				break;
		}

		if (!curTag)
			break;

		p++;
		curLen = 0;
		if (p[0] & 0x80)
		{
			lLen = p[0] & ~0x80;
			while (((p - mess) < Limit) && lLen)
			{
				lLen--;
				p++;
				curLen = (curLen << 8) | p[0];
			}
		}
		else
			curLen = p[0];

		p++;
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

T_TLV_Item T_TLV_Buffer::FindTag(TAGTYPE tag, TLVSEARCHMODE mode) const
{
	SearchOffset = 0;
	return DoFindTag(tag, mode);
}

T_TLV_Item T_TLV_Buffer::FindFirstTag(TLVSEARCHMODE mode) const
{
	return FindTag(0, mode);
}

T_TLV_Item T_TLV_Buffer::FindNextTag(TLVSEARCHMODE mode) const
{
	return DoFindTag(0, mode);
}

T_TLV_Item T_TLV_Buffer::FindNthTag(int idx, TLVSEARCHMODE mode) const
{
	int counter = 0;
	T_TLV_Item item = FindFirstTag(mode);
	for(;(counter<idx) && (item.GetDataLength() > 0); counter++)
		item = FindNextTag(mode);
	return item;
}


void T_TLV_Buffer::CopyFrom(const BYTE* p, TLVLENTYPE NewLength)
{
	Clear();
	bOwnsBuffer = 1;
	Length = NewLength;
	pData = new unsigned char[Length];
	if (p)
		memmove(pData, p, Length);
	else
		memset(pData, 0, Length);
}

void T_TLV_Buffer::CopyFrom(const T_TLV_Buffer& copy)
{
	CopyFrom(copy.GetData(), copy.GetLength());
}

void T_TLV_Buffer::CopyFrom(const T_BinaryData& copy)
{
	CopyFrom(copy.Data(), copy.Length());
}

T_TLV_Buffer& T_TLV_Buffer::operator=(const T_TLV_Buffer& copy)
{
	Clear();
	CopyFrom(copy);
	return (*this);
}

T_TLV_Buffer& T_TLV_Buffer::operator=(const T_BinaryData& copy)
{
	Clear();
	CopyFrom(copy);
	return (*this);
}


void T_TLV_Buffer::operator+=(const T_TLV_Buffer& other)
{
	T_TLV_Item Item = other.FindFirstTag();
	while(Item.GetTag())
	{
		AddTag(Item);
		Item = other.FindNextTag();
	}
}

T_TLV_Buffer T_TLV_Buffer::operator+(const T_TLV_Buffer& other) const
{
	T_TLV_Buffer result = *this;
	result += other;
	return result;
}

T_TLV_Buffer T_TLV_Buffer::operator+(const T_TLV_Item& item) const
{
	T_TLV_Buffer result = *this;
	result.AddTag(item);
	return result;
}

void T_TLV_Buffer::operator+=(T_TLV_Item item)
{
	AddTag(item);
}

bool T_TLV_Buffer::IsPaddingByte(BYTE b)
{
  return ((b == 0x00) || (b == 0xFF));
}

bool T_TLV_Buffer::RemovePaddingAtCurrentLocation()
{
  TLVLENTYPE orgPos, delta;

  orgPos = SearchOffset;
  while((SearchOffset < Length) && IsPaddingByte(pData[SearchOffset]))
    ++SearchOffset;

  /*
    remove any leading 0x00s and 0xFFs, maybe none 
  */
  delta = SearchOffset - orgPos;
  if(delta){
    memmove(pData + orgPos, pData + SearchOffset, Length - SearchOffset);
    Length -= delta;
  }
  SearchOffset = orgPos;
  DoFindTag(0, TLV_PLAIN);
  return (SearchOffset < Length) && (SearchOffset > orgPos);  
}

void T_TLV_Buffer::RemovePaddingBytes()
{
  if(!Length || !pData)
    return;
  SearchOffset = 0;
  while(RemovePaddingAtCurrentLocation());
}

TLVLENTYPE T_TLV_Buffer::AddTagData(TAGTYPE tag, const unsigned char* p, TLVLENTYPE len, TLVADDMODE mode)
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
	for (i = 0, flag = 0; i < 32; i += 8)
	{
		t = tag << i;
		if ((t & 0xFF000000UL) || flag)
		{
			pnew[Length++] = (unsigned char)(t >> 24);
			flag = 1;
		}
	}

	if (len > 127)
	{
		if (len > 255)
		{
			pnew[Length++] = (unsigned char)(2 | 0x80);
			pnew[Length++] = (unsigned char)(len >> 8);
		}
		else
			pnew[Length++] = (unsigned char)(1 | 0x80);
	}
	pnew[Length++] = (unsigned char)len;
	memmove(pnew + Length, p, len);
	Length += len;

	if (bOwnsBuffer && pData)
		delete[] pData;

	bOwnsBuffer = 1;
	pData = pnew;

	return Length;
}

TLVLENTYPE T_TLV_Buffer::AddTagDWORD(TAGTYPE tag, unsigned long val, TLVADDMODE mode)
{
	unsigned char buf[4];
	DWORD_TO_LSB(val, buf);
	return AddTagData(tag, buf, sizeof(buf), mode);
}

TLVLENTYPE T_TLV_Buffer::AddTagData(TAGTYPE tag, const T_BinaryData& data, TLVADDMODE mode)
{
	return AddTagData(tag, data.Data(), data.Length(), mode);
}

TLVLENTYPE T_TLV_Buffer::AddTagData(TAGTYPE tag, const std::string& s, TLVADDMODE mode)
{
	return AddTagData(tag, (BYTE*)s.c_str(), (TLVLENTYPE)s.length(), mode);
}

TLVLENTYPE T_TLV_Buffer::AddTag(T_TLV_Item Item, TLVADDMODE mode)
{
	return AddTagData(Item.GetTag(), Item.GetDataPtr(), Item.GetDataLength(), mode);
}

TLVLENTYPE T_TLV_Buffer::DelTagData(TAGTYPE tag)
{
	SearchOffset = 0;
	DoFindTag(tag, TLV_DELETE);
	return Length;
}

TLVLENTYPE T_TLV_Buffer::CopyTagFrom(const T_TLV_Buffer& other, TAGTYPE tag)
{
	TLVLENTYPE prevLength = Length;
	return AddTag(other.FindTag(tag)) - prevLength;
}

T_TLV_Item T_TLV_Buffer::operator[](TAGTYPE tag) const
{
	return FindTag(tag);
}

T_BinaryData T_TLV_Buffer::AsBinary() const
{
  return T_BinaryData(pData, Length);
}

T_TLV_Buffer T_TLV_Buffer::FilterTags(const DWORD* pTagList) const
{
  T_TLV_Buffer d;
  for(int i=0; pTagList[i]; ++i)
    d += FindTag(pTagList[i]);
  return d;
}

//--------------------------------------------------------------------------------------------------------------

void T_TLV_Item::Init(DWORD tag)
{
	Tag = tag;
	TagOffset = 0;
	FullLength = 0;
}

T_TLV_Item::T_TLV_Item(const T_TLV_Item& copy) :
		Buffer(copy.GetBuffer())
{
	Init(copy.GetTag());
	FullLength = copy.GetFullLength();
	TagOffset = copy.GetTagOffset();
}

/*
T_TLV_Item::T_TLV_Item(T_TLV_Item&& copy)
  : Buffer(copy.GetDataPtr(), copy.GetDataLength())
{
	Init(copy.GetTag());
	FullLength = copy.GetFullLength();
	TagOffset = copy.GetTagOffset();
}
*/

T_TLV_Item::T_TLV_Item(TAGTYPE tag, const unsigned char* p, TLVLENTYPE len, TLVLENTYPE tagoffset, TLVLENTYPE fulllen) :
		Buffer(p, len)
{
	Init(tag);
  SetParentData(tagoffset, fulllen);
}

T_TLV_Item::T_TLV_Item() :
		Buffer(NULL, 0)
{
	Init(0);
}

T_TLV_Item::T_TLV_Item(TAGTYPE tag, unsigned long value) :
		Buffer((unsigned char*)&value, sizeof(value))
{
	Init(tag);
}

T_TLV_Item::T_TLV_Item(TAGTYPE tag, const T_TLV_Buffer& buf) :
		Buffer(buf)
{
	Init(tag);
}


T_TLV_Item::~T_TLV_Item()
{
}

void T_TLV_Item::SetParentData(TLVLENTYPE tagoffset, TLVLENTYPE fulllen)
{
	TagOffset = tagoffset;
	FullLength = fulllen;
}

TAGTYPE T_TLV_Item::GetTag() const
{
	return Tag;
}

TLVLENTYPE T_TLV_Item::GetDataLength() const
{
	return Buffer.GetLength();
}

TLVLENTYPE T_TLV_Item::GetFullLength() const
{
	return FullLength;
}

TLVLENTYPE T_TLV_Item::GetTagOffset() const
{
	return TagOffset;
}

unsigned char* T_TLV_Item::GetDataPtr() const
{
	return Buffer.GetData();
}

unsigned long T_TLV_Item::AsInteger() const
{
	unsigned long val = 0;
	if(Buffer.GetLength() == 4)
		val = LSB_TO_DWORD(Buffer.GetData());
	if(Buffer.GetLength() == 2)
		val = LSB_TO_WORD(Buffer.GetData());
	if(Buffer.GetLength() == 1)
		val = Buffer.GetData()[0];
	return val;
}

unsigned long T_TLV_Item::AsIntegerDef(unsigned long defvalue) const
{
	if(!Buffer.GetLength())
		return defvalue;
	return AsInteger();
}


unsigned long BcdDigit(unsigned char v)
{
	return (v & 0x0F) + ((v & 0xF0) >> 4) * 10;
}

unsigned long T_TLV_Item::AsBCD() const
{
	unsigned long val = 0;
	for (TLVLENTYPE i = 0; i < Buffer.GetLength(); i++)
		val = (val * 100UL) + BcdDigit(Buffer.GetData()[i]);
	return val;
}

std::string T_TLV_Item::AsString() const
{
	return std::string((char*)Buffer.GetData(), Buffer.GetLength());
}

std::string T_TLV_Item::AsStringDefFixed(const std::string& defvalue) const
{
	if(Buffer.GetLength() != ((TLVLENTYPE)defvalue.length()))
		return defvalue;
	return AsString();
}

std::string T_TLV_Item::AsStringDefAny(const std::string& defvalue) const
{
	if(!Buffer.GetLength())
		return defvalue;
	return AsString();
}

std::string T_TLV_Item::AsIntToString(int len) const
{
	char* buf = new char[len+1];
	std::string s(IntToStr(AsInteger(), buf, len));
	delete[] buf;
	return s;
}

std::string T_TLV_Item::AsBcdString() const
{
	char* p = new char[Buffer.GetLength()*2 + 1];
	BytesToHex(Buffer.GetData(), p, Buffer.GetLength());
	std::string s(p, Buffer.GetLength() * 2);
	delete[] p;
	return s;
}

std::string T_TLV_Item::AsBcdStringTrimF() const
{
	std::string s = AsBcdString();
	size_t len = s.length();
	if((len > 0) && (s[len-1] == 'F'))
		s.resize(len-1);
	return s;
}

std::string T_TLV_Item::AsBcdStringDef(const std::string& defvalue) const
{
	if(Buffer.GetLength()*2 != ((TLVLENTYPE)defvalue.length()))
		return defvalue;
	return AsBcdString();
}


T_BinaryData T_TLV_Item::AsBinary() const
{
	return T_BinaryData(Buffer.GetData(), Buffer.GetLength());
}

T_BinaryData T_TLV_Item::AsBinaryDefFixed(const T_BinaryData& defvalue) const
{
  if(Buffer.GetLength() != (TLVLENTYPE)defvalue.Length())
    return defvalue;
	return T_BinaryData(Buffer.GetData(), Buffer.GetLength());
}

T_BinaryData T_TLV_Item::AsBinaryDefAny(const T_BinaryData& defvalue) const
{
  if(Buffer.GetLength() == 0)
    return defvalue;
	return T_BinaryData(Buffer.GetData(), Buffer.GetLength());
}

T_BinaryData T_TLV_Item::BinaryFromAscii() const
{
	int len = Buffer.GetLength();
	BYTE* p = new BYTE[len / 2 + 1];
	if(0 != HexToBytes((const char*)Buffer.GetData(), p, len))
    len = 0;
	T_BinaryData data(p, len/2);
	delete[] p;
	return data;
}

std::string T_TLV_Item::AsBcdAmount(int exponent) const
{
	return PrintableAmount(AsBcdString(), exponent);
}

const T_TLV_Buffer& T_TLV_Item::GetBuffer() const
{
	return Buffer;
}

void T_TLV_Item::CopyFrom(const T_TLV_Item& other)
{
	Tag = other.GetTag();
	Buffer.CopyFrom(other.GetBuffer());
	SetParentData( other.GetTagOffset(), other.GetFullLength());
}

T_TLV_Item& T_TLV_Item::operator=(const T_TLV_Item& other)
{
	CopyFrom(other);
	return (*this);
}

/*
T_TLV_Item& T_TLV_Item::operator=(T_TLV_Item&& other)
{
	CopyFrom(other);
	return (*this);
}
*/

T_TLV_Buffer T_TLV_Item::operator+(const T_TLV_Item& item2)
{
	return T_TLV_Buffer() + (*this) + item2;
}

T_TLV_Item T_TLV_Item::operator[](TAGTYPE tag) const
{
	return Buffer[tag];
}

//-----------------------------------------------------------------------------------------------------------------------------

