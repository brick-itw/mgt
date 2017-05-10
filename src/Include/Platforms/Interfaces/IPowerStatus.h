/**
 * @Author avbrik@sberbank.ru
 * @date   August 2016
 * @brief  Power management information interface declaration
 */

#ifndef _U2_POWERSTATUS_H
#define _U2_POWERSTATUS_H

#include "Base/Types/types.h"
#include "Platforms/Interfaces/IDevice.h"

class IPowerStatus : public IDevice {
public:

  virtual BOOLEAN IsBatteryInside()         = 0;
  virtual BOOLEAN IsPowerSupplyConnected()  = 0;
  virtual BYTE GetBatteryStateOutOf100() = 0;
};

#endif
