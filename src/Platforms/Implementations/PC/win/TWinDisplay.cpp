#include "Platforms/Implementations/PC/Win/TWinDisplay.h"
#include "Platforms/Interfaces/ISystem.h"

#ifndef _CONSOLE
  #include "stdafx.h"
  #include <Windows.h>
  #include "TestDlg.h"
  #include "TestDlgDlg.h"
#endif

TWinDisplay::TWinDisplay() : IGraphicDisplay()
{
}

void TWinDisplay::PaintCanvas(TCanvas *canvas)
{
#ifndef _CONSOLE
  ((CTestDlgDlg*)(theApp.m_pMainWnd))->SetCanvas(canvas);
  theApp.m_pMainWnd->Invalidate(FALSE);
  ISystem::Instance()->Idle();
#endif
}

