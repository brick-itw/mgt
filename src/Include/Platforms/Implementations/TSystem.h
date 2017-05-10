#ifndef T_SYSTEM
#define T_SYSTEM

#include "Platforms/Interfaces/ISystem.h"
#include "Platforms/Implementations/TDeviceLayer.h"
#include "Platforms/Interfaces/IFileSystem.h"

class TSystem : public ISystem{
protected:
  TDeviceLayer*   pDeviceLayer;
  IFileSystem*    pTraces;
  virtual void CreateDeviceLayer() = 0;

  IDeviceLayer* GetDeviceLayer()
  {
    if(!pDeviceLayer)
      CreateDeviceLayer();
    return pDeviceLayer;
  }

public:
  Error UnmountFileSystem(IFileSystem* fileSystem)
  {
    if(fileSystem)
      delete fileSystem;
    return SUCCESS;
  }

  virtual ~TSystem(){
    UnmountFileSystem(pTraces);
  };

  TSystem(){
    pDeviceLayer = NULL;
    pTraces = NULL;
  }
};

#endif
