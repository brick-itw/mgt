/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Terminal keyboard device interface declaration
 */

#ifndef _U2_KEYBOARD_H
#define _U2_KEYBOARD_H

#include "Platforms/Interfaces/IDevice.h"
#include "Base/Types/modules.h"
#include "Base/Types/inttypes.h"

class IKeyboard : public IDevice {
public:
  virtual BYTE GetKey() = 0;  // must return 0 if no key is currently pressed
};

#endif
