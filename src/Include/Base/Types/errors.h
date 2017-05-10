/**
 * @Author aakhartov.sbt@sberbank.ru
 * @date   August 2016
 * @brief  Common errors list. This errors can be used on any module
 */

#ifndef _UPOS_ERRORS_H
#define _UPOS_ERRORS_H

#include "Base/Types/modules.h"

#define COMMON_ERROR(code) REGISTER_ERROR(MODULE_COMMON,     (code))

enum CommonErrorCode
{
  ERROR_OK                = COMMON_ERROR(0),

  ERROR_NO_MEMORY         = COMMON_ERROR(1),
  ERROR_NO_SUPPORT        = COMMON_ERROR(2),

  ERROR_PROCESSING        = COMMON_ERROR(5),

  ERROR_USER_CANCELLED    = COMMON_ERROR(2000),
  ERROR_USER_TIMEOUT      = COMMON_ERROR(2002),

  ERROR_CODE_MENU_PRESSED = COMMON_ERROR(8000),
  ERROR_CODE_MENU         = COMMON_ERROR(8001), //menu will return code (8001 + n), where n = selected menu item, starting from 0

  ERROR_UNKNOWN           = COMMON_ERROR(9999)
};

#endif // _UPOS_ERRORS_H
