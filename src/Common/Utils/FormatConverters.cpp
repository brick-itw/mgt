//-----------------------------------------------------------------------------------------------------------------------------
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Common/Utils/FormatConverters.h"

#include <string>
//-----------------------------------------------------------------------------------------------------------------------------

#define PAD    '='

const char* ZeroData = "\0\0\0\0\0\0\0\0";
const char* Digits   = "0123456789ABCDEF";

unsigned char asctobin[129] = {
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0076,0200,0200,0200,0077,
	0064,0065,0066,0067,0070,0071,0072,0073,
	0074,0075,0200,0200,0200,0200,0200,0200,
	0200,0000,0001,0002,0003,0004,0005,0006,
	0007,0010,0011,0012,0013,0014,0015,0016,
	0017,0020,0021,0022,0023,0024,0025,0026,
	0027,0030,0031,0200,0200,0200,0200,0200,
	0200,0032,0033,0034,0035,0036,0037,0040,
	0041,0042,0043,0044,0045,0046,0047,0050,
	0051,0052,0053,0054,0055,0056,0057,0060,
	0061,0062,0063,0200,0200,0200,0200,0200
};

const char* bintoasc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static void encode(unsigned char[4], unsigned char[3], int);

int IsPemChar(char c)
{
  return (strchr(bintoasc, c) != 0);
}

int ENC(int c)
{
	return ((int)bintoasc[((c) & 077)]);
}

long Abs(long val)
{
	return (val >= 0L) ? val : -val;
}

void SwapInt(int* pA, int* pB)
{
	int i;
	i   = *pA;
	*pA = *pB;
	*pB = i;
}

int StrToInt(const char* str, int cnt)
{
	int res = 0;
	for (int i = 0; i < cnt; i++)
		res = res * 10 + (str[i] - '0');
	if (res < 0)
		res = 0;
	return res;
}

char* IntToStr(unsigned long val, char* str, int cnt)
{
	memset(str, '0', cnt);
	for (int i = cnt - 1; i >= 0; i--)
	{
		str[i] = '0' + char(val % 10);
		val = val / 10;
	}
	str[cnt] = 0;
	return str;
}

int BcdToInt(unsigned char* str, int cnt)
{
	char buf[65];
	BytesToHex(str, buf, cnt);
	return StrToInt(buf, cnt * 2);
}

unsigned char* IntToBcd(int val, unsigned char* str, int cnt)
{
	char buf[65];
	IntToStr(val, buf, cnt * 2);
	HexToBytes(buf, str, cnt * 2);
	return str;
}

int MsbToInt(unsigned char* str, int cnt)
{
	unsigned res = 0;
	int i;

	for (i = 0; i < cnt; i++)
		res = (res << 8) + ((unsigned)str[i]);

	return res;
}

unsigned char* IntToMsb(int val, unsigned char* str, int cnt)
{
	int i;

	for (i = 0; i < cnt; i++)
		str[i] = (unsigned char)((val >> (8 * (cnt - i - 1))) & 0x000000FF);

	return str;
}

int LsbToInt(unsigned char* str, int cnt)
{
	unsigned res = 0;
	int i;

	for (i = cnt - 1; i >= 0; i--)
		res = (res << 8) + ((unsigned)str[i]);

	return res;
}

unsigned char* IntToLsb(int val, unsigned char* str, int cnt)
{
	int i;

	for (i = 0; i < cnt; i++)
		str[i] = (unsigned char)((val >> (8 * i)) & 0x000000FF);

	return str;
}

void BytesToHex(void* pbytes, char* phex, int count)
{
	while (count--)
	{
		*phex++ = Digits[((*((char*)pbytes)) >> 4) & 0x0F];
		*phex++ = Digits[(*((char*)pbytes)) & 0x0F];
		pbytes = ((char*)pbytes) + sizeof(char);
	}
}

