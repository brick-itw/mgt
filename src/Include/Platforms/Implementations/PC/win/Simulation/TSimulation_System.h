#ifndef _U2_WIN_SIMULATION_SYSTEM_H
#define _U2_WIN_SIMULATION_SYSTEM_H

#include "Platforms/Implementations/PC/Win/TWinSystem.h"
#include "Platforms/Implementations/PC/Win/TWinDeviceLayer.h"

class TSimulation_DeviceLayer : public TWinDeviceDebugLayer {
protected:
  std::string TestFileName;

public:
  TSimulation_DeviceLayer(const std::string& testname) : TWinDeviceDebugLayer(NULL), TestFileName(testname)
  {
  };

  virtual ~TSimulation_DeviceLayer()
  {
  };

  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0);
};


class TSimulation_System : public TWinSystem {
protected:
  std::string TestFileName;
  std::string ConfigPath;

  virtual void CreateDeviceLayer();

public:
  TSimulation_System(const std::string& testname);
  virtual ~TSimulation_System();

  virtual Error GetDateTime(DWORD* pdwDate, DWORD* pdwTime);
  virtual Error GenerateRandom(BYTE* pData8);

  const std::string& GetConfigPath() const
  {
    return ConfigPath;
  };
};

#endif
