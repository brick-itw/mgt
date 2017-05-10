#include "Common/Crypto/my_des.h"

typedef unsigned char TData[65];
typedef unsigned char TBlock[33];
typedef unsigned char TKey[49];
typedef unsigned char THalfKey[29];
typedef unsigned char TTwoHalf[57];

const char ZeroVect[8] = {0,0,0,0,0,0,0,0};

unsigned char FirstP[64]=
{58,50,42,34,26,18,10, 2,
 60,52,44,36,28,20,12, 4,
 62,54,46,38,30,22,14, 6,
 64,56,48,40,32,24,16, 8,
 57,49,41,33,25,17, 9, 1,
 59,51,43,35,27,19,11, 3,
 61,53,45,37,29,21,13, 5,
 63,55,47,39,31,23,15, 7};

unsigned char EndP[64]=
{40, 8,48,16,56,24,64,32,
 39, 7,47,15,55,23,63,31,
 38, 6,46,14,54,22,62,30,
 37, 5,45,13,53,21,61,29,
 36, 4,44,12,52,20,60,28,
 35, 3,43,11,51,19,59,27,
 34, 2,42,10,50,18,58,26,
 33, 1,41, 9,49,17,57,25};

unsigned char STable[8][4][16]={
{{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
 { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
 { 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
 {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}   },

{{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
 { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
 { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
 {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}   },

{{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
 {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
 {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
 { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}   },

{{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
 {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
 {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
 { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}   },

{{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
 {14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6},
 { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
 {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}   },

{{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
 {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8},
 { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
 { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}   },

{{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
 {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6},
 { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
 { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}   },

{{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
 { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
 { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
 { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}   }   };

unsigned char Extention[48]=
{32, 1, 2, 3, 4, 5,
  4, 5, 6, 7 ,8, 9,
  8, 9,10,11,12,13,
 12,13,14,15,16,17,
 16,17,18,19,20,21,
 20,21,22,23,24,25,
 24,25,26,27,28,29,
 28,29,30,31,32, 1};

unsigned char PPP[32]={
16, 7 ,20,21,
29,12,28,17,
 1,15,23,26,
 5,18,31,10,
 2, 8,24,14,
32,27, 3, 9,
19,13,30, 6,
22,11, 4, 25};

unsigned char BBB[56]=
{57,49,41,33,25,17, 9,
  1,58,50,42,34,26,18,
 10, 2,59,51,43,35,27,
 19,11, 3,60,52,44,36,
 63,55,47,39,31,23,15,
  7,62,54,46,38,30,22,
 14, 6,61,53,45,37,29,
 21,13, 5,28,20,12, 4};

unsigned char Sd[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
unsigned char KPod[48]=
{14,17,11,24, 1, 5,
  3,28,15, 6,21,10,
 23,19,12, 4,26, 8,
 16, 7,27,20,13, 2,
 41,52,31,37,47,55,
 30,40,51,45,33,48,
 44,49,39,56,34,53,
 46,42,50,36,29,32};

void FirstPer(TData D)
{
 int i;
 TData NewData;

 for(i=0;i<64;i++)
	NewData[i]=D[FirstP[i]-1];

 for(i=0;i<64;i++)
	D[i]=NewData[i];
}

void EndPer(TData D)
{
 TData NewData;
 int i;

 for(i=0;i<64;i++)
   NewData[i]=D[EndP[i]-1];
 for(i=0;i<64;i++)
   D[i]=NewData[i];
}

char Ss(int num,unsigned char* p)
{
  int col=(p[1]<<3)^(p[2]<<2)^(p[3]<<1)^p[4];
  int stroka=(p[0]<<1)^p[5];
  return STable[num-1][stroka][col];
}

void Extan(TBlock R,TKey E)
{
 for(int i=0;i<48;i++)
   E[i] = R[Extention[i]-1];
}

void P(TBlock D)
{
  TBlock Dnew;
  int i;
  for(i=0;i<32;i++)
    Dnew[i]=D[PPP[i]-1];
  for(i=0;i<32;i++)
    D[i]=Dnew[i];
}

void B(TData K, THalfKey C, THalfKey D)
{
  int i;
  for(i=0;i<28;i++)
    C[i]=K[BBB[i]-1];
  for(i=0;i<28;i++)
    D[i]=K[BBB[i+28]-1];
}

void Sdvig(int Takt,THalfKey C, THalfKey D,TKey Ki)
{
 int i,k;
 THalfKey Cnew,Dnew;
 TTwoHalf CD;

 for(i=0;i<28;i++) {
   k=(i+Sd[Takt])%28;
   Cnew[i]=C[k];
   Dnew[i]=D[k];
 }

 for(i=0;i<28;i++) {
   D[i]=Dnew[i];
   C[i]=Cnew[i];
 }

 for(i=0;i<28;i++)
   CD[i]=C[i];
 for(i=0;i<28;i++)
   CD[i+28]=D[i];
 for(i=0;i<48;i++)
   Ki[i]=CD[KPod[i]-1];
}

void f(TBlock L, TBlock R, TKey Ki,TBlock Rnew)
{
  TKey ER;
  TKey ER_K;
  int i,j,k,n;
  TBlock Out;

  Extan(R,ER);
  for(i=0;i<48;i++)
    ER_K[i] = ER[i] ^ Ki[i];

  n = 0;
  for(j=0;j<8;j++) {
    k = Ss(j+1,ER_K+(j*6));
    for(i=3;i>=0;i--)
      Out[n++] = (unsigned char)((k >> i) & 0x01);
  }

  P(Out);

  for(i=0;i<32;i++)
    Rnew[i] = Out[i] ^ L[i];
}

void BinToBitmap(char *S, TData D, int len)
{
  int i,k,n=0;
  for(i=0;i<len;i++)
    for(k=7;k>=0;k--)
      D[n++] = (unsigned char)((S[i] >> k) & 0x01);
}

void BitmapToBin(TData D, char *S, int len)
{
  int i;
  for(i=0;i<(len>>3);i++)
	 S[i]=0;
  for(i=0;i<len;i++)
	 S[i>>3] |= char(D[i] << (7-(i & 0x0007)));
}

void DirectDES(const void* Ds, const void* Ks, void* res)
{
  int i,j,n;
  TData Data, Key;
  TBlock L, R, Lnew, Rnew;
  TKey Ki;
  THalfKey C, D;

  BinToBitmap((char *)Ds,Data,8);
  BinToBitmap((char *)Ks,Key,8);

  B(Key,C,D);
  FirstPer(Data);

  for(i=0;i<32;i++) {
    L[i]=Data[i];
    R[i]=Data[i+32];
  }

  for(n=0;n<16;n++)  {
    for(j=0;j<32;j++)
      Lnew[j]=R[j];
    Sdvig(n,C,D,Ki);
    f(L,R,Ki,Rnew);
    for(j=0;j<32;j++) {
      R[j]=Rnew[j];
      L[j]=Lnew[j];
    }
  }

  for(i=0;i<32;i++) {
    Data[i+32]=L[i];
    Data[i]=R[i];
  }

  EndPer(Data);

  BitmapToBin(Data,(char*)res,64);
}

void ReverseDES(const void* Ds, const void* Ks, void* res)
{
  int i,n;
  TData Data, Key;
  TBlock L,R,Lnew,Rnew;
  TKey Ki;
  THalfKey C,D;
  char KiN[16][48];

  BinToBitmap((char *)Ds,Data,8);
  BinToBitmap((char *)Ks,Key, 8);

  B(Key,C,D);
  FirstPer(Data);

  for(i=0;i<32;i++)
    R[i]=Data[i];
  for(i=0;i<32;i++)
    L[i]=Data[i+32];

  for(n=0;n<16;n++) {
    Sdvig(n,C,D,Ki);
    for(i=0;i<48;i++)
      KiN[15-n][i]=Ki[i];
  }

  for(n=0;n<16;n++) {
    for(i=0;i<48;i++)
      Ki[i]=KiN[n][i];
    f(R,L,Ki,Lnew);
    for(i=0;i<32;i++)
      Rnew[i]=L[i];
    for(i=0;i<32;i++)
      L[i]=Lnew[i];
    for(i=0;i<32;i++)
      R[i]=Rnew[i];
  }

  for(i=0;i<32;i++)
    Data[i+32]=R[i];
  for(i=0;i<32;i++)
    Data[i]=L[i];

  EndPer(Data);

  BitmapToBin(Data,(char*)res,64);
}