int HexToBytes(const char* phex, void* pbytes, int count)
{
	const char* pa;
	while (count > 0)
	{
		*((char*)pbytes) = 0;
		pa = strchr(Digits, *phex++);
		if (pa)
			*((char*)pbytes) |= char((char(pa - Digits)) << 4);
		else
			break;
		if (!(*phex))
			break;
		pa = strchr(Digits, *phex++);
		if (pa)
			*((char*)pbytes) |= (char(pa - Digits));
		else
			break;
		pbytes = ((char*)pbytes) + sizeof(char);
		count -= 2;
	}
	return count;
}

void XorBuf(char* pSrc, char* pDest, int cnt)
{
	for (int i = 0; i < cnt; i++)
		pDest[i] ^= pSrc[i];
}

int EvenBits(unsigned char b)
{
	int bc = 0;
	for (int i = 0; i < 8; i++)
		if ((b >> i) & 0x01)
			bc++;
	return (bc & 0x01) == 0;
}

int FindBufChar(void* buf, unsigned char c, int count)
{
	for (int i = 0; i < count; i++)
		if (((unsigned char*)buf)[i] == c)
			return i;
	return AUXLIB_NOTFOUND;
}

int FindBufStr(void* buf, int count, const void* subst, int scnt)
{
	int i, k;
	if (scnt <= count)
		for (i = 0; i <= (count - scnt); i++)
		{
			for (k = 0; k < scnt; k++)
			{
				if (((unsigned char*)buf)[i + k] != ((unsigned char*)subst)[k])
					break;
			}
			if (k == scnt)
				return i;
		}
	return AUXLIB_NOTFOUND;
}

int FindOneOfChars(void* p, int count, ...)
{
	unsigned char bytes[50];
	int bcnt = 0, i, k;
	unsigned char* buf = (unsigned char*)p;
	memset(bytes,0,sizeof(bytes));
	va_list argptr;
	va_start(argptr, count);
	while ((bytes[bcnt] = (unsigned char)va_arg(argptr, unsigned long)) != 0)
		bcnt++;
	va_end(argptr);

	for (i = 0; i < count; i++)
		for (k = 0; k < bcnt; k++)
			if (buf[i] == bytes[k])
				return i;

	return AUXLIB_NOTFOUND;
}

int R_EncodePEMBlock(
                     unsigned char *encodedBlock,      /* encoded block */
                     int           *encodedBlockLen,   /* length of encoded block */
                     unsigned char *block,             /* block */
                     int            blockLen)          /* length of block */
{
	int i, lastLen;

	if (blockLen < 1)
	{
		*encodedBlockLen = 0;
		return (0);
	}

	*encodedBlockLen = 0;

	/* Bug Fixed By SK 02/09/95 */
	for (i = 0, lastLen = blockLen; i < blockLen; i += 3)
	{
		encode(&encodedBlock[4 * i / 3], &block[i], (lastLen >= 3 ? 3: lastLen));
		lastLen -= 3;
		*encodedBlockLen += 4;
	}
	return (0);
}

int R_DecodePEMBlock (
                      unsigned char *outbuf,    /* block */
                      int *outlength,           /* length of block */
                      const unsigned char *inbuf,     /* encoded block */
                      int inlength)             /* length of encoded block */
{
	unsigned char *bp;
	int length;
	unsigned int c1, c2, c3, c4;
	register int j;

	if (inlength % 4)
		return (-1);

	if (inlength < 1)
	{
		*outlength = 0;
		return (0);
	}

	length = 0;
	bp = (unsigned char *)inbuf;

	/* FOUR input characters go into each THREE output charcters */

	while (inlength > 0)
	{
		if ((*bp & 0x80) || ((c1 = asctobin[*bp]) & 0x80))
			return (-1);
		++bp;
		if ((*bp & 0x80) || ((c2 = asctobin[*bp]) & 0x80))
			return (-1);
		if (*++bp == PAD)
		{
			c3 = c4 = 0;
			length += 1;
			if (*++bp != PAD)
				return (-1);
		}
		else if ((*bp & 0x80) || ((c3 = asctobin[*bp]) & 0x80))
			return (-1);
		else
		{
			if (*++bp == PAD)
			{
				c4 = 0;
				length += 2;
			}
			else if ((*bp & 0x80) || ((c4 = asctobin[*bp]) & 0x80))
				return (-1);
			else
				length += 3;
		}
		++bp;
		j = (c1 << 2) | (c2 >> 4);
		*outbuf++ = (unsigned char)j;
		j = (c2 << 4) | (c3 >> 2);
		*outbuf++ = (unsigned char)j;
		j = (c3 << 6) | c4;
		*outbuf++ = (unsigned char)j;
		inlength -= 4;
	}

	*outlength = length;
	return (0); /* normal return */

}

