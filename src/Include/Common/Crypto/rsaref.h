/*
	RSAREF.H - header file for RSAEURO cryptographic toolkit

	Copyright (c) J.S.A.Kapp 1994 - 1995.

	RSAEURO - RSA Library compatible with RSAREF(tm) 2.0.

	All functions prototypes are the Same as for RSAREF(tm).
	To aid compatiblity the source and the files follow the
	same naming comventions that RSAREF(tm) uses.  This should aid
	direct importing to your applications.

	This library is legal everywhere outside the US.  And should
	NOT be imported to the US and used there.

	All Trademarks Acknowledged.

	This header file is used to launch the RSAEURO.H header file.
	Provided for compatiblity with RSAREF(tm).

	Revision 1.00 - JSAK 23/6/95, Final Release Version
*/

#ifndef _RSAREF_H_
#define _RSAREF_H_


/* RSA key lengths. */

#define MIN__RSA_MODULUS_BITS 508
/* 
	 PGP 2.6.2 Now allows 2048-bit keys changing below will allow this.
     It does lengthen key generation slightly if the value is increased.
*/


#define MAX__RSA_MODULUS_BITS 2048
#define MAX__RSA_MODULUS_LEN ((MAX__RSA_MODULUS_BITS + 7) / 8)
#define MAX__RSA_PRIME_BITS ((MAX__RSA_MODULUS_BITS + 1) / 2)
#define MAX__RSA_PRIME_LEN ((MAX__RSA_PRIME_BITS + 7) / 8)

/* Maximum lengths of encoded and encrypted content, as a function of
	 content length len. Also, inverse functions. */

#define ENCODED_CONTENT_LEN(len) (4*(len)/3 + 3)
#define ENCRYPTED_CONTENT_LEN(len) ENCODED_CONTENT_LEN ((len)+8)
#define DECODED_CONTENT_LEN(len) (3*(len)/4 + 1)
#define DECRYPTED_CONTENT_LEN(len) (DECODED_CONTENT_LEN (len) - 1)

/* Maximum lengths of signatures, encrypted keys, encrypted
	 signatures, and message digests. */

#define MAX_SIGNATURE_LEN MAX__RSA_MODULUS_LEN
#define MAX_PEM_SIGNATURE_LEN ENCODED_CONTENT_LEN(MAX_SIGNATURE_LEN)
#define MAX_ENCRYPTED_KEY_LEN MAX__RSA_MODULUS_LEN
#define MAX_PEM_ENCRYPTED_KEY_LEN ENCODED_CONTENT_LEN(MAX_ENCRYPTED_KEY_LEN)
#define MAX_PEM_ENCRYPTED_SIGNATURE_LEN ENCRYPTED_CONTENT_LEN(MAX_SIGNATURE_LEN)
#define MAX_DIGEST_LEN 20

/* Maximum length of Diffie-Hellman parameters. */

#define DH_PRIME_LEN(bits) (((bits) + 7) / 8)

/* Error codes. */

#define RE_CONTENT_ENCODING 0x0400
#define RE_DATA 0x0401
#define RE_DIGEST_ALGORITHM 0x0402
#define RE_ENCODING 0x0403
#define RE_KEY 0x0404
#define RE_KEY_ENCODING 0x0405
#define RE_LEN 0x0406
#define RE_MODULUS_LEN 0x0407
#define RE_NEED_RANDOM 0x0408
#define RE_PRIVATE_KEY 0x0409
#define RE_PUBLIC_KEY 0x040a
#define RE_SIGNATURE 0x040b
#define RE_SIGNATURE_ENCODING 0x040c
#define RE_ENCRYPTION_ALGORITHM 0x040d

/* Library details. */

#define RSAEURO_VER_MAJ 1
#define RSAEURO_VER_MIN 02
#define RSAEURO_IDENT "RSAEURO"
#define RSAEURO_DATE "21/08/94"

/* Internal Error Codes */

#define ID_OK    0
#define ID_ERROR 1

/* Internal defs. */

#ifndef TRUE
  #define TRUE    1
#endif

#ifndef FALSE
  #define FALSE   0
#endif

/* Random structure. */

typedef struct {
  unsigned int bytesNeeded;                    /* seed bytes required */
  unsigned char state[16];                     /* state of object */
  unsigned int outputAvailable;                /* number byte available */
  unsigned char output[16];                    /* output bytes */
} RRANDOM_STRUCT;

/* RSA public and private key. */

typedef struct {
  unsigned short int bits;                     /* length in bits of modulus */
  unsigned char modulus[MAX__RSA_MODULUS_LEN];  /* modulus */
  unsigned char exponent[MAX__RSA_MODULUS_LEN]; /* public exponent */
} RRSA_PUBLIC_KEY;

typedef struct {
  unsigned short int bits;                     /* length in bits of modulus */
  unsigned char modulus[MAX__RSA_MODULUS_LEN];  /* modulus */
  unsigned char publicExponent[MAX__RSA_MODULUS_LEN];     /* public exponent */
  unsigned char exponent[MAX__RSA_MODULUS_LEN]; /* private exponent */
  unsigned char prime[2][MAX__RSA_PRIME_LEN];   /* prime factors */
  unsigned char primeExponent[2][MAX__RSA_PRIME_LEN];     /* exponents for CRT */
  unsigned char coefficient[MAX__RSA_PRIME_LEN];          /* CRT coefficient */
} RRSA_PRIVATE_KEY;

typedef struct {
  unsigned char *prime;                        /* prime */
  unsigned int primeLen;                       /* length of prime */
  unsigned char *generator;                    /* generator */
  unsigned int generatorLen;                   /* length of generator */
} RDH_PARAMS;


#endif /* _RSAREF_H_ */
