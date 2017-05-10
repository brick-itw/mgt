#ifndef FILESYSTEM_DEVICE_H
#define FILESYSTEM_DEVICE_H

//#include "TFile.h"

//#include "base.h"
#include "Base/Types/inttypes.h"

#include "Base/Types/error.h"
#include "Platforms/Interfaces/IFile.h"
#include "Platforms/Interfaces/IFileSystem.h"

#define MAX_FILENAME_LEN  128

#define DRV_FO_READ      0x0001
#define DRV_FO_WRITE     0x0002
#define DRV_FO_RDWR      0x0003
#define DRV_FO_NCRP      0x0004
#define DRV_FO_URGENT    0x0008

typedef struct{
  char   FileName[MAX_FILENAME_LEN];
  void*  pData;
  long   DataLen;
  long   Offset;
  int    IsDirty;
  int    IsUrgent;
  int    IsClosed;
  void*  pNext;
}T_Cached_File_Data;


class TFileSystem : public IFileSystem {
public:
  TFileSystem();
  virtual ~TFileSystem(){};  // must dismount file system

  virtual Error FlushCacheToDisk    ();
  virtual Error EraseFile           (const char* fileName);
  virtual Error RenameFile          (const char* oldName, const char* newName);
  virtual Error GetLastError();
  virtual long OpenFile(const char* fname, WORD mode);
  virtual void CloseFile(long fptr);
  virtual DWORD WriteFile(long fptr, void* pBuf, DWORD sz);
  virtual DWORD ReadFile(long fptr, void* pBuf, DWORD sz);
  virtual DWORD FileGetLength(long fptr);
  virtual DWORD GetFreeSpaceInKbytes();
  virtual DWORD FileGetPos(long fptr);
  
private:
  Error last_error;
  BYTE FileFlashSelected;
  long  CacheTimer;
  T_Cached_File_Data* pCache;
  
  virtual BYTE IsFlashFileName(const char* fName);
  virtual BYTE FileSelectFlash (BYTE bSelect);
  virtual BYTE ValidHandle(int iHandle);
  virtual long ReadCachedFile(T_Cached_File_Data* pNew, void* pDest, long Count);
  virtual long WriteCachedFile(T_Cached_File_Data* pNew, void* pSrc, long Count);
  virtual long GetCachedFileLength(T_Cached_File_Data* pNew);
  virtual void RenameCacheEntry(const char* fOldName, const char* fNewName);
  virtual void RearmCacheTimer(void);
  virtual void FlushCachedFile(T_Cached_File_Data* pNew);
  virtual void LoadCacheEntry(T_Cached_File_Data* pNew);
  virtual void DeleteCacheEntry(T_Cached_File_Data* pNew);
  virtual char* trueFileNamePtr(char* fname);
  virtual T_Cached_File_Data* FindCacheEntry(const char* fName);
  virtual T_Cached_File_Data* OpenCachedFile(const char* fName, WORD mode);
  virtual T_Cached_File_Data* NewCacheEntry(const char* fName);
  virtual T_Cached_File_Data* AppendCacheEntry(const char* fName);
  
};

#endif
