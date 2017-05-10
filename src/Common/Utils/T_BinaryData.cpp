//-----------------------------------------------------------------------------------------------------------------------------
#include "Common/Utils/T_BinaryData.h"
#include "Common/Utils/byteorder.h"
#include "Common/Utils/FormatConverters.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
//-----------------------------------------------------------------------------------------------------------------------------

void T_BinaryData::Assign(const BYTE* p, DWORD len, DWORD alignment)
{
	if (pData)
		delete[] pData;

	pData = 0;
  DWORD full_len = len;
  if((len > 0) && (alignment > 1))
    full_len = (len + alignment - 1) / alignment * alignment;

	Count = full_len;
	if(full_len)
	{
		pData = new BYTE[full_len];
		memset(pData, 0, full_len);
		if (p)
			memmove(pData, p, len);
	}
}

void T_BinaryData::Clear()
{
	Assign(0, 0);
}

void T_BinaryData::Add(const BYTE* p, DWORD len)
{
	if (len)
	{
		BYTE* p2 = new BYTE[Count + len];
    if(pData)
    {
		  memmove(p2, pData, Count);
		  delete[] pData;
    }
		pData = p2;
		memmove(pData + Count, p, len);
		Count += len;
	}
}

void T_BinaryData::AddDirectBytes(BYTE b, DWORD len)
{
	if (len)
	{
		BYTE* p2 = new BYTE[Count + len];
    if(pData)
    {
  		memmove(p2, pData, Count);
	  	delete[] pData;
    }
		pData = p2;
		memset(pData + Count, b, len);
		Count += len;
	}
}

T_BinaryData::T_BinaryData()
	: pData(0)
{
	Clear();
}

T_BinaryData::T_BinaryData(const BYTE* p, DWORD len, DWORD alignment)
	: pData(0)
{
	Assign(p, len, alignment);
}

T_BinaryData::T_BinaryData(const T_BinaryData& other)
	: pData(0)
{
	Assign(other.Data(), other.Length());
}

T_BinaryData::T_BinaryData(const std::string& other)
	: pData(0)
{
	Assign((const BYTE*)other.c_str(), (DWORD)other.length());
}

T_BinaryData::T_BinaryData(IFileReadable& File)
	: pData(0)
{
	ReadFromFile(File);
}

T_BinaryData::~T_BinaryData()
{
	Clear();
}

T_BinaryData T_BinaryData::operator+(const T_BinaryData& other) const
{
	T_BinaryData NewObject = *this;
	NewObject += other;
	return NewObject;
}

T_BinaryData T_BinaryData::operator^(const T_BinaryData& other) const
{
	T_BinaryData NewObject = (Count >= other.Length()) ? *this  : other;
  const T_BinaryData* p2 = (Count >= other.Length()) ? &other : this;

  for(DWORD i=0; (i<NewObject.Length()) && (i<p2->Length()); ++i)
	  NewObject.Data()[i] ^= p2->GetData()[i];
	return NewObject;
}

T_BinaryData T_BinaryData::operator|(const T_BinaryData& other) const
{
	T_BinaryData NewObject = (Count >= other.Length()) ? *this  : other;
  const T_BinaryData* p2 = (Count >= other.Length()) ? &other : this;

  for(DWORD i=0; (i<NewObject.Length()) && (i<p2->Length()); ++i)
	  NewObject.Data()[i] |= p2->GetData()[i];
	return NewObject;
}

T_BinaryData T_BinaryData::operator&(const T_BinaryData& other) const
{
	T_BinaryData NewObject = (Count >= other.Length()) ? *this  : other;
  const T_BinaryData* p2 = (Count >= other.Length()) ? &other : this;

  for(DWORD i=0; (i<NewObject.Length()) && (i<p2->Length()); ++i)
	  NewObject.Data()[i] &= p2->GetData()[i];
	return NewObject;
}

T_BinaryData& T_BinaryData::operator+=(const T_BinaryData& other)
{
	Add(other.Data(), other.Length());
	return *this;
}

T_BinaryData& T_BinaryData::operator=(const T_BinaryData& other)
{
	Assign(other.Data(), other.Length());
	return *this;
}

bool T_BinaryData::operator==(const T_BinaryData& other) const
{
	if (Count != other.Length())
		return 0;
	return memcmp(pData, other.Data(), Count) ? false:true;
}

bool T_BinaryData::operator!=(const T_BinaryData& other)  const
{
	return !(*this == other);
}

bool T_BinaryData::operator<(const T_BinaryData& other) const
{
  DWORD cmplen = std::min(Count, other.Length());
  int cmpres = memcmp(pData, other.Data(), cmplen);
  if(cmpres != 0)
    return (cmpres < 0);
  return (Count < other.Length());
}

BYTE T_BinaryData::operator[](DWORD idx)
{
	return pData[idx];
}

