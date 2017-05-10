/*
	RSA.C - RSA routines for RSAEURO

	Copyright (c) J.S.A.Kapp 1994 - 1995.
                      
	RSAEURO - RSA Library compatible with RSAREF(tm) 2.0.

	All functions prototypes are the Same as for RSAREF(tm).
	To aid compatiblity the source and the files follow the
	same naming comventions that RSAREF(tm) uses.  This should aid
	direct importing to your applications.

	This library is legal everywhere outside the US.  And should
	NOT be imported to the US and used there.

	All Trademarks Acknowledged.

	RSA encryption performed as defined in the PKCS (#1) by RSADSI.

	Revision history
		0.90 First revision, code produced very similar to that
		of RSAREF(tm), still it worked fine.

		0.91 Current revision, code altered to aid speeding up.
		Used pointer accesses to arrays to speed up some parts,
		mainly during the loops.
*/

#include "Common/Crypto/my_rsa.h"
#include "Common/Crypto/nn.h"

void R_memset(unsigned char*, int, unsigned int);
void R_memcpy(unsigned char*, unsigned char*, unsigned int);
int  R_memcmp(unsigned char*, unsigned char*, unsigned int);
/* Raw RSA public-key operation. Output has same length as modulus.

	 Requires input < modulus.
*/
static int rsapublicfunc(
  unsigned char *output,          /* output block */
  unsigned int *outputLen,        /* length of output block */
  const unsigned char *input,           /* input block */
  unsigned int inputLen,          /* length of input block */
  RRSA_PUBLIC_KEY *publicKey     /* RSA public key */
)
{
	NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],
		n[MAX_NN_DIGITS];
	unsigned int eDigits, nDigits;


		/* decode the required RSA function input data */
	NN_Decode(m, MAX_NN_DIGITS, input, inputLen);
	NN_Decode(n, MAX_NN_DIGITS, publicKey->modulus, MAX__RSA_MODULUS_LEN);
	NN_Decode(e, MAX_NN_DIGITS, publicKey->exponent, MAX__RSA_MODULUS_LEN);

	nDigits = NN_Digits(n, MAX_NN_DIGITS);
	eDigits = NN_Digits(e, MAX_NN_DIGITS);


	if(NN_Cmp(m, n, nDigits) >= 0)
		return(RE_DATA);
                

	*outputLen = (publicKey->bits + 7) / 8;

	/* Compute c = m^e mod n.  To perform actual RSA calc.*/

	NN_ModExp (c, m, e, eDigits, n, nDigits);

	/* encode output to standard form */
	NN_Encode (output, *outputLen, c, nDigits);

	/* Clear sensitive information. */

	R_memset((unsigned char*)c, 0, sizeof(c));
	R_memset((unsigned char*)m, 0, sizeof(m));

	return(ID_OK);
}

