#ifndef TLV_H
#define TLV_H


//-----------------------------------------------------------------------------------------------------------------------------
#include "Platforms/Interfaces/ISystem.h"
#include "Common/Utils/T_BinaryData.h"
#include <string>
//-----------------------------------------------------------------------------------------------------------------------------

typedef unsigned long TLVLENTYPE;
typedef unsigned long TAGTYPE;

enum TLVADDMODE
{
	TLV_ADD, TLV_REPLACE, TLV_ADD_AS_LOG
};

enum TLVSEARCHMODE
{
	TLV_PLAIN, TLV_RECURSE, TLV_DELETE, TLV_DOL
};

class T_TLV_Item;

class T_TLV_Buffer
{

protected:

	unsigned char* pData;
	mutable TLVLENTYPE Length;
	mutable TLVLENTYPE SearchOffset;
	int bOwnsBuffer, bNoRecursiveSearch;

	void CopyFrom(const BYTE* p, TLVLENTYPE NewLength);
	void CopyFrom(const T_TLV_Buffer& copy);
	void CopyFrom(const T_BinaryData& copy);

	int IsConstructed(TAGTYPE tag) const;
	virtual T_TLV_Item DoFindTag(TAGTYPE tag, TLVSEARCHMODE mode = TLV_RECURSE) const;   // creates a new object if tag is found
  bool RemovePaddingAtCurrentLocation();
  bool IsPaddingByte(BYTE b);

public:

	T_TLV_Buffer(const unsigned char* p = 0, TLVLENTYPE len = 0);
	T_TLV_Buffer(const T_TLV_Buffer& copy);
	T_TLV_Buffer(const T_BinaryData& copy, int bNoRecurr = 0);
	T_TLV_Buffer(IFileSystem* pfsys, const std::string& cfname, int bNoRecurr = 0);
	~T_TLV_Buffer();

	void Clear();
	TLVLENTYPE GetLength() const
	{
		return Length;
	}
	unsigned char* GetData() const
	{
		return pData;
	}

	T_TLV_Item FindTag(TAGTYPE tag, TLVSEARCHMODE mode = TLV_RECURSE) const;
	T_TLV_Item FindFirstTag(TLVSEARCHMODE mode = TLV_PLAIN) const;
	T_TLV_Item FindNextTag(TLVSEARCHMODE mode = TLV_PLAIN) const;
	T_TLV_Item FindNthTag(int idx, TLVSEARCHMODE mode = TLV_PLAIN) const;

	T_TLV_Item operator[](TAGTYPE tag) const;

	virtual TLVLENTYPE AddTagData(TAGTYPE tag, const unsigned char* p, TLVLENTYPE len, TLVADDMODE mode = TLV_REPLACE);

	TLVLENTYPE AddTagData(TAGTYPE tag, const T_BinaryData& data, TLVADDMODE mode = TLV_REPLACE);

	TLVLENTYPE AddTagData(TAGTYPE tag, const std::string& s, TLVADDMODE mode = TLV_REPLACE);

	TLVLENTYPE AddTagDWORD(TAGTYPE tag, unsigned long val, TLVADDMODE mode = TLV_REPLACE);

	TLVLENTYPE AddTag(T_TLV_Item Item, TLVADDMODE mode = TLV_REPLACE);
	TLVLENTYPE DelTagData(TAGTYPE tag);

	T_TLV_Buffer operator+(const T_TLV_Item& item) const;
	void operator+=(T_TLV_Item item);

	T_TLV_Buffer operator+(const T_TLV_Buffer& other) const;
	void operator+=(const T_TLV_Buffer& other);

	T_TLV_Buffer& operator=(const T_TLV_Buffer& copy);
	T_TLV_Buffer& operator=(const T_BinaryData& copy);

	TLVLENTYPE CopyTagFrom(const T_TLV_Buffer& other, TAGTYPE tag);
 	T_BinaryData AsBinary() const;

	void RemovePaddingBytes();

 	T_TLV_Buffer FilterTags(const DWORD* pTagList) const;

	friend class T_TLV_Item;
};

class T_TLV_Item
{

private:
	T_TLV_Buffer Buffer;
	TAGTYPE Tag;
	TLVLENTYPE TagOffset;
	TLVLENTYPE FullLength;

	void Init(DWORD tag);
	void SetParentData(TLVLENTYPE tagoffset, TLVLENTYPE fulllen);
	void CopyFrom(const T_TLV_Item& other);

public:
	T_TLV_Item(const T_TLV_Item& copy);
	//T_TLV_Item(T_TLV_Item&& copy);
	T_TLV_Item();
	T_TLV_Item(TAGTYPE tag, const unsigned char* p = 0, TLVLENTYPE len = 0, 
             TLVLENTYPE tagoffset = 0, TLVLENTYPE fulllen = 0);
	T_TLV_Item(TAGTYPE tag, unsigned long value);
	T_TLV_Item(TAGTYPE tag, const T_TLV_Buffer& buf);
	T_TLV_Item& operator=(const T_TLV_Item& other);
	//T_TLV_Item& operator=(T_TLV_Item&& other);

	~T_TLV_Item();

	TAGTYPE GetTag() const;
	TLVLENTYPE GetDataLength() const;
	TLVLENTYPE GetFullLength() const;
	TLVLENTYPE GetTagOffset() const;
	unsigned char* GetDataPtr();
	const T_TLV_Buffer& GetBuffer() const;


	unsigned long AsInteger() const;
	unsigned long AsIntegerDef(unsigned long defvalue) const;
	unsigned long AsBCD() const;
	std::string AsString() const;
	std::string AsBcdString() const;
	std::string AsBcdStringDef(const std::string& defvalue) const;
	std::string AsBcdStringTrimF() const;
  std::string AsBcdAmount(int exponent) const;

	T_BinaryData AsBinary() const;
	T_BinaryData AsBinaryDefFixed(const T_BinaryData& defvalue) const;
	T_BinaryData AsBinaryDefAny(const T_BinaryData& defvalue) const;

	T_BinaryData BinaryFromAscii() const;
	std::string  AsIntToString(int len) const;
	std::string  AsStringDefFixed(const std::string& defvalue) const;
	std::string  AsStringDefAny(const std::string& defvalue) const;

	T_TLV_Buffer operator+(const T_TLV_Item& other);
	T_TLV_Item operator[](TAGTYPE tag) const;
};

//-----------------------------------------------------------------------------------------------------------------------------

#endif