static void encode(
                   unsigned char *f,               /* output buffer */
                   unsigned char *p,               /* input buffer */
                   int count)                      /* count */
{
	int c1, c2, c3, c4;

	c1 = *p >> 2;
	c2 = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
	c3 = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	c4 = p[2] & 077;
	*f++ = (unsigned char)ENC(c1);
	*f++ = (unsigned char)ENC(c2);
	if (count == 1)
	{
		*f++ = PAD;
		*f++ = PAD;
	}
	else
	{
		*f++ = (unsigned char)ENC(c3);
		if (count == 2)
			*f++ = PAD;
		else
			*f++ = (unsigned char)ENC(c4);
	}
}

char* NewString(const char* pOld, int len)
{
  if(pOld && !len)
	  len = (int)strlen(pOld);
  
	char* p = new char[len + 1];
	if (pOld)
		strcpy(p, pOld);

	p[len] = 0;
	return p;
}

void ReplaceFileNameInPath(char* path, const char* pNewFileName)
{
	char* p;

	if (!path || !pNewFileName)
		return;

	p = path + strlen(path);
	while ((p > path) && (p[-1] != '\\') && (p[-1] != '/'))
		p--;
	strcpy(p, pNewFileName);
}

char* MyStrnCpy(char* pDest, const char* pSrc, int maxLen)
{
	memset(pDest, 0, maxLen);
	strncpy(pDest, pSrc, maxLen - 1);
	return pDest;
}

unsigned long SimplePower(unsigned long x, unsigned long y)
{
	unsigned long lim=1;

	for (unsigned long i = 0; i < y; i++)
		lim *= x;

	return lim;
}

char CountLRC(const char* msg, int cnt)
{
	char c=0;
	for(int i=0;i<cnt;i++)
		c ^= msg[i];
	return c;
}

std::string FormatYYMMDDtoMMDDYY(const std::string& s)
{
	return s.substr(2, 4) + s.substr(0,2);
}

unsigned long IntFromStdString(const std::string& s)
{
	return StrToInt(s.c_str(), (int)s.length());
}

unsigned long GetDateAsDWORD(time_t t)
{
	struct tm* ptm;
	ptm = localtime(&t);

	return  (ptm->tm_year+1900) * 10000L +  /* year  */
		    (ptm->tm_mon+1)     *   100L +  /* month */
		    (ptm->tm_mday);                 /* day   */
}

unsigned long GetCurrentDateAsDWORD()
{
	return GetDateAsDWORD(time(0));
}

unsigned long GetMonthFromDateAsDWORD(unsigned long d)
{
	return (d/100)%100;
}

unsigned long GetMonthAndDayFromDateAsDWORD(unsigned long d)
{
	return d%10000;
}

std::string   ExtendStdString(const std::string& s, size_t newSize, char padding_char)
{
	if(s.length() > newSize)
		return s.substr(0, newSize);

	if(s.length() == newSize)
		return s;

	return s + std::string(newSize - s.length(), padding_char);
}

