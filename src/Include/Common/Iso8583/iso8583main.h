//=============================================================================================================================
// * Name      : iso8583main.h
// * Author    : Alexey Brik
//=============================================================================================================================

#pragma once

//-----------------------------------------------------------------------------------------------------------------------------
#include "Common/Utils/T_BinaryData.h"

#include <array>
//-----------------------------------------------------------------------------------------------------------------------------
namespace ump
{

	enum ISO8583_FLD_TYPE
	{
		FTYP_UNKNOWN = 0,
		FTYP_BCD = 1,
		FTYP_ASCII = 2,
		FTYP_BINARY = 3
	};

	enum ISO8583_FLD_LEN_PREFIX_FMT
	{
		LPFT_UNKNOWN = 0,
		LPFT_BCD = 1,
		LPFT_ASCII = 2
	};

	enum ISO8583_MSG_LEN_PREFIX_FMT
	{
		LMFT_UNKNOWN = 0,
		LMFT_BINARY_LSB2 = 1,
		LMFT_BINARY_MSB2 = 2
	};

	#define MAX_ISO8583_FIELDS 193

	class T_Iso8583_Field_Description
	{
	protected:
		int Number;
		ISO8583_FLD_TYPE Type;
		int Length; // > 0 -> fixed len, -1 -> ..9, -2 -> ..99, -3 -> ..999
		ISO8583_FLD_LEN_PREFIX_FMT LenFormat;

		int GetFixedLengthInBytes() const;
		int GetPrefixLengthInBytes() const;
		int GetVariableLengthInBytes(const T_BinaryData& isomsg, int start_offset) const;
		T_BinaryData GetDirectData(const T_BinaryData& isomsg, int start_offset, int len, bool bForceToBinary) const;

		void AppendVariableLength(T_BinaryData& isomsg, int len) const;
		void AppendFixedData(T_BinaryData& isomsg, const T_BinaryData& data, bool bForceFromBinary) const;

	public:

		T_Iso8583_Field_Description(int num=-1, ISO8583_FLD_LEN_PREFIX_FMT lft=LPFT_UNKNOWN, ISO8583_FLD_TYPE typ=FTYP_UNKNOWN, int len=0)
		{
			Number = num; Type = typ; Length = len; LenFormat = lft;
		};

		bool IsValid() const
		{
			return (Type != FTYP_UNKNOWN) && (LenFormat != LPFT_UNKNOWN) && (Number >= 0) && (Number < MAX_ISO8583_FIELDS) && (Length != 0);
		};

		// A negative returned  value indicates that the message is not complete
		//
		int CheckWhereFieldEnds(const T_BinaryData& isomsg, int start_offset) const;

		T_BinaryData GetFieldData(const T_BinaryData& isomsg, int start_offset, bool bForceToBinary) const;
		void AppendFieldData(T_BinaryData& isomsg, const T_BinaryData& data, bool bForceFromBinary) const;
	};

	class T_Iso8583_Dialect
	{
	protected:
		std::array<T_Iso8583_Field_Description*, MAX_ISO8583_FIELDS> FieldDescrs;

		void Clear();

	public:
		T_Iso8583_Dialect(){};
		virtual ~T_Iso8583_Dialect();

		virtual void LoadDescriptions() = 0;

		const T_Iso8583_Field_Description* operator[](int fldNum) const
		{
			return FieldDescrs[fldNum];
		};
	};

	struct T_Iso8583_StaticFieldDescr
	{
		ISO8583_FLD_TYPE Type;
		int Length;
	};

	struct T_Iso8583_StaticDescription
	{
		ISO8583_FLD_LEN_PREFIX_FMT LengthPrefixFormat;
		int NumberOfFieldsUsed;
		T_Iso8583_StaticFieldDescr Fields[MAX_ISO8583_FIELDS];
	};

	class T_Iso8583_StaticDialect : public T_Iso8583_Dialect
	{
	protected:
		const T_Iso8583_StaticDescription* pStaticDescription;

	public:
		T_Iso8583_StaticDialect(const T_Iso8583_StaticDescription* pdescr) : T_Iso8583_Dialect()
		{
			pStaticDescription = pdescr;
			LoadDescriptions();
		};
		virtual ~T_Iso8583_StaticDialect(){};

		virtual void LoadDescriptions();
	};

	class T_Iso8583_Message
	{
	protected:
		T_Iso8583_Dialect* pDialect;
		std::array<T_BinaryData, MAX_ISO8583_FIELDS> Fields;
		bool bValid;

		int  LoadFieldsByBitmap(const T_BinaryData& isomsg,	const T_BinaryData& bitmap,	int offset,	int fldnum);

	public:
		T_Iso8583_Message(T_Iso8583_Dialect* pdialect);
		T_Iso8583_Message(T_Iso8583_Dialect* pdialect, const T_BinaryData& isomsg);

		bool CheckAndLoadMessage(const T_BinaryData& isomsg);

		bool IsValid()
		{
			return bValid;
		};


		T_BinaryData& operator[] (int fldNum)
		{
			return Fields[fldNum];
		};

		void Clear();

		T_BinaryData AssembleMessage();
	};


class T_Iso8583_Session: public IIso8583Session
{
protected:

public:

	T_Iso8583_Session(DWORD dwHostID, DWORD dwInstanceID, T_MessageHandler* pM, BYTE logLevel):
		IIso8583Session(dwHostID, dwInstanceID, pM, logLevel)
	{
	};

	virtual ~T_Iso8583_Session()
	{
	};

	virtual void HandleMessage(const T_Message& Mess) = 0;
	virtual void Idle() = 0;

};

}

//-----------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------
