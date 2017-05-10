#include "Base/Types/errors.h"
#include "Platforms/Implementations/Pc/TPCSystem.h"
#include <stdlib.h>
#include <stdio.h>

TPCSystem::TPCSystem()
{
}

TPCSystem::~TPCSystem()
{
}

Error TPCSystem::GenerateRandom(BYTE* pData8)
{
  DWORD dt, tm;
  GetDateTime(&dt, &tm);
  srand(tm);
  for(int i = 0;  i < 8; ++i)
    pData8[i] = (BYTE)rand();
  return SUCCESS;
}

Error TPCSystem::Shutdown()
{
  exit(0);
  return SUCCESS;
}

Error TPCSystem::Restart()
{
  exit(-1);
  return ERROR_NO_SUPPORT;
}

Error TPCSystem::UpdateSoftware(const char* archiveName)
{
  return ERROR_NO_SUPPORT;
}

Error TPCSystem::SetDateTime(DWORD dwDate, DWORD dwTime)
{
  return ERROR_NO_SUPPORT;
}

void TPCSystem::Idle()
{
}

void TPCSystem::FillOsVersion(char* pDest, int maxLen)
{
  strncpy(pDest, "PC system", maxLen);
}

