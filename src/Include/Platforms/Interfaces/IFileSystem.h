/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  File System interface declaration
 */

#ifndef _U2_FILESYSTEM_H
#define _U2_FILESYSTEM_H

#include "Base/Types/types.h"
#include "Platforms/Interfaces/IFile.h"
#include "Base/Types/modules.h"

#define FILESYSTEM_ERROR(code) REGISTER_ERROR(MODULE_FILESYSTEM, (code))

enum FilesystemErrorCode
{
  ERROR_FILE_OPEN_FAILURE   = FILESYSTEM_ERROR(1),
  ERROR_FILE_ERASE_FAILURE  = FILESYSTEM_ERROR(2),
  ERROR_FILE_RENAME_FAILURE = FILESYSTEM_ERROR(3),
  ERROR_FILE_INVALID_DIR    = FILESYSTEM_ERROR(4),
};


// must open or create a file with specified name and attributes
//
// mode combines FO_xxx bits
//
#define FO_READ      0x01
#define FO_WRITE     0x02
#define FO_CREATE    0x04
#define FO_TRUNC     0x08
#define FO_APPEND    0x10
#define FO_NO_CACHE  0x20

class IFileSystem {

public:
  virtual ~IFileSystem(){};  // must dismount file system

  virtual DWORD GetFreeSpaceInKbytes() = 0;
  virtual Error FlushCacheToDisk    () = 0;
  virtual DWORD GetFileLength       (const char* fileName) = 0;
  virtual Error EraseFile           (const char* fileName) = 0;
  virtual Error RenameFile          (const char* oldName, const char* newName) = 0;

  virtual IFileReadable* OpenForRead(const char* fname) = 0;
  virtual IFileWritable* OpenForWrite(const char* fname, WORD mode) = 0;

  virtual Error GetLastError() = 0;
};

#endif
