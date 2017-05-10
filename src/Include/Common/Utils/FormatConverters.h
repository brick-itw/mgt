#ifndef _AUX_LIB
#define _AUX_LIB
//-----------------------------------------------------------------------------------------------------------------------------

#include "Common/Utils/T_BinaryData.h"
#include <time.h>

#include <string>


#define AUXLIB_NOTFOUND (-1)
#define AUXLIB_PENDING  (-2)

//pbytes - pointer to input bytes
//phex - pointer to output hex string (function will not allocate mem!). Function will not add zero at end of string.
//count - amount of input bytes
void BytesToHex(void* pbytes,char* phex,int count);

//phex - pointer to hex string, for example "01AA"
//pbytes - pointer to resulting buffer (function will not allocate mem!). Expected result in buffer: [0x01, 0xAA]
//count - amount of input bytes
int  HexToBytes(const char* phex,void* pbytes,int count);

void XorBuf(char* pSrc, char* pDest, int cnt);
int  EvenBits(unsigned char b);
char CountLRC(const char* msg, int cnt);

long Abs(long val);
void SwapInt(int* pA, int* pB);

char* NewString(const char* pOld, int len);
char* MyStrnCpy(char* pDest, const char* pSrc, int maxLen);

unsigned long SimplePower(unsigned long x, unsigned long y);

int  StrToInt(const char* str, int cnt);
int  LsbToInt(unsigned char* str, int cnt);
int  MsbToInt(unsigned char* str, int cnt);
int  BcdToInt(unsigned char* str, int cnt);

char*  IntToStr(unsigned long val, char* str, int cnt);
unsigned char*  IntToBcd(int val, unsigned char* str, int cnt);
unsigned char*  IntToLsb(int val, unsigned char* str, int cnt);
unsigned char*  IntToMsb(int val, unsigned char* str, int cnt);

inline bool IS_DIGIT_ (unsigned char c)
{ return ((c>='0') && (c<='9')); };

inline bool IS_HEX_(unsigned char c)
{ return ((c>='0') && (c<='9')) || ((c>='A') && (c<='F')) || ((c>='a') && (c<='f'));};

inline bool IS_ALPHA_(unsigned char c)
{ return ((c>='0') && (c<='9')) || ((c>='A') && (c<='Z')) || ((c>='a') && (c<='z'));};

inline unsigned char UPPER_CASE_(unsigned char c)
{ return ((c>='a') && (c<='z')) ? (c-0x20) : c; };


int  FindBufChar(void* buf, unsigned char c, int count);
int  FindBufStr (void* buf, int count, const void* subst, int scnt);
int  FindOneOfChars(void* p, int count, ...);

int R_EncodePEMBlock(
                     unsigned char *encodedBlock,    /* encoded block */
                     int *encodedBlockLen,           /* length of encoded block */
                     unsigned char *block,           /* block */
                     int blockLen                    /* length of block */
                    );

int R_DecodePEMBlock(
                     unsigned char *outbuf,          /* block */
                     int *outlength,                 /* length of block */
                     const unsigned char *inbuf,           /* encoded block */
                     int inlength                    /* length of encoded block */
                    );

void ReplaceFileNameInPath(char* path, const char* pNewFileName);

std::string FormatYYMMDDtoMMDDYY(const std::string& s);
unsigned long IntFromStdString (const std::string& s);
std::string   IntToStdString(unsigned long val, int cnt=0);
T_BinaryData  GetBcdDataFromString(const std::string& d);
T_BinaryData  BinaryDataFromInt(unsigned long dw);
std::string   TwoHexChars(BYTE b);
std::string   UpperCase(const std::string& s);
unsigned long IsDottedIPaddress(const std::string& s);

unsigned long GetDateAsDWORD(time_t t);

/*
 * returns YYYYMMDD as unsigned long
 */
unsigned long GetCurrentDateAsDWORD();
unsigned long GetMonthFromDateAsDWORD(unsigned long d);
unsigned long GetMonthAndDayFromDateAsDWORD(unsigned long d);
std::string GetCurrentDateAndTimeAsString();

std::string   TrimLeadingZeroes(const std::string& s, int MinLen);
std::string   ExtendStdString(const std::string& s, size_t newSize, char padding_char = ' ');
std::string   PadStringWithZeroes(const std::string& s, size_t newSize);

std::string   ConvertFrom866To1251(const std::string& s);
std::string   PrintableAmount(const std::string& s, int exponent);

int         convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n);
std::string convert_utf8_to_windows1251(const char* utf8, size_t n);
std::string convert_utf8_to_windows1251(const std::string& s);

std::string ConvertString866To1251(const std::string& s);

std::string GetPANFromTrack2(const std::string& track2);
std::string GetMaskedPANFromPAN(const std::string& pan);

std::string ReplaceString(const std::string& s, const std::string& search, const std::string &replace);

void SwapDWORDS(unsigned long* pdw1, unsigned long* pdw2);
//-----------------------------------------------------------------------------------------------------------------------------


#endif