std::string T_BinaryData::ToAscii() const
{
	if (!pData || !Count)
		return std::string("");

	char* p = new char[Count * 2 + 1];
	BytesToHex(pData, p, Count);

	std::string s(p, Count * 2);

	delete[] p;
	return s;
}

std::string T_BinaryData::ToAsciiTrimF() const
{
	if (!pData || !Count)
		return std::string("");

	char* p = new char[Count * 2 + 1];
	BytesToHex(pData, p, Count);

	std::string s(p, Count * 2);
	size_t len = s.length();
	if((len > 0) && (s[len-1] == 'F'))
		s.resize(len-1);

	delete[] p;
	return s;
}

void T_BinaryData::FromAscii(const std::string& s0)
{
  std::string s(s0);
 
	size_t len = s.length();
  if(len % 2)
  {
    s += "F";
    ++len;
  }

	BYTE* p = new BYTE[len / 2 + 1];
	HexToBytes(s.c_str(), p, (int)len);
	Assign(p, (DWORD)len / 2);
	delete[] p;
}

WORD T_BinaryData::MsbWord(DWORD offs)
{
	return MSB_TO_WORD(pData + offs);
}

WORD T_BinaryData::LsbWord(DWORD offs)
{
	return LSB_TO_WORD(pData + offs);
}

DWORD T_BinaryData::MsbDword(DWORD offs)
{
	return MSB_TO_DWORD(pData + offs);
}

DWORD T_BinaryData::LsbDword(DWORD offs)
{
	return LSB_TO_DWORD(pData + offs);
}

DWORD T_BinaryData::ToInt(DWORD offs, DWORD len)
{
	return StrToInt((const char*)pData + offs, len);
}

std::string T_BinaryData::ToString(DWORD offs, DWORD len) const
{
	if (!len)
		len = Length();
  if(!len)
    return std::string();
	return std::string((char*)pData + offs, len);
}

void T_BinaryData::ShiftLeft(DWORD offs, DWORD len)
{
	memmove(pData + offs, pData + offs + len, Count - offs - len);
	Count -= len;
}

T_BinaryData T_BinaryData::Cut(DWORD offs, DWORD len)
{
	T_BinaryData n(pData + offs, len);
	ShiftLeft(offs, len);
	return n;
}

T_BinaryData T_BinaryData::Copy(DWORD offs, DWORD len) const
{
	return T_BinaryData(pData + offs, len);
}

T_BinaryData T_BinaryData::CopyZeroPadded(DWORD offs, DWORD len) const
{
  if(offs>=Count)
    return T_BinaryData(NULL, len);

  if((offs+len)>Count)
    return T_BinaryData(pData+offs, Count-offs).AddBytes(0x00, offs+len-Count);
  
	return T_BinaryData(pData + offs, len);
}


T_BinaryData T_BinaryData::AsciiToBinary()
{
	T_BinaryData n;
	std::string s = ToString(0, Count);
	n.FromAscii(s);
	return n;
}

bool T_BinaryData::DecodeFromBase64(const BYTE* pBase64, int len)
{
	int outlen;

	Assign(0, len);

	if(R_DecodePEMBlock(pData, &outlen, pBase64, len) != 0)
	{
		Clear();
		return false;
	}
	Count = (DWORD)outlen;
	return true;
}

bool T_BinaryData::DecodeFromBase64(const std::string& s)
{
	return DecodeFromBase64((BYTE*)s.c_str(), (int)s.length());
}

T_BinaryData T_BinaryData::TrimTrailingBytes(BYTE bTail)
{
	DWORD i = Count;
	while((i > 0) && (pData[i-1] == bTail))
		i--;
	return T_BinaryData(pData, i);
}

std::string T_BinaryData::EncodeToBase64()
{
	std::string s;
	int outlen;
	char* pOutData = new char[Count*2 + 4];

	R_EncodePEMBlock((BYTE*)pOutData, &outlen, pData, Count);
	pOutData[outlen] = 0;

	s = pOutData;
	delete[] pOutData;
	return s;
}

T_BinaryData T_BinaryData::AddBytes(BYTE b, DWORD len) const
{
	T_BinaryData NewData = *this;
	NewData.AddDirectBytes(b, len);
	return NewData;
}

bool T_BinaryData::OnlyZeroes()
{
  for(DWORD i=0;i<Count;++i)
    if(pData[i])
      return false;
  return true;
}

void T_BinaryData::ReadFromFile(IFileReadable& File)
{
	Clear();
	DWORD len = File.GetLength();

	if (len)
	{
		pData = new BYTE[len];
		Count = File.Read(pData, len);
	}
}

void T_BinaryData::SaveToFile(IFileWritable& File)
{
	if(Count)
		File.Write(pData, Count);
}


//-----------------------------------------------------------------------------------------------------------------------------

