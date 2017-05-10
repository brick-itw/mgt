#ifndef _LIN_PC_FILE_H
#define _LIN_PC_FILE_H

#include "Platforms/Implementations/PC/TPCFile.h"

class TLinFileSystem :  public TPCFileSystem{
public:
  TLinFileSystem(const char* path) : TPCFileSystem(path) {};
  ~TLinFileSystem(){};

  virtual DWORD GetFreeSpaceInKbytes();
  virtual DWORD GetFileLength(const char* fileName);
  
};

#endif
