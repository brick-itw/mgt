#include "Platforms/Implementations/PaxProlin/TPaxProlinFileSystem.h"
#include "Base/Types/errno_error.h"
#include <stdio.h>
#include <unistd.h>

char* TPaxProlinFileSystem::MakeFullPath(char* pDest, const char* fname)
{
  if(!strcmp(fname,"PAX.AIP"))
    strcpy(pDest, fname);  
  else  	
  if(pDest[0])
    sprintf(pDest, "data/%s/%s", Directory, fname);
  else
    sprintf(pDest, "data/%s", fname);

  return pDest;
}

FILE* TPaxProlinFileSystem::OpenFilePointer(const char* fname, WORD mode)
{
  char fullName[MAX_PC_FILE_NAME_LEN];
  const char* fmode;
  FILE* fptr;

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

  if(!fptr){
    LastError = ErrnoError(ERROR_FILE_OPEN_FAILURE);
    return NULL;
  }

  if(mode & FO_APPEND)
    fseek(fptr, 0, SEEK_END);

  return fptr;
}


