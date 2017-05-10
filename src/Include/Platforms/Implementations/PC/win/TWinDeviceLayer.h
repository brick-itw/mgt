#ifndef T_WIN_DEVICE_LAYER
#define T_WIN_DEVICE_LAYER

#include "Platforms/Implementations/TDeviceLayer.h"

class TWinDeviceLayer : public TDeviceLayer {
protected:
  Error LastError;

public:
  TWinDeviceLayer() : LastError(SUCCESS)
  {
  };

  virtual ~TWinDeviceLayer()
  {
  };

  virtual Error Test(DEVICE deviceType, unsigned* deviceCount);
  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0) = 0;

  virtual Error GetLastError()
  {
    return LastError;
  };
};

class TWinDeviceDebugLayer : public TWinDeviceLayer {
protected:
  IFileSystem*  pLogDirectory;

public:
  TWinDeviceDebugLayer(IFileSystem*  pLogs) : TWinDeviceLayer()
  {
  pLogDirectory = pLogs;
  };

  virtual ~TWinDeviceDebugLayer()
  {
  };

  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0);
};

#endif
