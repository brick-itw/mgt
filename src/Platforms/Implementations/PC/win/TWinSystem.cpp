
#ifndef _CONSOLE
  #include "stdafx.h"
  #include "TestDlg.h"
  #include "TestDlgDlg.h"
  #include "afxdialogex.h"
  #include "Platforms/Implementations/Pc/win/TWinDisplay.h"
  #include "Platforms/Implementations/Pc/win/TWinKeyboard.h"
  #include "Platforms/Implementations/PC/win/TWinTouchscreen.h"
#endif

#include "Platforms/Implementations/Pc/win/TWinSystem.h"
#include "Platforms/Implementations/Pc/win/TWinFileSystem.h"
#include "Platforms/Implementations/Pc/win/TWinRS232Interface.h"
#include "Platforms/Implementations/Pc/win/TWinEthernetInterface.h"
#include "Platforms/Implementations/Pc/win/TPCSC_Cardreader.h"
#include "Platforms/Implementations/Pc/win/TWinCryptoDevice.h"
#include "Platforms/Implementations/Pc/win/TWinDeviceLayer.h"
#include "Base/Types/errno_error.h"
#include "Base/Types/errors.h"
#include "Common/Utils/FormatConverters.h"

#include <stdio.h>


Error TWinSystem::GetDateTime(DWORD* pDate, DWORD* pTime)
{
  SYSTEMTIME dt;
  GetLocalTime(&dt);

  if(pDate)
    *pDate = dt.wYear * 10000L +dt.wMonth*100 + dt.wDay;
  if(pTime)
    *pTime = dt.wHour * 10000L +dt.wMinute*100+dt.wSecond; 

  return SUCCESS;
}

void TWinSystem::Wait(DWORD msec)
{
  Sleep(msec);
}

ITimer* TWinSystem::CreateTimer(DWORD msec)
{
  return new TWinTimer(msec);
}

Error TWinSystem::MountFileSystem(const char* path, IFileSystem** fileSystem)
{
  CreateDirectory(path, NULL);
  *fileSystem = new TWinFileSystem(path);
  return SUCCESS;
}

void TWinSystem::CreateDeviceLayer()
{
#ifdef _NEED_TRACES
  MountFileSystem("Trace", (IFileSystem**)&pTraces);
#endif
  pDeviceLayer = new TWinDeviceDebugLayer(pTraces);
}

void TWinSystem::Idle()
{
#ifndef _CONSOLE
  MSG   Message;
  if(PeekMessage(&Message,NULL,0,0,PM_REMOVE) ) 
  {
    ((CTestDlgDlg*)(theApp.m_pMainWnd))->PreTranslateMessage(&Message);
    TranslateMessage(&Message);
    DispatchMessage(&Message);
  }
#endif
}

void TWinSystem::FillModelName(char* pDest, int maxLen)
{
  strncpy(pDest, "WIN32 DEBUG TERMINAL", maxLen);
}

void TWinSystem::GetScreenSize(WORD &width, WORD &height)
{
  width = 240;
  height = 320;
}

Error TWinSystem::GenerateRandom(BYTE* pData8)
{
  return TPCSystem::GenerateRandom(pData8);
}

Error TWinSystem::UpdateSoftware(const char* archiveName){
  Error err = SUCCESS;
  system("del TestDlgOld.exe");
  system("rename TestDlg.exe TestDlgOld.exe");
  system("rename new.exe TestDlg.exe");
  return err;
}
