
#include "Platforms/Implementations/Verifon/TFilesystem.h"
  #include <ctype.h>
  #include <stdarg.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <cardslot.h>
  #include <vos_ddi_ext_err.h>
  #include <svc_sec.h>
  
#define TMM_CACHE_FLUSH  15000UL

void* Malloc(unsigned size)
{
  char buf[32];
  void* p;
   if (!size)
    size=4;
  p = malloc(size);
  return p;
}
void ResetTimer(long tid)
{
}

void Wait(long msec)
{
  SVC_WAIT(msec);
}
long SetTimer  (long msec)
{
  return (long)(read_ticks() + ((unsigned long)msec*TICKS_PER_SEC/1000));
}

BYTE CheckTimer(long tid)
{
  return (BYTE)(read_ticks()>=(unsigned long)tid ? 1 : 0);
}
TFileSystem::TFileSystem() : last_error(0ul)
{
  FileFlashSelected = 0;
  CacheTimer           = 0;
  pCache = 0;
}

BYTE TFileSystem::IsFlashFileName(const char* fName)
{
  return (BYTE)(
                 (strstr(fName, "F:") == fName) ||
                 (strstr(fName, "f:") == fName) ||
                 (strstr(fName, "FLASH:") == fName) ||
                 (strstr(fName, "flash:") == fName) ||
                 (strstr(fName, "FLASH\\") == fName) ||
                 (strstr(fName, "flash\\") == fName)
               );
}
BYTE TFileSystem::FileSelectFlash (BYTE bSelect)
{
  FileFlashSelected = bSelect;

  return 0;
}


char* TFileSystem::trueFileNamePtr(char* fname)
{
   char *p;
   char *trueNamePtr;

   p = trueNamePtr = fname;

   while ((*p) != '\0') {
     if (((*p)=='/') || ((*p)=='\\') || ((*p)==':'))
       trueNamePtr = p+1;
     p++;
   }

   return trueNamePtr;
}
T_Cached_File_Data* TFileSystem::FindCacheEntry(const char* fName)
{
  T_Cached_File_Data *p;
  p     = pCache;

  while(p && strcmp(fName, p->FileName))
    p = (T_Cached_File_Data *)p->pNext;

  return p;
}
void Free(void* ptr)
{
  if(ptr)
    free(ptr);
}

void FreeCacheEntry(T_Cached_File_Data* pNew)
{
  Free(pNew->pData);
  Free(pNew);
}

void TFileSystem::DeleteCacheEntry(T_Cached_File_Data* pNew)
{
  T_Cached_File_Data *p, *pPrev;
  p     = pCache;
  pPrev = 0;
  if(p){
    while((p != pNew) && p->pNext){
      pPrev = p;
      p = (T_Cached_File_Data *)p->pNext;
    }
    if(p == pNew){
      if(pPrev)
        pPrev->pNext = p->pNext;
      else
        pCache =(T_Cached_File_Data *)p->pNext;
      FreeCacheEntry(p);
    }
  }
}

long TFileSystem::ReadCachedFile(T_Cached_File_Data* pNew, void* pDest, long Count)
{
  long res=0;

  if((pNew->Offset+Count) > pNew->DataLen)
    Count = pNew->DataLen - pNew->Offset;

  memmove(pDest, (char*)pNew->pData + pNew->Offset, Count);
  pNew->Offset += Count;
  return Count;
}

long TFileSystem::WriteCachedFile(T_Cached_File_Data* pNew, void* pSrc, long Count)
{
  long   dwNewSize;
  void*  pNewData;

  if((pNew->Offset+Count) > pNew->DataLen){
    pNewData = Malloc(pNew->Offset+Count);
    if(pNew->pData);
      memmove(pNewData, pNew->pData, pNew->DataLen);
    Free(pNew->pData);
    pNew->pData   = pNewData;
    pNew->DataLen = pNew->Offset+Count;
  }
  memmove((char*)(pNew->pData) + pNew->Offset, pSrc, Count);
  pNew->Offset += Count;
  pNew->IsDirty = 1;
  return Count;
}

