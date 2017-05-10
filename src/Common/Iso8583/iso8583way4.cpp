//=============================================================================================================================
// * Name      : iso8583way4.cpp
// * Author    : Alexey Brik
// * Created on: 31 дек. 2015 г.
//=============================================================================================================================

#include "Common/Iso8583/iso8583way4.h"
#include "Common/Tags/emv_tags.h"
#include "Common/Utils/byteorder.h"
#include "Base/Types/error.h"
#include "Common/Utils/FormatConverters.h"

namespace ump
{

#define W4_REQUEST_TIMEOUT_SEC    10
#define W4_HEADER_TIMEOUT_SEC      3
#define W4_MAC_TIMEOUT_SEC         6
#define W4_RESP_TIMEOUT_SEC        8
#define W4_DB_TIMEOUT_SEC         10

#define W4_REQ_TIMEOUT_SEC        60
#define W4_REPLY_TIMEOUT_SEC      60


T_Iso8583_StaticDescription cWay4DialectDescr =
{
	LPFT_BCD,
	64,
	{
			{FTYP_BCD,      4},   //  0, MSG type

			{FTYP_BINARY,   8},   //  1, bitmap
			{FTYP_BCD,     -2},   //  2, PAN
			{FTYP_BCD,      6},   //  3, processing code
			{FTYP_BCD,     12},   //  4, amount
			{FTYP_BCD,     12},   //  5
			{FTYP_BCD,     12},   //  6
			{FTYP_BCD,     10},   //  7, STAN
			{FTYP_BCD,      8},   //  8
			{FTYP_BCD,      8},   //  9
			{FTYP_BCD,      8},   // 10

			{FTYP_BCD,      6},   // 11
			{FTYP_BCD,      6},   // 12
			{FTYP_BCD,      4},   // 13
			{FTYP_BCD,      4},   // 14
			{FTYP_BCD,      6},   // 15
			{FTYP_BCD,      4},   // 16
			{FTYP_BCD,      4},   // 17
			{FTYP_BCD,      4},   // 18
			{FTYP_BCD,      3},   // 19
			{FTYP_BCD,      3},   // 20

			{FTYP_BCD,      3},   // 21
			{FTYP_BCD,      3},   // 22, POS entry mode
			{FTYP_BCD,      3},   // 23, PAN sequence number
			{FTYP_BCD,      3},   // 24, function code
			{FTYP_BCD,      2},   // 25, condition code
			{FTYP_BCD,      4},   // 26
			{FTYP_BCD,      1},   // 27
			{FTYP_ASCII,    9},   // 28
			{FTYP_BCD,      3},   // 29
			{FTYP_BCD,     24},   // 30

			{FTYP_ASCII,   -2},   // 31
			{FTYP_BCD,     -2},   // 32
			{FTYP_BCD,     -2},   // 33
			{FTYP_BCD,     -2},   // 34
			{FTYP_ASCII,   -2},   // 35, Track2
			{FTYP_BCD,     -3},   // 36
			{FTYP_ASCII,   12},   // 37, RRN
			{FTYP_ASCII,    6},   // 38, AuthCode
			{FTYP_ASCII,    2},   // 39, ReplyCode
			{FTYP_BCD,      3},   // 40

			{FTYP_ASCII,    8},   // 41, TerminalID
			{FTYP_ASCII,   15},   // 42, MerchantID
			{FTYP_ASCII,   40},   // 43
			{FTYP_ASCII,   -2},   // 44
			{FTYP_ASCII,   -2},   // 45
			{FTYP_ASCII,   -3},   // 46
			{FTYP_ASCII,   -3},   // 47
			{FTYP_ASCII,   -3},   // 48
			{FTYP_BCD,      3},   // 49, Currency code
			{FTYP_BCD,      3},   // 50

			{FTYP_BINARY,   8},   // 52, PIN block
			{FTYP_BINARY,   8},   // 53, aux. PIN info
			{FTYP_BINARY,  -3},   // 54
			{FTYP_BINARY,  -3},   // 55, ICC data
			{FTYP_BCD,     -2},   // 56
			{FTYP_BCD,      3},   // 57
			{FTYP_ASCII,   -2},   // 58
			{FTYP_ASCII,   -3},   // 59
			{FTYP_BINARY,  -3},   // 60

			{FTYP_BINARY,  -3},   // 61
			{FTYP_BINARY,  -3},   // 62
			{FTYP_BINARY,  -3},   // 63
			{FTYP_BINARY,   4}    // 64, MAC
	}
};

std::string cNoRespCode          = "99";
std::string cDefaultRespCode     = "96";
std::string cWrongMacRespCode    = "98";
std::string cFormatErrRespCode   = "96";
std::string cTermUnknownRespCode = "03";


T_Way4Session::T_Way4Session(DWORD dwHostID, DWORD dwInstanceID, T_MessageHandler* pM, BYTE logLevel):
	T_Iso8583_Session(dwHostID, dwInstanceID, pM, logLevel), Dialect(&cWay4DialectDescr), RequestMsg(&Dialect), ReplyMsg(&Dialect)
{
	Init();
}


}
