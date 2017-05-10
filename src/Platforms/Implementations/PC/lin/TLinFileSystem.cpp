#include "Platforms/Implementations/PC/lin/TLinFileSystem.h"
#include <stdio.h>
#include <sys\stat.h>
#include <sys\vfs.h>

DWORD TLinFileSystem::GetFreeSpaceInKbytes()
{
  struct statfs buf;
  int           res;

  res = statfs("/", &buf);
  return ((res < 0) ? 0 : (buf.f_bfree * buf.f_bsize / 1024));
}

DWORD TLinFileSystem::GetFileLength(const char* fileName)
{
  struct stat buf;
  char	      fullName[MAX_PC_FILE_NAME_LEN];
  int         res;
  
  res = stat(MakeFullPath(fullName, fileName), &buf);
  return (res < 0) ? 0 : buf.st_size;
}