/* RSA decryption, according to RSADSI's PKCS #1. */
int RRSAPublicDecrypt (unsigned char* output, unsigned int* outputLen,
                      const unsigned char* input, unsigned int inputLen,
                      RRSA_PUBLIC_KEY* publicKey)
{
	int status;
	unsigned int  modulusLen;

	modulusLen = (publicKey->bits + 7) / 8;

	if(inputLen > modulusLen)
		return(RE_LEN);

	status = rsapublicfunc(output, outputLen, input, inputLen, publicKey);
	if(status)
		return(status);

	if(*outputLen != modulusLen)
		return(RE_LEN);

	return(ID_OK);
}

                            
/* Raw RSA private-key operation. Output has same length as modulus.

	 Requires input < modulus.
*/
static int rsaprivatefunc(
  unsigned char *output,          /* output block */
  unsigned int *outputLen,        /* length of output block */
  unsigned char *input,           /* input block */
  unsigned int inputLen,          /* length of input block */
  RRSA_PRIVATE_KEY *privateKey   /* RSA private key */
)
{
	NN_DIGIT c[MAX_NN_DIGITS], cP[MAX_NN_DIGITS], cQ[MAX_NN_DIGITS],
		dP[MAX_NN_DIGITS], dQ[MAX_NN_DIGITS], mP[MAX_NN_DIGITS],
		mQ[MAX_NN_DIGITS], n[MAX_NN_DIGITS], p[MAX_NN_DIGITS], q[MAX_NN_DIGITS],
		qInv[MAX_NN_DIGITS], t[MAX_NN_DIGITS];
	unsigned int cDigits, nDigits, pDigits;

	/* decode required input data from standard form */
	NN_Decode(c, MAX_NN_DIGITS, input, inputLen);           /* input */

					/* private key data */
	NN_Decode(p, MAX_NN_DIGITS, privateKey->prime[0], MAX__RSA_PRIME_LEN);
	NN_Decode(q, MAX_NN_DIGITS, privateKey->prime[1], MAX__RSA_PRIME_LEN);
	NN_Decode(dP, MAX_NN_DIGITS, privateKey->primeExponent[0], MAX__RSA_PRIME_LEN);
	NN_Decode(dQ, MAX_NN_DIGITS, privateKey->primeExponent[1], MAX__RSA_PRIME_LEN);
	NN_Decode(n, MAX_NN_DIGITS, privateKey->modulus, MAX__RSA_MODULUS_LEN);
	NN_Decode(qInv, MAX_NN_DIGITS, privateKey->coefficient, MAX__RSA_PRIME_LEN);
		/* work out lengths of input components */

    cDigits = NN_Digits(c, MAX_NN_DIGITS);
    pDigits = NN_Digits(p, MAX_NN_DIGITS);
    nDigits = NN_Digits(n, MAX_NN_DIGITS);


	if(NN_Cmp(c, n, nDigits) >= 0)
		return(RE_DATA);

	*outputLen = (privateKey->bits + 7) / 8;

	/* Compute mP = cP^dP mod p  and  mQ = cQ^dQ mod q. (Assumes q has
		 length at most pDigits, i.e., p > q.)
	*/

	NN_Mod(cP, c, cDigits, p, pDigits);
	NN_Mod(cQ, c, cDigits, q, pDigits);

	NN_AssignZero(mP, nDigits);
	NN_ModExp(mP, cP, dP, pDigits, p, pDigits);

	NN_AssignZero(mQ, nDigits);
	NN_ModExp(mQ, cQ, dQ, pDigits, q, pDigits);

	/* Chinese Remainder Theorem:
			m = ((((mP - mQ) mod p) * qInv) mod p) * q + mQ.
	*/

	if(NN_Cmp(mP, mQ, pDigits) >= 0) {
		NN_Sub(t, mP, mQ, pDigits);
	}else{
		NN_Sub(t, mQ, mP, pDigits);
		NN_Sub(t, p, t, pDigits);
	}

	NN_ModMult(t, t, qInv, p, pDigits);
	NN_Mult(t, t, q, pDigits);
	NN_Add(t, t, mQ, nDigits);

	/* encode output to standard form */
	NN_Encode (output, *outputLen, t, nDigits);

	return(ID_OK);
}

int RRSAPrivateEncrypt(
  unsigned char *output,          /* output block */
  unsigned int *outputLen,        /* length of output block */
  unsigned char *input,           /* input block */
  unsigned int inputLen,          /* length of input block */
  RRSA_PRIVATE_KEY *privateKey   /* RSA private key */
)
{
	int status;
	unsigned int  modulusLen;
	modulusLen = (privateKey->bits + 7) / 8;
	status = rsaprivatefunc(output, outputLen, input/*pkcsBlock*/, modulusLen, privateKey);
	return(status);
}


void R_memset(unsigned char* output, int value, unsigned int len)
{
	if(len != 0) {
		do {
			*output++ = (unsigned char)value;
		}while(--len != 0);
	}
}

// Secure memcpy routine 

void R_memcpy(unsigned char* output, unsigned char* input, unsigned int len)
{
	if (len != 0) {
		do {
			*output++ = *input++;
		}while (--len != 0);
	}
}

/* Secure memcmp routine */

int R_memcmp(unsigned char* Block1, unsigned char* Block2, unsigned int len)
{
	if(len != 0) {
		/* little trick in declaring vars */
		register const unsigned char *p1 = Block1, *p2 = Block2;

		do {
			if(*p1++ != *p2++)
				return(*--p1 - *--p2);
		}while(--len != 0);
	}
	return(0);
}


