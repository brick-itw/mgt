#ifndef I_DEVICE_LAYER
#define I_DEVICE_LAYER

#include "Platforms/Interfaces/IDevice.h"
#include "Base/Types/types.h"

class IDeviceLayer
{
protected:
  /**
   * @brief This function will allocate memory for device. Every Device should be destroyed
   * @note  Error of device creation we should catch by GetLastError
   * @param deviceType type of device @see DEVICE
   * @param deviceIndex number of device same type (device can have two displays)
   * @return pointer to created device
   */
  virtual IDevice* CreateDevice(DEVICE deviceType, unsigned deviceIndex = 0) = 0;

  template<class T>
  T* CreateDeviceT(DEVICE deviceType, unsigned deviceIndex = 0)
  {
    return dynamic_cast<T*>(CreateDevice(deviceType, deviceIndex));
  }

public:
  /**
   * @brief  Simple test of device without opening
   * @note   Error of device creation we should catch by GetLastError
   * @return SUCCESS when test ok or not available without opening, ERROR_NO_SUPPORT, or error of test.
   */
  virtual Error Test(DEVICE deviceType, unsigned* deviceCount) = 0;

  //get device from cache or create and init. Will return NULL for platforms with no device cache
  virtual IDevice* GetDevice(DEVICE deviceType, unsigned deviceIndex = 0) = 0;

  virtual Error GetLastError() = 0;

  virtual ~IDeviceLayer() {}
};

#endif
