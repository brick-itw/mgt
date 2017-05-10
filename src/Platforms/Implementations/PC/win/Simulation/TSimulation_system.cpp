
#include "Platforms/Implementations/Pc/Win/Simulation/TSimulation_System.h"
#include "Platforms/Implementations/Pc/Win/Simulation/TSimulation_Cardreader.h"
#include "Platforms/Implementations/Pc/Win/Simulation/TSimulation_Keyboard.h"
#include "Platforms/Implementations/Pc/Win/TIniFile.h"
#include "Common/Utils/FormatConverters.h"
#include "Common/Parameters/TRefManager.h"

#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>

#include <stack>

//static TSimulation_System* _pSS = NULL;
std::stack<TSimulation_System*> TestSystemInstances;

const char* cTestName = "Test";

IDevice* TSimulation_DeviceLayer::CreateDevice(DEVICE deviceType, unsigned deviceIndex)
{
  std::string script_name;

  switch(deviceType)
  {
  case DVC_CARD_READER:
    return new TEval_CardReader(std::string("sc_scripts\\") + TIniFile(TestFileName).GetString(cTestName, "CardExchange")); 

  case DVC_KEYBOARD:
    return new TSimulation_Keyboard(TIniFile(TestFileName).GetString(cTestName, "KeyboardInput"));

  case DVC_DISPLAY:
  case DVC_TOUCHSCREEN:
    return NULL;

  default: 
    return TWinDeviceDebugLayer::CreateDevice(deviceType, deviceIndex);
  };
}

TSimulation_System::TSimulation_System(const std::string& testname) : TWinSystem(), TestFileName(testname), ConfigPath()
{
  std::string conf_file = TIniFile(testname).GetString(cTestName, "Config");
  if(conf_file.length())
  {
    ConfigPath = std::string("EmvTests\\Configs\\") + conf_file;
    TRefManager::SetConfigPath(ConfigPath);
  }
  TestSystemInstances.push(this);
}

TSimulation_System::~TSimulation_System()
{
  TestSystemInstances.pop();

  if(TestSystemInstances.size())
  {
    TSimulation_System* pPrevSystem = TestSystemInstances.top();
    TRefManager::SetConfigPath(pPrevSystem->GetConfigPath());
  }
}

Error TSimulation_System::GetDateTime(DWORD* pDate, DWORD* pTime)
{
  TIniFile Ini(TestFileName);
  std::string date = Ini.GetString(cTestName,"StartDate");
  std::string time = Ini.GetString(cTestName,"StartTime");

  SYSTEMTIME dt;
  GetLocalTime(&dt);

  if(pDate)
  {
    if(date.length() == 6)
      *pDate = 20000000 + IntFromStdString(date);
    else
      *pDate = dt.wYear * 10000L +dt.wMonth*100 + dt.wDay;
  }
  if(pTime)
  {
    if(time.length() == 6)
      *pTime = IntFromStdString(time);
    else
      *pTime = dt.wHour * 10000L +dt.wMinute*100+dt.wSecond; 
  }
  return SUCCESS;
}

void TSimulation_System::CreateDeviceLayer()
{
  pDeviceLayer = new TSimulation_DeviceLayer(TestFileName);
}

Error TSimulation_System::GenerateRandom(BYTE* pData8)
{
  TIniFile Ini(TestFileName);
  for(;;){
    std::string rnd = Ini.GetIndexString(cTestName, "Random", CurrentRandomIndex+1);
    if(rnd.length() >= 16)
    {
      ++CurrentRandomIndex;
      return HexToBytes(rnd.c_str(), pData8, 16) ? ERROR_NO_SUPPORT : SUCCESS;
    }

    if(!CurrentRandomIndex)
      return ERROR_NO_SUPPORT;

    CurrentRandomIndex = 0;
  }
  return ERROR_NO_SUPPORT;
}

ISystem* ISystem::Instance()
{
  /*
  static TSimulation_System* _inst = 0;

  if(!_inst)
    _inst = new TSimulation_System("EmvTests/Tests/Test1.txt");

  return _inst;
  */
  TSimulation_System* _pSS = TestSystemInstances.size() ? TestSystemInstances.top() : NULL;

  _ASSERT_EXPR(_pSS, L"TSimulation_System object not defined");

  return _pSS;
}
