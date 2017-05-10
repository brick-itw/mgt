#include "Platforms/Implementations/PC/win/TWinDeviceLayer.h"
#include "Platforms/Implementations/PC/win/TWinRS232Interface.h"
#ifndef _CONSOLE
  #include "Platforms/Implementations/PC/win/TWinDisplay.h"
  #include "Platforms/Implementations/PC/win/TWinTouchscreen.h"
#endif
#include "Platforms/Implementations/PC/win/TWinKeyboard.h"
#include "Platforms/Implementations/PC/win/TWinEthernetInterface.h"
#include "Platforms/Implementations/PC/win/TWinCryptoDevice.h"
#include "Platforms/Implementations/PC/win/TPCSC_CardReader.h"
#include "Platforms/Implementations/PC/win/Simulation/TSimulation_CardReader.h"
#include "Common/Utils/FormatConverters.h"

Error TWinDeviceLayer::Test(DEVICE deviceType, unsigned* deviceCount)
{
  *deviceCount = 0;
  return (LastError = ERROR_NO_SUPPORT);
}

IDevice* TWinDeviceDebugLayer::CreateDevice(DEVICE deviceType, unsigned deviceIndex)
{
  std::string script_name;

  switch(deviceType)
  {
  case DVC_RS232:
    return new TWinRS232Interface(pLogDirectory);

  case DVC_ETHERNET:
  case DVC_WIFI:
  case DVC_GPRS:
  case DVC_LOOPBACK:
    return new TWinEthernetInterface(pLogDirectory);

#ifndef NO_CARD_READER
  case DVC_CARD_READER:
    return new TPCSC_CardReader(pLogDirectory);
#endif

#ifndef NO_CRYPTO_DEVICE
  case DVC_CRYPTO:
    return new TWin_CryptoDevice();
#endif

#ifndef _CONSOLE
  case DVC_DISPLAY:
    return new TWinDisplay();

  case DVC_KEYBOARD:
    return new TWinKeyboard();

  case DVC_TOUCHSCREEN:
    return new TTouchScreenWin();
#endif

  default:
    return NULL;
  };
}
