#include "Platforms/Implementations/PC/Win/TWinKeyboard.h"
#include "Platforms/Interfaces/ISystem.h"
#include "UI/Keys.h"
#ifndef _CONSOLE
  #include "stdafx.h"
  #include <Windows.h>
  #include "TestDlg.h"
  #include "TestDlgDlg.h"
#endif



BYTE TWinKeyboard::GetKey()
{
#ifndef _CONSOLE
  return ((CTestDlgDlg*)(theApp.m_pMainWnd))->GetLastKeyPressed();
#else
  return 0;
#endif
}


Error TWinKeyboard::Open()
{
	return SUCCESS;
}

void TWinKeyboard::Close()
{
}
