#ifndef TOUCH_SCREEN_PAX
#define TOUCH_SCREEN_PAX

#include "Platforms/Implementations/TTouchScreen.h"

class TPaxProlinTouchScreen : public TTouchScreen{
public:
  virtual Error Start(){}
  virtual Error Stop(){}

  virtual Error Open();
  virtual void Close();

  virtual void Update();

private:
  unsigned char state;
  unsigned int xx, yy;
};

#endif