std::string IntToStdString(unsigned long val, int cnt)
{
	if(cnt <= 0)
	{
		char buf[32];
		//_itoa(val, buf, 10); //cannot find _itoa for pax
		//snprintf(buf, sizeof(buf), "%lu", val);//cannot find snprintf for Windows
		sprintf(buf, "%lu", val);
		return std::string(buf);
	}
	char* p = new char[cnt+1];
	IntToStr(val, p, cnt);
	p[cnt]=0;
	std::string s(p);
	delete[] p;
	return s;
}

T_BinaryData GetBcdDataFromString(const std::string& d)
{
	T_BinaryData BinData;
	BinData.FromAscii(d);
	return BinData;
}

std::string   TrimLeadingZeroes(const std::string& s, int MinLen)
{
	int i;
	int len = (int)s.length();
	for(i=0;((i+MinLen) < len) && (s[i] == '0');i++)
		;
	return s.substr(i, len-i);
}

std::string   PadStringWithZeroes(const std::string& s, size_t newSize)
{
	if(s.length() > newSize)
		return s.substr(0, newSize);

	if(s.length() == newSize)
		return s;

	return std::string(newSize - s.length(),'0') + s;
}


T_BinaryData  BinaryDataFromInt(unsigned long dw)
{
	unsigned char buf4[4];
	IntToMsb(dw, buf4, sizeof(buf4));
	return T_BinaryData((BYTE*)buf4, sizeof(buf4));
}

std::string GetCurrentDateAndTimeAsString()
{
	char buf[20];
	struct tm* ptm;
	time_t t = time(0);

	ptm = localtime(&t);

	sprintf(buf, "%04d%02d%02d%02d%02d%02d",
			ptm->tm_year+1900,
			ptm->tm_mon+1,
			ptm->tm_mday,
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_sec
			);
	return std::string(buf);
}

std::string TwoHexChars(BYTE b)
{
  char a[3];
  BytesToHex(&b, a, 1);
  return std::string(a, 2);
}

std::string ConvertFrom866To1251(const std::string& s)
{
  std::string s2;
  for(size_t i=0; i<s.length(); ++i)
  {
    if((BYTE(s[i]) >= 0x80) && (BYTE(s[i]) <= 0xAF))
      s2 += char(BYTE(s[i]) + 0x40);
    else
    if((BYTE(s[i]) >= 0xE0) && (BYTE(s[i]) <= 0xEF))
      s2 += char(BYTE(s[i]) + 0x10);
    else
      s2 += s[i];
  }
  return s2;
}

std::string PrintableAmount(const std::string& s, int exponent)
{
	int i;
	int len = (int)s.length();
	for(i=0;((i+exponent+1) < len) && (s[i] == '0');i++) ;
	
  std::string s2 = s.substr(i, len-i);
  if(!exponent)
    return s2;
  if(s2.length() < (DWORD)(exponent+1))
    s2.insert((size_t)0, (size_t)exponent+1-s2.length(), (char)'0');
  return s2.substr(0, s2.length()-exponent) + "." + s2.substr(s2.length()-exponent,2);
}

std::string UpperCase(const std::string& s)
{
  std::string dest;
  for(size_t i=0;i<s.size();++i)
    dest += (char)(UPPER_CASE_(s[i]));
  return dest;
}


//-----------------------------------------------------------------------------------------------------------------------------
typedef struct ConvLetter {
        unsigned char     win1251;
        unsigned short    unicode;
} Letter;