void TFileSystem::RenameCacheEntry(const char* fOldName, const char* fNewName)
{
  T_Cached_File_Data *p;
  p = FindCacheEntry(fOldName);

  if(p)
    strncpy(p->FileName, fNewName, sizeof(p->FileName));
}

void TFileSystem::RearmCacheTimer(void)
{
  if(CacheTimer)
    ResetTimer(CacheTimer);
  CacheTimer = SetTimer(TMM_CACHE_FLUSH);
}

BYTE TFileSystem::ValidHandle(int iHandle)
{
  return (BYTE)(iHandle != -1);
}

void TFileSystem::FlushCachedFile(T_Cached_File_Data* pNew)
{
  int iHandle;

  if(!pNew->FileName[0] || !pNew->IsDirty || !pNew->IsClosed)
    return;

  iHandle = open(pNew->FileName, O_CREAT | O_RDWR | O_TRUNC | O_APPEND);
  if(ValidHandle(iHandle)){
    write(iHandle, (char*)pNew->pData, pNew->DataLen);
    close(iHandle);
    pNew->IsDirty = 0;
  }
}

void TFileSystem::LoadCacheEntry(T_Cached_File_Data* pNew)
{
  int  iHandle;
  long fLen;

  if(!pNew->FileName[0])
    return;

  pNew->Offset   = 0;
  pNew->IsDirty  = 0;
  pNew->DataLen  = 0;

  iHandle = open(pNew->FileName, O_RDONLY);
  if(ValidHandle(iHandle)){

    fLen = lseek(iHandle, 0L, SEEK_END);
    lseek(iHandle, 0, SEEK_SET);
    if(fLen > 0){
      Free(pNew->pData);
      pNew->pData   = Malloc(fLen);
      pNew->DataLen = read(iHandle, (char*)pNew->pData, fLen);
    }
    close(iHandle);
  }
}
T_Cached_File_Data*  TFileSystem::NewCacheEntry(const char* fName)
{
  T_Cached_File_Data* p;
  p = (T_Cached_File_Data*)Malloc(sizeof(T_Cached_File_Data));
  strncpy(p->FileName, fName, sizeof(p->FileName));
  return p;
}


T_Cached_File_Data* TFileSystem::AppendCacheEntry(const char* fName)
{
  T_Cached_File_Data* p;
  T_Cached_File_Data* pNew;

  pNew = NewCacheEntry(fName);
  p    = pCache;
  if(!p){
    pCache = (T_Cached_File_Data*)pNew;
  }
  else{
    while(p->pNext)
      p = (T_Cached_File_Data*)p->pNext;
    p->pNext = pNew;
  }
  return pNew;
}
long TFileSystem::GetCachedFileLength(T_Cached_File_Data* pNew)
{
  return pNew->DataLen;
}
DWORD TFileSystem::FileGetPos(long fptr)
{
  return ((T_Cached_File_Data*)fptr)->Offset;
}

T_Cached_File_Data* TFileSystem::OpenCachedFile(const char* fName, WORD mode)
{
  T_Cached_File_Data *p;
  p = FindCacheEntry(fName);
  if(!p){
    p = AppendCacheEntry(fName);
    if(p){
      if(mode & DRV_FO_URGENT)
        p->IsUrgent = 1;
      LoadCacheEntry(p);
    }
  }
  if(p){
    p->Offset   = 0;
    p->IsClosed = 0;
  }
  return p;
}

long TFileSystem::OpenFile(const char* fname, WORD OpenMode){

char fileName[MAX_FILENAME_LEN];
  BYTE bUseFlash = 0;

  if (FileFlashSelected || IsFlashFileName(fname))
    bUseFlash = 1;

  FileFlashSelected = 0;

  fileName[0] = 0;
  if (bUseFlash)
    strcpy(fileName, "F:");

  strcat(fileName, trueFileNamePtr((char*)fname));

  return (long)OpenCachedFile(fileName, OpenMode);
  
}

void TFileSystem::CloseFile(long fptr)
{
  T_Cached_File_Data* p = (T_Cached_File_Data*)fptr;
  if(p){
    p->Offset = 0;
    p->IsClosed = 1;
    if(p->IsDirty)
      RearmCacheTimer();
    if(p->IsUrgent)
      FlushCachedFile(p);
  }
}

