/* sha1.c : Implementation of the Secure Hash Algorithm */

/* SHA: NIST's Secure Hash Algorithm */

/*	This version written November 2000 by David Ireland of 
	DI Management Services Pty Limited <code@di-mgt.com.au>

	Adapted from code in the Python Cryptography Toolkit, 
	version 1.0.0 by A.M. Kuchling 1995.
*/

/* AM Kuchling's posting:- 
   Based on SHA code originally posted to sci.crypt by Peter Gutmann
   in message <30ajo5$oe8@ccu2.auckland.ac.nz>.
   Modified to test for endianness on creation of SHA objects by AMK.
   Also, the original specification of SHA was found to have a weakness
   by NSA/NIST.  This code implements the fixed version of SHA.
*/


#include <stdio.h>
#include <string.h>

#include "Common/Crypto/my_sha1.h"

/* POINTER defines a generic pointer type */
#if !defined(POINTER)
  typedef unsigned char *POINTER;
#endif

/* UINT4 defines a four byte word */
#ifndef __UINT4__
  #define __UINT4__
  typedef unsigned long int UINT4; 
#endif

/* BYTE defines a unsigned character */
#ifndef __BYTE__
  #define __BYTE__
  typedef unsigned char BYTE;
#endif

#ifndef TRUE
  #define FALSE	0
  #define TRUE	( !FALSE )
#endif /* TRUE */


void SHAInit(SHA_CTX *);
void SHAUpdate(SHA_CTX *, BYTE *buffer, int count);
void SHAFinal(BYTE *output, SHA_CTX *);
void endianTest(unsigned short *endianness);

static void SHAtoByte(BYTE *output, UINT4 *input, unsigned int len);

/* The SHA block size and message digest sizes, in bytes */

#define SHA_DATASIZE    64

/* The SHA f()-functions.  The f1 and f3 functions can be optimized to
   save one boolean operation each - thanks to Rich Schroeppel,
   rcs@cs.arizona.edu for discovering this */

#define f1(x,y,z)   ( z ^ ( x & ( y ^ z ) ) )           /* Rounds  0-19 */
#define f2(x,y,z)   ( x ^ y ^ z )                       /* Rounds 20-39 */
#define f3(x,y,z)   ( ( x & y ) | ( z & ( x | y ) ) )   /* Rounds 40-59 */
#define f4(x,y,z)   ( x ^ y ^ z )                       /* Rounds 60-79 */

typedef UINT4 (*T_F_Func)(UINT4 x, UINT4 y, UINT4 z);

UINT4 _F1(UINT4 x, UINT4 y, UINT4 z)
{
  return z ^ ( x & ( y ^ z ) );
}

UINT4 _F2(UINT4 x, UINT4 y, UINT4 z)
{
  return z ^ x ^ y ;
}

UINT4 _F3(UINT4 x, UINT4 y, UINT4 z)
{
  return ( x & y ) | ( z & ( x | y ) );
}


/* The SHA Mysterious Constants */

const UINT4 _K[4] = {
0x5A827999L ,                                /* Rounds  0-19 */
0x6ED9EBA1L ,                                /* Rounds 20-39 */
0x8F1BBCDCL ,                                /* Rounds 40-59 */
0xCA62C1D6L                                 /* Rounds 60-79 */
};

/* SHA initial values */

const UINT4 _hInit[5] = {
0x67452301L,
0xEFCDAB89L,
0x98BADCFEL,
0x10325476L,
0xC3D2E1F0L
};

/* Note that it may be necessary to add parentheses to these macros if they
   are to be called with expressions as arguments */
/* 32-bit rotate left - kludged with shifts */

#define ROTL(n,X)  ( ( ( X ) << n ) | ( ( X ) >> ( 32 - n ) ) )

/* The initial expanding function.  The hash function is defined over an
   80-UINT2 expanded input array W, where the first 16 are copies of the input
   data, and the remaining 64 are defined by

        W[ i ] = W[ i - 16 ] ^ W[ i - 14 ] ^ W[ i - 8 ] ^ W[ i - 3 ]

   This implementation generates these values on the fly in a circular
   buffer - thanks to Colin Plumb, colin@nyx10.cs.du.edu for this
   optimization.

   The updated SHA changes the expanding function by adding a rotate of 1
   bit.  Thanks to Jim Gillogly, jim@rand.org, and an anonymous contributor
   for this information */

