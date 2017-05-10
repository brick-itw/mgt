/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Touch screen device interface declaration
 */

#ifndef _U2_TOUCHSCREEN_H
#define _U2_TOUCHSCREEN_H

#include "Base/Types/inttypes.h"
#include "Platforms/Interfaces/IDevice.h"
#include "Base/Types/modules.h"

#include <deque>

#define TOUCHSCREEN_ERROR(code) REGISTER_ERROR(MODULE_TOUCHSCREEN, (code))

enum TouchScreenErrorCode
{
  ERROR_TOUCHSCREEN_INTERNAL = TOUCHSCREEN_ERROR(1),
};

struct TouchPoint{
	TouchPoint(){
		x = 0;
		y = 0;
	}
	int x;
	int y;
};

class ITouchScreen : public IDevice{

public:
  /*@brief start capturing points
  */
  virtual Error Start()  = 0;

  /*@brief stop capturing points
  */
  virtual Error Stop()  = 0;
  virtual void Clear() = 0;
  virtual int IsTouchPressed() = 0;
  virtual TouchPoint GetTouch(size_t position = 0) = 0;//function will not remove touch from deque
  virtual TouchPoint GetFirstTouch() = 0;//function will remove touch from deque
  virtual void Update() = 0;

protected:
  virtual void PutTouch(TouchPoint point) = 0;
};


#endif

