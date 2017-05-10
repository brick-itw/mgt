#include "Platforms/Implementations/Pc/TPCFile.h"
#include "Base/Types/errno_error.h"
#include <stdio.h>
#ifndef WIN32
  #include <unistd.h>
  #define _unlink  unlink
  #define _stricmp strcasecmp
#endif

char* TPCFileSystem::MakeFullPath(char* pDest, const char* fname)
{
  if(!Directory[0])
    sprintf(pDest, "%s", fname);
  else
    sprintf(pDest, "%s/%s", Directory, fname);

  return pDest;
}

FILE* TPCFileSystem::OpenFilePointer(const char* fname, WORD mode)
{
  char fullName[MAX_PC_FILE_NAME_LEN];
  const char* fmode;
  FILE* fptr;

  if(mode & FO_APPEND)
    fmode = (mode & FO_READ) ? "a+b" : "ab";
  else
  if(mode & (FO_TRUNC|FO_CREATE))
    fmode = "wb";
  else
  if(mode & FO_WRITE)
    fmode = "r+b";
  else
    fmode = "rb";

  fptr = fopen(MakeFullPath(fullName, fname), fmode);
  if((!fptr) && ((mode & FO_WRITE) != 0))
    fptr = fopen(fullName, (mode & FO_READ) ? "w+b" : "wb");

  return fptr;
}


TPCFile* TPCFileSystem::OpenFileWithMode(const char* fname, WORD mode)
{
  FILE* fptr = OpenFilePointer(fname, mode);

  if(!fptr){
    LastError = ErrnoError(ERROR_FILE_OPEN_FAILURE);
    return NULL;
  }

  return new TPCFile(fname, fptr);
}

IFileReadable* TPCFileSystem::OpenForRead(const char* fname)
{
  return OpenFileWithMode(fname, FO_READ);
}

IFileWritable* TPCFileSystem::OpenForWrite(const char* fname, WORD mode)
{
  return OpenFileWithMode(fname, mode | FO_WRITE);
}

Error TPCFileSystem::EraseFile(const char* fileName)
{
  char fullName[MAX_PC_FILE_NAME_LEN];

  MakeFullPath(fullName, fileName);

  return (LastError = ErrnoError(((_unlink(fullName) == -1) && (errno != ENOENT)) ?
                                  ERROR_FILE_ERASE_FAILURE : SUCCESS));
}

Error TPCFileSystem::RenameFile(const char* oldName, const char* newName)
{
  char fullNameOld[MAX_PC_FILE_NAME_LEN];
  char fullNameNew[MAX_PC_FILE_NAME_LEN];

  if(!_stricmp(oldName, newName))
    return (LastError = SUCCESS);

  MakeFullPath(fullNameOld, oldName);
  MakeFullPath(fullNameNew, newName);

  _unlink(fullNameNew);
  return (LastError = ErrnoError((rename(fullNameOld, fullNameNew) == -1) ?
                         ERROR_FILE_RENAME_FAILURE : SUCCESS));
}


DWORD TPCFile::Read(BYTE* pDest, DWORD maxLen)
{
  return (DWORD)fread(pDest, 1, maxLen, fptr);
}


DWORD TPCFile::Write(const BYTE* pSrc, DWORD maxLen)
{
  return (DWORD)fwrite((void*)pSrc, 1, maxLen, fptr);
}


void TPCFile::Seek(DWORD offset)
{
  fseek(fptr, offset, SEEK_SET);
}


DWORD TPCFile::Tell()
{
  return ftell(fptr);
}

DWORD TPCFile::GetLength()
{
  DWORD pos, sz;
  pos = ftell(fptr);
  fseek(fptr, 0, SEEK_END);
  sz = ftell(fptr);
  fseek(fptr, pos, SEEK_SET);
  return sz;
}
                                 


