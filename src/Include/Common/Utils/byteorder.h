#ifndef _U2_BYTE_ORD_H
#define _U2_BYTE_ORD_H

//these functions differs from similar functions from UPOS1.0, be careful
//for example, DWORD_TO_LSB is not the same as old DWORD_LSB

unsigned short LSB_TO_WORD (const unsigned char* p);
unsigned char* WORD_TO_LSB (unsigned short w, unsigned char* p);

unsigned long  LSB_TO_DWORD(const unsigned char* p);
unsigned char* DWORD_TO_LSB(unsigned long dw, unsigned char* p);

unsigned short MSB_TO_WORD (const unsigned char* p);
unsigned char* WORD_TO_MSB (unsigned short w, unsigned char* p);

unsigned long  MSB_TO_DWORD(const unsigned char* p);
unsigned char* DWORD_TO_MSB(unsigned long dw, unsigned char* p);

#endif


