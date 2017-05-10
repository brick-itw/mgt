/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Internal timer interface declaration
 */

#ifndef _U2_TIMER_H
#define _U2_TIMER_H

#include "Base/Types/inttypes.h"

class ITimer {
public:
  virtual ~ITimer(){};

  virtual BOOLEAN IsExpired() = 0;
  virtual void    Restart(DWORD newMsec = 0) = 0;
};

#endif
