#ifndef TOUCH_SCREEN_WIN
#define TOUCH_SCREEN_WIN

#include "Platforms/Implementations/TTouchScreen.h"

class TTouchScreenWin : public TTouchScreen{
public:
  virtual Error Start(){return SUCCESS;}
  virtual Error Stop(){return SUCCESS;}

  virtual Error Open();
  virtual void Close();

  virtual void Update();

private:
  unsigned char state;
  unsigned int xx, yy;

};



#endif
