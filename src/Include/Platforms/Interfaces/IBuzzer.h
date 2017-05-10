/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Buzzer device interface declaration
 */

#ifndef _U2_BUZZER_H
#define _U2_BUZZER_H

#include "Platforms/Interfaces/IDevice.h"
#include "Base/Types/modules.h"

#define BUZZER_ERROR(code) REGISTER_ERROR(MODULE_BUZZER, (code))

enum BuzzerErrorCode
{
  ERROR_BUZZER_INTERNAL   = BUZZER_ERROR(1),
};

/**
*   @brief  Interface of terminal buzzer
*/
class IBuzzer: public IDevice {
public:
  virtual Error BeepOK()    = 0;
  virtual Error BeepError() = 0;
};

#endif
