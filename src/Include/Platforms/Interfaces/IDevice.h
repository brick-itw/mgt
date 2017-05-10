/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Device list and device interface declaration
 */

#ifndef _U2_DEVICE_H
#define _U2_DEVICE_H

#include "Base/Types/error.h"

typedef enum{
  DVC_SYSTEM    = 0, ///< @ see ISystem
  DVC_CARD_READER,
  DVC_CRYPTO,
  DVC_DISPLAY,         ///< @see IGraphicDisplay
  DVC_PRINTER,         ///< @see IGraphicPrinter
  DVC_KEYBOARD,        ///< @see IKeyboard
  DVC_BUZZER,          ///< @see IBuzzer
  DVC_POWER,           ///< @see IPowerStatus
  DVC_TOUCHSCREEN,     ///< @see ITouchScreen
  DVC_ETHERNET,        ///< @see IEthernetInterface
  DVC_WIFI,            ///< @see IWiFiInterface
  DVC_GPRS,            ///< @see IGprsInterface
  DVC_RS232,           ///< @see IRS232Interface
  DVC_LOOPBACK,        ///< @see ILoopbackInterface
} DEVICE;


class IDevice
{
public:
  virtual Error Open() = 0;
  virtual void Close() = 0;

  virtual ~IDevice(){};
};

#endif
