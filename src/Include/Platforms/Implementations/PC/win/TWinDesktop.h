#ifndef _U2_WIN_DESKTOP
#define _U2_WIN_DESKTOP

#include "UI/IDesktop.h"
#include "GenericParamDlg.h"

class TWinDesktop : public IDesktop{
protected:

  DWORD             dwCurrentWallpaper;
  CGenericParamDlg* pCurrentWallpaper;


  CGenericParamDlg* CreateDialogByID(DWORD id, TScreenOptions* pOptions);

  void       UpdateWallpaperParams(const TScreenOptions* pOptions);
  void       KillWallpaper();
  void       CreateWallpaper(DWORD screenID);


public:
  TWinDesktop();
  virtual ~TWinDesktop();

  virtual Error Execute      (DWORD screenID,       TScreenOptions* pOptions = NULL);
  virtual void  ShowWindow   (DWORD screenID,       TScreenOptions* pOptions = NULL);
  virtual Error GetWindowStatus(TScreenOptions* pOptions = NULL);

  virtual void ShowStatus(std::string status);

  virtual void  Reload();  
};

#endif
