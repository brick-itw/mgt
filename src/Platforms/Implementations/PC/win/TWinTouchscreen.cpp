#include "Platforms\Implementations\PC\win\TWinTouchscreen.h"
#include "Platforms/Interfaces/ISystem.h"

#ifndef _CONSOLE
  #include "stdafx.h"
  #include <Windows.h>
  #include "TestDlg.h"
  #include "TestDlgDlg.h"
#endif

Error TTouchScreenWin::Open(){
  return SUCCESS;
}

void TTouchScreenWin::Close(){
}

void TTouchScreenWin::Update(){
#ifndef _CONSOLE
  CTestDlgDlg *dlg = (CTestDlgDlg*)(theApp.m_pMainWnd);
  if(dlg && dlg->HasTouch()){
    CPoint p = dlg->GetLastPointTouched();
  
    TouchPoint point;
    point.x = p.x;
	  point.y = p.y;
	  PutTouch(point);
  }
#endif
}
