/**
 * @Author aakhartov.sbt@sberbank.ru
 * @date   August 2016
 * @brief  File interface declaration
 */

#ifndef _U2_FILE_H
#define _U2_FILE_H

#include "Base/Types/types.h"
#include "Platforms/Interfaces/IDevice.h"

class IFileReadable {
public:
  virtual ~IFileReadable() {};        // must close the file

  virtual DWORD Read      (BYTE* pDest,      DWORD maxLen) = 0;
  virtual void  Seek      (DWORD offset) = 0;
  virtual DWORD Tell      () = 0;
  virtual DWORD GetLength () = 0;
};

class IFileWritable : public IFileReadable {
public:
  virtual DWORD Write     (const BYTE* pSrc, DWORD maxLen) = 0;
};
#endif