static Letter g_letters[] = {
        {0x82, 0x201A}, // SINGLE LOW-9 QUOTATION MARK
        {0x83, 0x0453}, // CYRILLIC SMALL LETTER GJE
        {0x84, 0x201E}, // DOUBLE LOW-9 QUOTATION MARK
        {0x85, 0x2026}, // HORIZONTAL ELLIPSIS
        {0x86, 0x2020}, // DAGGER
        {0x87, 0x2021}, // DOUBLE DAGGER
        {0x88, 0x20AC}, // EURO SIGN
        {0x89, 0x2030}, // PER MILLE SIGN
        {0x8A, 0x0409}, // CYRILLIC CAPITAL LETTER LJE
        {0x8B, 0x2039}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
        {0x8C, 0x040A}, // CYRILLIC CAPITAL LETTER NJE
        {0x8D, 0x040C}, // CYRILLIC CAPITAL LETTER KJE
        {0x8E, 0x040B}, // CYRILLIC CAPITAL LETTER TSHE
        {0x8F, 0x040F}, // CYRILLIC CAPITAL LETTER DZHE
        {0x90, 0x0452}, // CYRILLIC SMALL LETTER DJE
        {0x91, 0x2018}, // LEFT SINGLE QUOTATION MARK
        {0x92, 0x2019}, // RIGHT SINGLE QUOTATION MARK
        {0x93, 0x201C}, // LEFT DOUBLE QUOTATION MARK
        {0x94, 0x201D}, // RIGHT DOUBLE QUOTATION MARK
        {0x95, 0x2022}, // BULLET
        {0x96, 0x2013}, // EN DASH
        {0x97, 0x2014}, // EM DASH
        {0x99, 0x2122}, // TRADE MARK SIGN
        {0x9A, 0x0459}, // CYRILLIC SMALL LETTER LJE
        {0x9B, 0x203A}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
        {0x9C, 0x045A}, // CYRILLIC SMALL LETTER NJE
        {0x9D, 0x045C}, // CYRILLIC SMALL LETTER KJE
        {0x9E, 0x045B}, // CYRILLIC SMALL LETTER TSHE
        {0x9F, 0x045F}, // CYRILLIC SMALL LETTER DZHE
        {0xA0, 0x00A0}, // NO-BREAK SPACE
        {0xA1, 0x040E}, // CYRILLIC CAPITAL LETTER SHORT U
        {0xA2, 0x045E}, // CYRILLIC SMALL LETTER SHORT U
        {0xA3, 0x0408}, // CYRILLIC CAPITAL LETTER JE
        {0xA4, 0x00A4}, // CURRENCY SIGN
        {0xA5, 0x0490}, // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
        {0xA6, 0x00A6}, // BROKEN BAR
        {0xA7, 0x00A7}, // SECTION SIGN
        {0xA8, 0x0401}, // CYRILLIC CAPITAL LETTER IO
        {0xA9, 0x00A9}, // COPYRIGHT SIGN
        {0xAA, 0x0404}, // CYRILLIC CAPITAL LETTER UKRAINIAN IE
        {0xAB, 0x00AB}, // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
        {0xAC, 0x00AC}, // NOT SIGN
        {0xAD, 0x00AD}, // SOFT HYPHEN
        {0xAE, 0x00AE}, // REGISTERED SIGN
        {0xAF, 0x0407}, // CYRILLIC CAPITAL LETTER YI
        {0xB0, 0x00B0}, // DEGREE SIGN
        {0xB1, 0x00B1}, // PLUS-MINUS SIGN
        {0xB2, 0x0406}, // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
        {0xB3, 0x0456}, // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
        {0xB4, 0x0491}, // CYRILLIC SMALL LETTER GHE WITH UPTURN
        {0xB5, 0x00B5}, // MICRO SIGN
        {0xB6, 0x00B6}, // PILCROW SIGN
        {0xB7, 0x00B7}, // MIDDLE DOT
        {0xB8, 0x0451}, // CYRILLIC SMALL LETTER IO
        {0xB9, 0x2116}, // NUMERO SIGN
        {0xBA, 0x0454}, // CYRILLIC SMALL LETTER UKRAINIAN IE
        {0xBB, 0x00BB}, // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
        {0xBC, 0x0458}, // CYRILLIC SMALL LETTER JE
        {0xBD, 0x0405}, // CYRILLIC CAPITAL LETTER DZE
        {0xBE, 0x0455}, // CYRILLIC SMALL LETTER DZE
        {0xBF, 0x0457} // CYRILLIC SMALL LETTER YI
};

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n)
{
        int i = 0;
        int j = 0;
        for(; i < (int)n && utf8[i] != 0; ++i) {
                char prefix = utf8[i];
                char suffix = utf8[i+1];
                if ((prefix & 0x80) == 0) {
                        windows1251[j] = (char)prefix;
                        ++j;
                } else if ((~prefix) & 0x20) {
                        int first5bit = prefix & 0x1F;
                        first5bit <<= 6;
                        int sec6bit = suffix & 0x3F;
                        int unicode_char = first5bit + sec6bit;


                        if ( unicode_char >= 0x410 && unicode_char <= 0x44F ) {
                                windows1251[j] = (char)(unicode_char - 0x350);
                        } else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
                                windows1251[j] = (char)(unicode_char);
                        } else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
                                windows1251[j] = (char)(unicode_char - 0x382);
                        } else {
                                int count = sizeof(g_letters) / sizeof(Letter);
                                for (int k = 0; k < count; ++k) {
                                        if (unicode_char == g_letters[k].unicode) {
                                                windows1251[j] = g_letters[k].win1251;
                                                goto NEXT_LETTER;
                                        }
                                }
                                // can't convert this char
                                return 0;
                        }
NEXT_LETTER:
                        ++i;
                        ++j;
                } else {
                        // can't convert this chars
                        return 0;
                }
        }
        windows1251[j] = 0;
        return 1;
}

