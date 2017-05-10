/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Declarations of elementary types
 */

#ifndef _U2_INT_TYPES_H
#define _U2_INT_TYPES_H

#ifndef __BOOLEAN__
#define __BOOLEAN__
    typedef unsigned char  BOOLEAN;
    #define TRUE  1
    #define FALSE 0
#endif


#ifndef __BYTE__
#define __BYTE__
    typedef unsigned char  BYTE;
#endif

#ifndef __WORD__
#define __WORD__
    typedef unsigned short WORD;
#endif

#ifndef __DWORD__
#define __DWORD__
    typedef unsigned long  DWORD;
#endif

enum LANGUAGE_CODE{
  L_UNDEFINED = 0,
  L_RUSSIAN   = 643,
  L_ENGLISH   = 840
};

#include <string.h>
#define _safecopy(x, y)x[0]=0;if(y)strncpy(x,y,sizeof(x));x[sizeof(x)-1]=0

#endif