DWORD TFileSystem::WriteFile(long fptr, void* pBuf, DWORD sz)
{
  return WriteCachedFile((T_Cached_File_Data*)fptr, pBuf, sz);
}
DWORD TFileSystem::ReadFile(long fptr, void* pBuf, DWORD sz)
{
  return ReadCachedFile((T_Cached_File_Data*)fptr, pBuf, sz);
}

DWORD TFileSystem::GetFreeSpaceInKbytes(){
  struct fs_size fs;
  BYTE bUseFlash = 0;
  DWORD ramSize;

  if (FileFlashSelected)
    bUseFlash = 1;

  FileFlashSelected = 0;
  ramSize = dir_get_sizes("F:",&fs);

  fs.Avail /= 1024;
  ramSize  /= 1024;
  return bUseFlash ? fs.Avail : ramSize;
}

Error TFileSystem::FlushCacheToDisk(){
	return 0;
}

DWORD TFileSystem::FileGetLength(long fptr)
{
  return GetCachedFileLength((T_Cached_File_Data*)fptr);
}

Error TFileSystem::EraseFile(const char* fileName){
  char fName[MAX_FILENAME_LEN];
  BYTE ret = SUCCESS;
  BYTE bUseFlash = 0;
  T_Cached_File_Data* p;

  if (FileFlashSelected || IsFlashFileName(fileName))
    bUseFlash = 1;

  FileFlashSelected = 0;

  fName[0] = 0;
  if (bUseFlash)
    strcpy(fName, "F:");
  strcat(fName, trueFileNamePtr((char*)fileName));

  p = FindCacheEntry(fName);
  if(p)
    DeleteCacheEntry(p);

  if (remove ((char*)fName) == -1) {
    if (errno != ENOENT) {
      ret = 1;
    }
  }
  return ret;
}

Error TFileSystem::RenameFile(const char* oldName, const char* newName){
  long src;
  long dst;
  BYTE *copyBuf;
  DWORD curRead;
  DWORD curWritten;
  DWORD totRead;
  DWORD totWritten;
  BYTE ret;

  BYTE bFlashSource = 0;
  BYTE bFlashTarget = 0;

  if (FileFlashSelected || IsFlashFileName(oldName))
    bFlashSource = 1;

  if (FileFlashSelected || IsFlashFileName(newName))
    bFlashTarget = 1;

  FileFlashSelected = 0;

  ret = SUCCESS;
  src = dst = 0;
  curRead = curWritten = totRead = totWritten = 0;
  copyBuf = 0;


  FileSelectFlash(bFlashTarget);
  ret = EraseFile(newName);

  if (ret == SUCCESS) {

    if (bFlashSource || bFlashTarget) {

      FileSelectFlash(bFlashSource);
      src = OpenFile(oldName,DRV_FO_READ);

      FileSelectFlash(bFlashTarget);
      dst = OpenFile(newName,DRV_FO_RDWR);

      copyBuf = (BYTE*)Malloc(1024);

      if (src && dst && copyBuf) {
        while ( (curRead = ReadFile(src, copyBuf, 1024))>0 ) {
          totRead += curRead;
          curWritten = WriteFile(dst, copyBuf, curRead);
          if (curWritten != curRead)
            break;
          else
            totWritten += curWritten;
        }
        if (totRead != totWritten)
          ret = 1;
      }
      else
        ret = 1;
    }
    else {
      if (_rename(oldName, newName) == -1)
        ret = 1;
      else
        RenameCacheEntry(oldName, newName);
    }
  }

  if (src) CloseFile(src);
  if (dst) CloseFile(dst);
  if (copyBuf) Free(copyBuf);

  if (ret == SUCCESS) {
    FileSelectFlash(bFlashSource);
    EraseFile(oldName);
  }
  else {
    FileSelectFlash(bFlashTarget);
    EraseFile(newName);
  }

  return ret;

}


Error TFileSystem::GetLastError(){
	return last_error;
}