std::string convert_utf8_to_windows1251(const char* utf8, size_t n)
{
	char* buf = new char[n+1];
	memset(buf, 0, n+1);
	convert_utf8_to_windows1251(utf8, buf, n);
	std::string s(buf);
  delete[] buf;
  return s;
}

std::string convert_utf8_to_windows1251(const std::string& s)
{
  return convert_utf8_to_windows1251(s.c_str(), s.length());
}

BYTE Char866To1251(BYTE c)
{
	if((c >= 0xE0) && (c <= 0xEF))
		c += 0x10;
	if((c >= 0x80) && (c <= 0xAF))
		c += 0x40;
	return c;
}

std::string ConvertString866To1251(const std::string& s)
{
	std::string result;
	for(size_t i=0; i<s.size(); ++i)
		result.push_back(Char866To1251((BYTE)(s[i])));
	
	return result;
}


std::string GetPANFromTrack2(const std::string& track2)
{
  size_t del_pos = track2.find("D");
  if(std::string::npos == del_pos)
    return std::string("");
  return track2.substr(0, del_pos);
}

std::string GetMaskedPANFromPAN(const std::string& pan)
{
  if(pan.length() < 13)
    return std::string("");
  int len_asterisks = (int)pan.length()-4;
  std::string result = std::string(len_asterisks, '*') + pan.substr(pan.length()-4, std::string::npos);
  return result;
}

DWORD IsDottedIPaddress(const std::string& addr)
{
  DWORD a1=0, a2=0, a3=0, a4=0;
  if(4 != sscanf(addr.c_str(), "%lu.%lu.%lu.%lu", &a1, &a2, &a3, &a4))
    return 0UL;
  if((a1 > 255) || (a2 > 255) || (a3 > 255) || (a4 > 255))
    return 0UL;
  return (a1 << 24) | (a2 << 16) | (a3 << 8) | a4;
}

std::string ReplaceString(const std::string& s, const std::string& search, const std::string &replace)
{
  std::string subj(s);
  size_t pos = 0;
  while((pos = subj.find(search, pos)) != std::string::npos){
    subj.replace(pos, search.length(), replace);
    pos+=replace.length();
  }
  return subj;
}

void SwapDWORDS(unsigned long* pdw1, unsigned long* pdw2)
{
  unsigned long d = *pdw1;
  *pdw1 = *pdw2;
  *pdw2 = d;
}

