#ifndef _U2_WIN_DISPLAY_H
#define _U2_WIN_DISPLAY_H

#include "Ui/IGraphicDisplay.h"

class TWinDisplay : public IGraphicDisplay  {
public:
  TWinDisplay();

  virtual void  PaintCanvas(TCanvas *canvas);
  virtual void  Backlight(BOOLEAN On){};

  virtual Error Open(){return SUCCESS;};
  virtual void Close(){};
};


#endif

