//=============================================================================================================================
// * Name      : iso8583main.cpp
// * Author    : Alexey Brik
//=============================================================================================================================
#include "Common/Iso8583/iso8583main.h"
#include "Common/Utils/FormatConverters.h"

namespace ump
{

//-----------------------------------------------------------------------------------------------------------------------------

const bool cBcdFields[MAX_ISO8583_FIELDS]=
{
true,  /* msg type         fld  0   */

true,  /* primary bitmap   fld  1   */

true,  /* PAN              fld  2   */
true,  /* Processing code  fld  3   */
true,  /* Amount           fld  4   */
false, /*                  fld  5   */
false, /*                  fld  6   */
false, /*                  fld  7   */
false, /*                  fld  8   */
false, /*                  fld  9   */
false, /*                  fld 10   */

false, /*                  fld 11   */
false, /*                  fld 12   */
false, /*                  fld 13   */
false, /*                  fld 14   */
false, /*                  fld 15   */
false, /*                  fld 16   */
false, /*                  fld 17   */
false, /*                  fld 18   */
false, /*                  fld 19   */
false, /*                  fld 20   */

false, /*                  fld 21   */
false, /*                  fld 22   */
false, /*                  fld 23   */
false, /*                  fld 24   */
false, /*                  fld 25   */
false, /*                  fld 26   */
false, /*                  fld 27   */
false, /*                  fld 28   */
false, /*                  fld 29   */
false, /*                  fld 30   */

false, /*                  fld 31   */
false, /*                  fld 32   */
false, /*                  fld 33   */
false, /*                  fld 34   */
true,  /* Track2           fld 35   */
false, /*                  fld 36   */
false, /* RRN              fld 37   */
false, /* Auth code        fld 38   */
false, /* Response code    fld 39   */
false, /*                  fld 40   */

false, /* Terminal ID      fld 41   */
false, /* Merchant ID      fld 42   */
false, /*                  fld 43   */
false, /*                  fld 44   */
false, /*                  fld 45   */
false, /*                  fld 46   */
false, /*                  fld 47   */
false, /*                  fld 48   */
false, /*                  fld 49   */
false, /*                  fld 50   */

false, /*                  fld 51   */
true,  /* PIN block        fld 52   */
false, /*                  fld 53   */
false, /*                  fld 54   */
true,  /* ICC data         fld 55   */
false, /*                  fld 56   */
false, /*                  fld 57   */
false, /*                  fld 58   */
false, /*                  fld 59   */
false, /*                  fld 60   */

false, /*                  fld 61   */
false, /*                  fld 62   */
false, /*                  fld 63   */
true,  /* MAC              fld 64   */

true,  /* Secondary bitmap fld 65   */

false, /*                  fld 66   */
false, /*                  fld 67   */
false, /*                  fld 68   */
false, /*                  fld 69   */
false, /*                  fld 70   */

false, /*                  fld 71   */
false, /*                  fld 72   */
false, /*                  fld 73   */
false, /*                  fld 74   */
false, /*                  fld 75   */
false, /*                  fld 16   */
false, /*                  fld717   */
false, /*                  fld 78   */
false, /*                  fld 79   */
false, /*                  fld 80   */

false, /*                  fld 91   */
false, /*                  fld 92   */
false, /*                  fld 93   */
false, /*                  fld 94   */
false, /*                  fld 95   */
false, /*                  fld 96   */
false, /*                  fld 97   */
false, /*                  fld 98   */
false, /*                  fld 99   */
false, /*                  fld 100  */

false, /*                  fld 101  */
false, /*                  fld 102  */
false, /*                  fld 103  */
false, /*                  fld 104  */
false, /*                  fld 105  */
false, /*                  fld 106  */
false, /*                  fld 107  */
false, /*                  fld 108  */
false, /*                  fld 109  */
false, /*                  fld 110  */

false, /*                  fld 121  */
false, /*                  fld 122  */
false, /*                  fld 123  */
false, /*                  fld 124  */
false, /*                  fld 125  */
false, /*                  fld 126  */
false, /*                  fld 127  */
true,  /* MAC              fld 128  */

true,  /* Third bitmap     fld 129  */

false, /*                  fld 130  */

false, /*                  fld 131  */
false, /*                  fld 132  */
false, /*                  fld 133  */
false, /*                  fld 134  */
false, /*                  fld 135  */
false, /*                  fld 136  */
false, /*                  fld 137  */
false, /*                  fld 138  */
false, /*                  fld 139  */
false, /*                  fld 140  */

false, /*                  fld 141  */
false, /*                  fld 142  */
false, /*                  fld 143  */
false, /*                  fld 144  */
false, /*                  fld 145  */
false, /*                  fld 146  */
false, /*                  fld 147  */
false, /*                  fld 148  */
false, /*                  fld 149  */
false, /*                  fld 150  */

false, /*                  fld 151  */
false, /*                  fld 152  */
false, /*                  fld 153  */
false, /*                  fld 154  */
false, /*                  fld 155  */
false, /*                  fld 156  */
false, /*                  fld 157  */
false, /*                  fld 158  */
false, /*                  fld 159  */
false, /*                  fld 160  */

false, /*                  fld 161  */
false, /*                  fld 162  */
false, /*                  fld 163  */
false, /*                  fld 164  */
false, /*                  fld 165  */
false, /*                  fld 166  */
false, /*                  fld 167  */
false, /*                  fld 168  */
false, /*                  fld 169  */
false, /*                  fld 170  */

false, /*                  fld 171  */
false, /*                  fld 172  */
false, /*                  fld 173  */
false, /*                  fld 174  */
false, /*                  fld 175  */
false, /*                  fld 176  */
false, /*                  fld 177  */
false, /*                  fld 178  */
false, /*                  fld 179  */
false, /*                  fld 180  */

false, /*                  fld 181  */
false, /*                  fld 182  */
false, /*                  fld 183  */
false, /*                  fld 184  */
false, /*                  fld 185  */
false, /*                  fld 186  */
false, /*                  fld 187  */
false, /*                  fld 188  */
false, /*                  fld 189  */
false, /*                  fld 190  */

false, /*                  fld 191  */

true   /* MAC              fld 192  */
};

int T_Iso8583_Field_Description::GetPrefixLengthInBytes() const
{
	switch(Length)
	{
	case -1: return 1;
	case -2: return (LenFormat == LPFT_BCD) ? 1 : 2;
	case -3: return (LenFormat == LPFT_BCD) ? 2 : 3;
	default: return -1;
	}

}

int T_Iso8583_Field_Description::GetFixedLengthInBytes() const
{
	switch(Type)
	{
	case FTYP_BCD:    return Length/2;
	case FTYP_ASCII:  return Length;
	case FTYP_BINARY: return Length;
	default:          return -1;
	}
}

int T_Iso8583_Field_Description::GetVariableLengthInBytes(const T_BinaryData& isomsg, int start_offset) const
{
	if(LenFormat == LPFT_ASCII)
		return isomsg.ToInt((DWORD)start_offset, (DWORD)(-Length));

	switch(Length)
	{
	case -1: return isomsg[start_offset];
	case -2: return IntFromStdString(isomsg.Copy(start_offset, 1).ToAscii());
	case -3: return IntFromStdString(isomsg.Copy(start_offset, 2).ToAscii());
	default: return -1;
	}
}

void T_Iso8583_Field_Description::AppendVariableLength(T_BinaryData& isomsg, int len) const
{
	std::string SLen = IntToStdString(len, -Length);
	if(LenFormat == LPFT_ASCII)
	{
		isomsg += T_BinaryData(SLen);
	}
	else
	switch(Length)
	{
	case -1:
		isomsg.AddDirectBytes((BYTE)len, 1);
		break;
	case -2:
		isomsg += GetBcdDataFromString(SLen);
		break;
	case -3:
		isomsg += GetBcdDataFromString(std::string("0") + SLen);
		break;
	}
}

void T_Iso8583_Field_Description::AppendFixedData(T_BinaryData& isomsg, const T_BinaryData& data, bool bForceFromBinary) const
{
	T_BinaryData d;

	if(bForceFromBinary && (Type == FTYP_ASCII)){
		std::string tail;
		if(data.Length() % 1)
			tail = "F";
		d.FromAscii(data.ToString(0, data.Length()) + tail);
	}
	else
		d = data;

	if(d.Length() > (DWORD)Length)
		d = d.Copy(0, d.Length() - Length);

	if(d.Length() < (DWORD)Length)
		d.AddDirectBytes(((Type == FTYP_ASCII) && !bForceFromBinary) ? ' ' : 0x00, Length - d.Length());

	isomsg += d;
}

void T_Iso8583_Field_Description::AppendFieldData(T_BinaryData& isomsg, const T_BinaryData& data, bool bForceFromBinary) const
{
	if(Length < 0)
		AppendVariableLength(isomsg, data.Length());
	AppendFixedData(isomsg, data, bForceFromBinary);
}

int T_Iso8583_Field_Description::CheckWhereFieldEnds(const T_BinaryData& isomsg, int start_offset) const
{
	if(!IsValid())
		return -1;

	if(Length > 0)
	{
		int target_len = GetFixedLengthInBytes();
		if(target_len < 0)
			return -1;
		target_len += start_offset;
		return (target_len <= (int)isomsg.Length()) ?  target_len : -1;
	}
	else
	{
		int prefix_len = GetPrefixLengthInBytes();
		if(prefix_len < 0)
			return -1;
		prefix_len += start_offset;
		int var_len = GetVariableLengthInBytes(isomsg, start_offset);
		if(var_len < 0)
			return -1;
		var_len += prefix_len;
		return (var_len <= (int)isomsg.Length()) ?  var_len : -1;
	}
}

T_BinaryData  T_Iso8583_Field_Description::GetDirectData(const T_BinaryData& isomsg, int start_offset, int len, bool bForceToBinary) const
{
	if(Type == FTYP_BCD)
      len /= 2;

	T_BinaryData d = isomsg.Copy((DWORD)start_offset, (DWORD)len);
	if((!bForceToBinary) || (Type != FTYP_ASCII))
		return d;

	if(len % 1)
	{
		d = d + T_BinaryData((BYTE*)"F", 1);
		len++;
	}

	return d.AsciiToBinary();
}

T_BinaryData  T_Iso8583_Field_Description::GetFieldData(const T_BinaryData& isomsg, int start_offset, bool bForceToBinary) const
{
	if(CheckWhereFieldEnds(isomsg, start_offset)  < 0)
		return T_BinaryData();

	if(Length > 0)
		return GetDirectData(isomsg, start_offset, Length, bForceToBinary);

	int prefix_len = GetPrefixLengthInBytes();
	int var_len = GetVariableLengthInBytes(isomsg, start_offset);
	return GetDirectData(isomsg, start_offset + prefix_len, var_len, bForceToBinary);
}

T_Iso8583_Message::T_Iso8583_Message(T_Iso8583_Dialect* pdialect)
{
	pDialect = pdialect;
	bValid = false;
}

T_Iso8583_Message::T_Iso8583_Message(T_Iso8583_Dialect* pdialect, const T_BinaryData& isomsg)
{
	pDialect = pdialect;
	bValid = CheckAndLoadMessage(isomsg);
}


int T_Iso8583_Message::LoadFieldsByBitmap(const T_BinaryData& isomsg,
		const T_BinaryData& bitmap,
		int offset,
		int fldnum
		)
{
	int limit = fldnum + 63, next_offset;

	Fields[fldnum-1] = bitmap;

	for(; (offset < (int)isomsg.Length()) && (fldnum < limit); fldnum++)
	{
		if(IsBitPresent(bitmap, (fldnum-1)%64))
		{
			next_offset = ((*pDialect)[fldnum])->CheckWhereFieldEnds(isomsg, offset);
			if(next_offset < 0)
				return -1;
			Fields[fldnum] = (*pDialect)[fldnum]->GetFieldData(isomsg, offset, cBcdFields[fldnum]);
			offset = next_offset;
		}
	}
	return offset;
}

void T_Iso8583_Message::Clear()
{
	T_BinaryData emptyField;
	Fields.fill(emptyField);
}

bool T_Iso8583_Message::CheckAndLoadMessage(const T_BinaryData& isomsg)
{
	if(!pDialect)
		return false;

	int offset = ((*pDialect)[0])->CheckWhereFieldEnds(isomsg, 0);
	if(offset < 0)
		return false;

	Clear();
	Fields[0] = (*pDialect)[0]->GetFieldData(isomsg, 0, true);

	for(int fldnum = 2; (offset > 0) && (offset < (int)isomsg.Length() && (fldnum < MAX_ISO8583_FIELDS)); fldnum += 64)
	{
		int next_offset = ((*pDialect)[fldnum-1])->CheckWhereFieldEnds(isomsg, offset);
		if(next_offset < 0)
			return false;

		T_BinaryData bitmap = ((*pDialect)[fldnum-1])->GetFieldData(isomsg, offset, true);
		if(bitmap.Length() != 8)
			return false;

		offset = LoadFieldsByBitmap(isomsg, bitmap, next_offset, fldnum);
		if(offset < 0)
			return false;

		if(!IsBitPresent(bitmap, 0))
			break;
	}
	return (offset == (int)isomsg.Length()) ? true : false;
}

T_BinaryData T_Iso8583_Message::AssembleMessage()
{
	T_BinaryData d;

	if(Fields[0].Length())
	{
		(*pDialect)[0]->AppendFieldData(d, Fields[0], true);

		int i;
		T_BinaryData bitmap;

		for(i=2; i<MAX_ISO8583_FIELDS; i++)
			if(Fields[i].Length())
				SetBitInMask(bitmap, i-1);

		if(bitmap.Length() >= 24)
		{
			SetBitInMask(bitmap, 64);
			Fields[129] = bitmap.Copy(16, 8);
		}

		if(bitmap.Length() >= 16)
		{
			SetBitInMask(bitmap, 0);
			Fields[65] = bitmap.Copy(8, 8);
		}

		Fields[1] = bitmap.Copy(0, 8);
		int limit = bitmap.Length()*8;

		for(i=2; i<=limit; i++)
			if(Fields[i].Length())
				(*pDialect)[i]->AppendFieldData(d, Fields[i], cBcdFields[i]);
	}

	return d;
}

void T_Iso8583_Dialect::Clear()
{
	for(int i=0; i<MAX_ISO8583_FIELDS; i++)
		if(FieldDescrs[i])
		{
			delete FieldDescrs[i];
			FieldDescrs[i] = 0;
		}
}

T_Iso8583_Dialect::~T_Iso8583_Dialect()
{
	Clear();
}

void T_Iso8583_StaticDialect::LoadDescriptions()
{
	Clear();

	for(int i=0; i<MAX_ISO8583_FIELDS; i++)
	{
		FieldDescrs[i] = new T_Iso8583_Field_Description(i,
				pStaticDescription->LengthPrefixFormat,
				(i <= pStaticDescription->NumberOfFieldsUsed) ? pStaticDescription->Fields[i].Type   : FTYP_UNKNOWN,
				(i <= pStaticDescription->NumberOfFieldsUsed) ? pStaticDescription->Fields[i].Length : 0             );
	}
}


}

