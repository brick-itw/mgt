/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  List of supported modules of abstract device layer
 */

#ifndef _UPOS_ERROR_MODULES_H
#define _UPOS_ERROR_MODULES_H

//#define REGISTER_ERROR(module, code) (((module) << 16) | (code))
#define REGISTER_ERROR(module, code) (((module) * 1000) + (code))
#define SUCCESS (0UL)
enum Module
{
  MODULE_COMMON                  = 0,
  MODULE_SYSTEM                  = 1,
  MODULE_CARD_READER             = 2,
  MODULE_CRYPTO                  = 3,
  MODULE_FILE                    = 4,
  MODULE_DISPLAY                 = 5,
  MODULE_PRINTER                 = 6,
  MODULE_KEYBOARD                = 7,
  MODULE_BIO                     = 8,
  MODULE_BUZZER                  = 9,
  MODULE_POWER                   = 10,
  MODULE_TOUCHSCREEN             = 11,
  MODULE_INFO                    = 12,
  MODULE_FILESYSTEM              = 13,
  MODULE_CONNECTION              = 14,
  MODULE_OS_WIN                  = 15,
  MODULE_OS_LIN                  = 16,
  MODULE_OS_PROLIN		           = 17,
  MODULE_EMV			               = 18,
  MODULE_LOGIC                   = 19,
  MODULE_COMMAND_ENTER_CARD      = 20,
  MODULE_HTTP                    = 21,
  MODULE_BITMAP_MANAGER          = 22,
  MODULE_FONT_MANAGER            = 23,
  MODULE_SIMULATION_READER       = 24,
  MODULE_CHECK_FILES             = 25,
  MODULE_COMMAND_LOAD_KEY        = 26,
  MODULE_COMMAND_READ_CARD       = 27,
};

#endif // _UPOS_ERROR_MODULES_H