UINT4 expand(UINT4* W, int i)
{ 
  if(i > 15)
    W[ i & 15 ] = ROTL( 1, ( W[ i & 15 ] ^ W[ (i - 14) & 15 ] ^ 
                       W[ (i - 8) & 15 ] ^ W[ (i - 3) & 15 ] ) )  ;
  return W[ i & 15 ];
}


/* The prototype SHA sub-round.  The fundamental sub-round is:

        a' = e + ROTL( 5, a ) + f( b, c, d ) + k + data;
        b' = a;
        c' = ROTL( 30, b );
        d' = c;
        e' = d;

   but this is implemented by unrolling the loop 5 times and renaming the
   variables ( e, a, b, c, d ) = ( a', b', c', d', e' ) each iteration.
   This code is then replicated 20 times for each of the 4 functions, using
   the next 20 values from the W[] array each time */

void subRound(UINT4* abcde, int n, T_F_Func f, UINT4 k, UINT4 data)
{
   abcde[(9-n)%5] += ROTL( 5, abcde[(5-n)%5] ) +
        f( abcde[(6-n)%5], abcde[(7-n)%5], abcde[(8-n)%5] ) + k + data;

   abcde[(6-n)%5] = ROTL( 30, abcde[(6-n)%5] );
}

/* Initialize the SHA values */

void SHAInit(SHA_CTX *shsInfo)
{
    int i;
    endianTest(&shsInfo->Endianness);

    /* Set the h-vars to their initial values */
    for(i=0;i<5;i++)
      shsInfo->digest[ i ] = _hInit[i];

    /* Initialise bit count */
    shsInfo->countLo = shsInfo->countHi = 0;
}


/* Perform the SHA transformation.  Note that this code, like MD5, seems to
   break some optimizing compilers due to the complexity of the expressions
   and the size of the basic block.  It may be necessary to split it into
   sections, e.g. based on the four subrounds

   Note that this corrupts the shsInfo->data area */

void SHATransform(UINT4 *digest, UINT4 *data)
    {
    UINT4 eData[ 16 ];       /* Expanded data */
    UINT4 abcde[  5 ];
    T_F_Func _F[  4 ];
    int i,k;

    _F[0] = (T_F_Func)_F1;
    _F[1] = _F[3] = (T_F_Func)_F2;
    _F[2] = (T_F_Func)_F3;

    for(i=0;i<5;i++)
      abcde[i] = digest[i];

    memcpy( (POINTER)eData, (POINTER)data, SHA_DATASIZE );

    for(k=0;k<4;k++)
      for(i=0;i<20;i++)
        subRound( abcde, i%5, _F[k], _K[k], expand( eData, 20*k+i ) );

    for(i=0;i<5;i++)
      digest[i] += abcde[i];
 }

/* When run on a little-endian CPU we need to perform byte reversal on an
   array of long words. */

static void longReverse(UINT4 *buffer, int byteCount, unsigned short Endianness )
{
    UINT4 value;

    if (Endianness) return;
    byteCount /= sizeof( UINT4 );
    while( byteCount-- )
        {
        value = *buffer;
        value = ( ( value & 0xFF00FF00L ) >> 8  ) |
                ( ( value & 0x00FF00FFL ) << 8 );
        *buffer++ = ( value << 16 ) | ( value >> 16 );
        }
}

/* Update SHA for a block of data */

