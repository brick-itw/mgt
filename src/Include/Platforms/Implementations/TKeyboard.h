/**
 * @Author DGKokorev@sberbank.ru
 * @date   March 2017
 * @brief  Terminal keyboard device base class
 */

#ifndef _U2_T_KEYBOARD_H
#define _U2_T_KEYBOARD_H

#include "Platforms/Interfaces/IKeyboard.h"
#include <map>

class TKeyboard : public IKeyboard {
public:
  virtual BYTE GetKey() = 0;  // must return 0 if no key is currently pressed
protected:
  std::map<int, int> upos_keys;//used for conversion from platform specific key codes to upos key codes
};

#endif
