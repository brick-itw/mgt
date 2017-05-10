#ifndef _PC_FILE_H
#define _PC_FILE_H

#include "Base/Types/inttypes.h"
#include "Platforms/Interfaces/IFileSystem.h"
#include "Platforms/Interfaces/IFile.h"
#include <stdio.h>

#define MAX_PC_FILE_NAME_LEN  260

class TPCFile : public IFileWritable{
protected:
   FILE* fptr;
   char  FileName[MAX_PC_FILE_NAME_LEN];

   TPCFile(const char* name, FILE* f)
   {
     _safecopy(FileName, name);
     fptr = f;
   };

public:

  virtual ~TPCFile()
  {
    fclose(fptr);
  };

  virtual DWORD Read        (BYTE* pDest,      DWORD maxLen);
  virtual DWORD Write       (const BYTE* pSrc, DWORD maxLen);
  virtual void  Seek        (DWORD offset);
  virtual DWORD Tell        ();
  virtual DWORD GetLength   ();

  friend class TPCFileSystem;
};



class TPCFileSystem : public IFileSystem{
protected:
  char       Directory[MAX_PC_FILE_NAME_LEN];
  Error      LastError;

  virtual FILE*  OpenFilePointer(const char* fname, WORD mode);
  virtual char*  MakeFullPath(char* pDest, const char* fname);
  TPCFile*       OpenFileWithMode(const char* fname, WORD mode);

public:
  TPCFileSystem(const char* path) : LastError(SUCCESS)
  {
    _safecopy(Directory, path);
  };

  virtual ~TPCFileSystem()
  {
  };  

  virtual Error FlushCacheToDisk()
  {
    return SUCCESS;
  };


  virtual Error EraseFile  (const char* fileName);
  virtual Error RenameFile (const char* oldName, const char* newName);

  virtual IFileReadable* OpenForRead(const char* fname);
  virtual IFileWritable* OpenForWrite(const char* fname, WORD mode);

  virtual Error GetLastError()
  {
    return LastError;
  };
};


#endif
