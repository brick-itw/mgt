#ifndef _WIN_PC_FILE_H
#define _WIN_PC_FILE_H

#include "Platforms/Implementations/PC/TPCFile.h"

class TWinFileSystem :  public TPCFileSystem{
public:
  TWinFileSystem(const char* path) : TPCFileSystem(path) {};
  ~TWinFileSystem(){};

  virtual DWORD GetFreeSpaceInKbytes();
  virtual DWORD GetFileLength(const char* fileName);
  
};

#endif