void SHAUpdate(SHA_CTX *shsInfo, const unsigned char *buffer, int count)
{
    UINT4 tmp;
    int dataCount;

    /* Update bitcount */
    tmp = shsInfo->countLo;
    if ( ( shsInfo->countLo = tmp + ( ( UINT4 ) count << 3 ) ) < tmp )
        shsInfo->countHi++;             /* Carry from low to high */
    shsInfo->countHi += count >> 29;

    /* Get count of bytes already in data */
    dataCount = ( int ) ( tmp >> 3 ) & 0x3F;

    /* Handle any leading odd-sized chunks */
    if( dataCount )
        {
        BYTE *p = ( BYTE * ) shsInfo->data + dataCount;

        dataCount = SHA_DATASIZE - dataCount;
        if( count < dataCount )
            {
            memcpy( p, buffer, count );
            return;
            }
        memcpy( p, buffer, dataCount );
        longReverse( shsInfo->data, SHA_DATASIZE, shsInfo->Endianness);
        SHATransform( shsInfo->digest, shsInfo->data );
        buffer += dataCount;
        count -= dataCount;
        }

    /* Process data in SHA_DATASIZE chunks */
    while( count >= SHA_DATASIZE )
        {
        memcpy( (POINTER)shsInfo->data, (POINTER)buffer, SHA_DATASIZE );
        longReverse( shsInfo->data, SHA_DATASIZE, shsInfo->Endianness );
        SHATransform( shsInfo->digest, shsInfo->data );
        buffer += SHA_DATASIZE;
        count -= SHA_DATASIZE;
        }

    /* Handle any remaining bytes of data. */
    memcpy( (POINTER)shsInfo->data, (POINTER)buffer, count );
}

/* Final wrapup - pad to SHA_DATASIZE-byte boundary with the bit pattern
   1 0* (64-bit count of bits processed, MSB-first) */

void SHAFinal(unsigned char *output, SHA_CTX *shsInfo)
{
    int count;
    BYTE *dataPtr;

    /* Compute number of bytes mod 64 */
    count = ( int ) shsInfo->countLo;
    count = ( count >> 3 ) & 0x3F;

    /* Set the first char of padding to 0x80.  This is safe since there is
       always at least one byte free */
    dataPtr = ( BYTE * ) shsInfo->data + count;
    *dataPtr++ = 0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = SHA_DATASIZE - 1 - count;

    /* Pad out to 56 mod 64 */
    if( count < 8 )
        {
        /* Two lots of padding:  Pad the first block to 64 bytes */
        memset( dataPtr, 0, count );
        longReverse( shsInfo->data, SHA_DATASIZE, shsInfo->Endianness );
        SHATransform( shsInfo->digest, shsInfo->data );

        /* Now fill the next block with 56 bytes */
        memset( (POINTER)shsInfo->data, 0, SHA_DATASIZE - 8 );
        }
    else
        /* Pad block to 56 bytes */
        memset( dataPtr, 0, count - 8 );

    /* Append length in bits and transform */
    shsInfo->data[ 14 ] = shsInfo->countHi;
    shsInfo->data[ 15 ] = shsInfo->countLo;

    longReverse( shsInfo->data, SHA_DATASIZE - 8, shsInfo->Endianness );
    SHATransform( shsInfo->digest, shsInfo->data );

	/* Output to an array of bytes */
	SHAtoByte(output, shsInfo->digest, SHA_DIGESTSIZE);

	/* Zeroise sensitive stuff */
	memset((POINTER)shsInfo, 0, sizeof(shsInfo));
}

static void SHAtoByte(BYTE *output, UINT4 *input, unsigned int len)
{	/* Output SHA digest in byte array */
	unsigned int i, j;

	for(i = 0, j = 0; j < len; i++, j += 4)
	{
        output[j+3] = (BYTE)( input[i]        & 0xff);
        output[j+2] = (BYTE)((input[i] >> 8 ) & 0xff);
        output[j+1] = (BYTE)((input[i] >> 16) & 0xff);
        output[j  ] = (BYTE)((input[i] >> 24) & 0xff);
	}
}

void endianTest(unsigned short *endian_ness)
{
	if((*(unsigned short *) ("#S") >> 8) == '#')
	{
		/* printf("Big endian = no change\n"); */
		*endian_ness = !(0);
	}
	else
	{
		/* printf("Little endian = swap\n"); */
		*endian_ness = 0;
	}
}

void MakeSha1Hash(const unsigned char* p, int Len, unsigned char* TheHash)
{
  SHA_CTX ctx;
  SHAInit   (&ctx);
  SHAUpdate (&ctx, p, Len);
  SHAFinal  (TheHash,&ctx);
}
 
