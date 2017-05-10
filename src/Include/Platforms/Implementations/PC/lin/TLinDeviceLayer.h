#ifndef T_LIN_DEVICE_LAYER
#define T_LIN_DEVICE_LAYER

#include "Platforms/Implementations/TDeviceLayer.h"

class TLinDeviceLayer : public TDeviceLayer {
protected:
  Error LastError;

public:
  TLinDeviceLayer() : LastError(SUCCESS)
  {
  };

  virtual Error Test(DEVICE deviceType, unsigned* deviceCount);
  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0) = 0;

  virtual Error GetLastError()
  {
    return LastError;
  };
};

class TLinDeviceDebugLayer : public TLinDeviceLayer {
protected:
  IFileSystem*  pLogDirectory;

public:
  TLinDeviceDebugLayer(IFileSystem*  pLogs) : TLinDeviceLayer()
  {
    pLogDirectory = pLogs;
  };

  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0);
};

#endif
