#ifndef _MY_SHA1_H_
#define _MY_SHA1_H_

//Low level functions. See Common/Crypto/my_sha1.h for high level implementation.

#define SHA_DIGESTSIZE  20

/* The structure for storing SHA info */
typedef struct
{
	unsigned long  digest[ 5 ];            /* Message digest */
	unsigned long  countLo, countHi;       /* 64-bit bit count */
	unsigned long  data[ 16 ];             /* SHA data buffer */
	unsigned short Endianness;
} SHA_CTX;


/* Message digest function */
void MakeSha1Hash(const unsigned char* p, int Len, unsigned char* TheHash);

void SHAInit  (SHA_CTX *);
void SHAUpdate(SHA_CTX *, const unsigned char* buffer, int count);
void SHAFinal (unsigned char* output, SHA_CTX *);


#endif /* end _MY_SHA_H_ */


