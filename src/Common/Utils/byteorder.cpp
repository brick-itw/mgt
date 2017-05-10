#include "Common/Utils/byteorder.h"

unsigned short LSB_TO_WORD(const unsigned char* p)
{
	return (((unsigned short)p[1]) << 8) + (unsigned short)p[0];
}

unsigned char* WORD_TO_LSB(unsigned short w, unsigned char* p)
{
	p[1] = (unsigned char)(w >> 8);
	p[0] = (unsigned char)(w);
	return p;
}

unsigned long LSB_TO_DWORD(const unsigned char* p)
{
	return (((unsigned long)p[3]) << 24) + (((unsigned long)p[2]) << 16) + (((unsigned long)p[1]) << 8) + (unsigned long)p[0];
}

unsigned char* DWORD_TO_LSB(unsigned long dw, unsigned char* p)
{
	p[3] = (unsigned char)(dw >> 24);
	p[2] = (unsigned char)(dw >> 16);
	p[1] = (unsigned char)(dw >> 8);
	p[0] = (unsigned char)(dw);
	return p;
}

unsigned short MSB_TO_WORD(const unsigned char* p)
{
	return (((unsigned short)p[0]) << 8) + (unsigned short)p[1];
}

unsigned char* WORD_TO_MSB(unsigned short w, unsigned char* p)
{
	p[0] = (unsigned char)(w >> 8);
	p[1] = (unsigned char)(w);
	return p;
}

unsigned long MSB_TO_DWORD(const unsigned char* p)
{
	return (((unsigned long)p[0]) << 24) + (((unsigned long)p[1]) << 16) + (((unsigned long)p[2]) << 8) + (unsigned long)p[3];
}

unsigned char* DWORD_TO_MSB(unsigned long dw, unsigned char* p)
{
	p[0] = (unsigned char)(dw >> 24);
	p[1] = (unsigned char)(dw >> 16);
	p[2] = (unsigned char)(dw >> 8);
	p[3] = (unsigned char)(dw);
	return p;
}

//-----------------------------------------------------------------------------------------------------------------------------

