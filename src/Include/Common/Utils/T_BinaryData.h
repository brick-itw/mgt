#ifndef _U2_BINDATA_H
#define _U2_BINDATA_H

//-----------------------------------------------------------------------------------------------------------------------------
#include "Base/Types/types.h"
#include "Platforms/Interfaces/IFile.h"
#include <string>
//-----------------------------------------------------------------------------------------------------------------------------


class T_BinaryData
{
protected:
	BYTE* pData;
	DWORD Count;

	void ShiftLeft(DWORD offs, DWORD len);

public:
	T_BinaryData();
	T_BinaryData(const T_BinaryData& other);
	T_BinaryData(const std::string& other);
	T_BinaryData(const BYTE* p, DWORD len, DWORD alignment = 0);
	T_BinaryData(IFileReadable& File);
	~T_BinaryData();

	void Assign(const BYTE* p, DWORD len, DWORD alignment = 0);
	void Add(const BYTE* p, DWORD len);
	void AddDirectBytes(BYTE b, DWORD len);

	DWORD Length() const
	{
		return Count;
	}

	BYTE* Data() const
	{
		return pData;
	}

	const BYTE* GetData() const
	{
		return pData;
	}

	T_BinaryData  operator+(const T_BinaryData& other) const;
	T_BinaryData  operator^(const T_BinaryData& other) const;
	T_BinaryData  operator|(const T_BinaryData& other) const;
	T_BinaryData  operator&(const T_BinaryData& other) const;
	T_BinaryData& operator+=(const T_BinaryData& other);
	T_BinaryData& operator=(const T_BinaryData& other);
	bool operator==(const T_BinaryData& other) const;
	bool operator!=(const T_BinaryData& other) const;
	bool operator<(const T_BinaryData& other) const;
	BYTE operator[](DWORD idx);

	std::string ToAscii() const;
	std::string ToAsciiTrimF() const;
	void FromAscii(const std::string& s);

	WORD MsbWord(DWORD offs=0) const;
	WORD LsbWord(DWORD offs=0) const;
	DWORD MsbDword(DWORD offs=0) const;
	DWORD LsbDword(DWORD offs=0) const;

	DWORD ToInt(DWORD offs, DWORD len);

	std::string ToString(DWORD offs=0, DWORD len = 0) const;
	std::string EncodeToBase64();
	bool DecodeFromBase64(const std::string& s);
	bool DecodeFromBase64(const BYTE* pBase64, int len);

	void Clear();

	T_BinaryData TrimTrailingBytes(BYTE bTail);

	T_BinaryData TrimTrailingZeroes()
	{return TrimTrailingBytes(0);};

	T_BinaryData TrimTrailingFFs()
	{return TrimTrailingBytes(0xFF);};

	T_BinaryData AddBytes(BYTE b, DWORD len) const;

	T_BinaryData Cut(DWORD offs, DWORD len);
	T_BinaryData Copy(DWORD offs, DWORD len) const;
	T_BinaryData CopyZeroPadded(DWORD offs, DWORD len) const;
	T_BinaryData AsciiToBinary();
  bool OnlyZeroes();

	void ReadFromFile(IFileReadable& File);
	void SaveToFile  (IFileWritable& File);
};

#endif

//-----------------------------------------------------------------------------------------------------------------------------

