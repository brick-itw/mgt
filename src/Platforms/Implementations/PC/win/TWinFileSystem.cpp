#include <windows.h>
#include "Platforms/Implementations/Pc/win/TWinFileSystem.h"

DWORD TWinFileSystem::GetFreeSpaceInKbytes()
{
  ULARGE_INTEGER FreeBytesAvailableToCaller, TotalNumberOfBytes;

  GetDiskFreeSpaceEx(NULL,&FreeBytesAvailableToCaller, &TotalNumberOfBytes, NULL);
  return    FreeBytesAvailableToCaller.LowPart>>10 | 
          ((FreeBytesAvailableToCaller.HighPart & 0x3FFL)<<22);
}

DWORD TWinFileSystem::GetFileLength(const char* fileName)
{
  WIN32_FILE_ATTRIBUTE_DATA Data;
  char fullName[MAX_PC_FILE_NAME_LEN];

  if(!GetFileAttributesEx(MakeFullPath(fullName, fileName),GetFileExInfoStandard,&Data))
    return 0;
  return Data.nFileSizeLow;
}

