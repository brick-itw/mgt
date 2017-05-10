#ifndef T_DEVICE_LAYER
#define T_DEVICE_LAYER

#include "Platforms/Interfaces/ISystem.h"

//implement cache of devices
class TDeviceLayer : public IDeviceLayer
{
private:
  std::map<DWORD, IDevice*> devices;

  DWORD GetDeviceID(DEVICE deviceType, unsigned deviceIndex = 0)
  {
    DWORD res = ((DWORD)deviceType << 16) + deviceIndex;
    return res;
  };

public:
  virtual IDevice* GetDevice(DEVICE deviceType, unsigned deviceIndex = 0)
  {
    DWORD device_id = GetDeviceID(deviceType, deviceIndex);
    IDevice *device = devices[device_id];
    if(!device)
    {
      device = CreateDevice(deviceType, deviceIndex);
      if(device)
        devices[device_id] = device;
    }
    return device;
  }
};

#endif
