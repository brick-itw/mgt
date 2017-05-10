#include "Common/Application/tsystem.h"
#include "Common/Loggers/TLoggerToFile.h"
#include "Common/Utils/FormatConverters.h"


TSystem::TSystem()
{
  pDefaultFilesystem = ISystem::Instance()->GetDefaultFileSystem();
  ISystem::Instance()->MountFileSystem("Trace", &pTraceFilesystem);
}

TSystem::~TSystem()
{
  if(pTraceFilesystem)
    ISystem::Instance()->UnmountFileSystem(pTraceFilesystem);
}

TSystem& TSystem::Instance()
{
  static TSystem sys;
  return sys;
}

void TSystem::LogData(const ILogEvent& event)
{
  DWORD dwDate = 0, dwTime = 0;
  ISystem::Instance()->GetDateTime(&dwDate, &dwTime);
  TLoggerToFile Logger(std::string("trace") + IntToStdString(dwDate, 8) + ".txt", pTraceFilesystem);
  Logger.LogData(event);
}


