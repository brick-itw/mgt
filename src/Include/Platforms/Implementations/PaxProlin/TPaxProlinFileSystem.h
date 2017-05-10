#ifndef _PAX_FILE_SYSTEM_H
#define _PAX_FILE_SYSTEM_H

#include "Platforms/Implementations/PC/lin/TLinFileSystem.h"

class TPaxProlinFileSystem : public TLinFileSystem{
protected:
  virtual FILE*    OpenFilePointer(const char* fname, WORD mode);
  virtual char*    MakeFullPath(char* pDest, const char* fname);

public:
  TPaxProlinFileSystem(const char* path) : TLinFileSystem(path) {};
  ~TPaxProlinFileSystem(){};
};


#endif
