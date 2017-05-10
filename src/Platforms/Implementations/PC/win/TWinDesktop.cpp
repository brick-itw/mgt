#include "Platforms/Implementations/Pc/win/TWinDesktop.h"
#include "Resource.h"
#include "PleaseWaitDlg.h"
#include "EnterDialog.h"
#include "MenuDialog.h"
#include "EnterPinDialog.h"
#include "EnterCardDialog.h"
#include "EnterAmountDialog.h"
#include "StartupDialog.h"
#include "ErrorDialog.h"
#include "UI/screen_ids.h"

TWinDesktop::TWinDesktop()
{
  dwCurrentWallpaper = 0;
  pCurrentWallpaper = 0;
}

TWinDesktop::~TWinDesktop()
{
  if(pCurrentWallpaper)
    delete pCurrentWallpaper;
}

IDesktop* IDesktop::Instance()
{
  static TWinDesktop* pDesktop = 0;
  if(!pDesktop)
    pDesktop = new TWinDesktop;
  return pDesktop;
}

CGenericParamDlg* TWinDesktop::CreateDialogByID(DWORD id, TScreenOptions* pOptions)
{
  CGenericParamDlg* p;
  switch(id){
    case IDS_PLEASE_WAIT:    p = new PleaseWaitDlg(pOptions); break;
    case 102:                p = new EnterAmountDialog(pOptions); break;
    case IDS_GENERIC_ENTER:  p = new CEnterDialog(pOptions); break;
    case IDS_GENERIC_MENU:   
    case IDS_SELECT_APP:   
    case IDS_SELECT_LANGUAGE:p = new CMenuDialog(pOptions); break;
    case IDS_ENTER_PIN:      p = new EnterPinDialog(pOptions); break;
    case IDS_ENTER_CARD:     p = new EnterCardDialog(pOptions); break;
    case IDS_START_UP:       p = new StartupDialog(pOptions); break;
    case IDS_ERROR:          p = new ErrorDialog(pOptions); break;
    default:                 return NULL;
  };
  return p;
}


void TWinDesktop::UpdateWallpaperParams(const TScreenOptions* pOptions)
{
  if(pCurrentWallpaper){
    pCurrentWallpaper->UpdateScreenParams((TScreenOptions*)pOptions);
    pCurrentWallpaper->ShowWindow(TRUE);
  }
}

void TWinDesktop::KillWallpaper()
{
  if(pCurrentWallpaper){
    delete pCurrentWallpaper;
    pCurrentWallpaper = NULL;
  }
  dwCurrentWallpaper = 0;
}

void TWinDesktop::CreateWallpaper(DWORD screenID)
{
  KillWallpaper(); 
  dwCurrentWallpaper = screenID;
  pCurrentWallpaper  = CreateDialogByID(screenID, NULL);
  pCurrentWallpaper->Create(screenID);
}

Error TWinDesktop::Execute (DWORD screenID, TScreenOptions* pOptions)
{
  CDialogEx* pDlg = CreateDialogByID(screenID, pOptions);
  if(!pDlg)
    return ERROR_FORM_NOT_FOUND;  
  
  KillWallpaper();
  DWORD result = pDlg->DoModal();

  delete pDlg;
  return (result == IDOK) ? SUCCESS : result;
}

void TWinDesktop::ShowWindow(DWORD screenID, TScreenOptions* pOptions)
{
  if(screenID != dwCurrentWallpaper)
    CreateWallpaper(screenID);
  UpdateWallpaperParams(pOptions);
}

Error TWinDesktop::GetWindowStatus(TScreenOptions* pOptions)
{
  if(!pCurrentWallpaper)
    return ERROR_NO_MODELESS_WND;
  if(SUCCESS == pCurrentWallpaper->GetModelessResult())
    pCurrentWallpaper->OutputControlParams(pOptions);
  return pCurrentWallpaper->GetModelessResult();
}

void TWinDesktop::ShowStatus(std::string status)
{
}

void TWinDesktop::Reload()
{
}
