#ifndef _MGT_SYSTEM_H
#define _MGT_SYSTEM_H

#include "Platforms/Interfaces/ISystem.h"
#include "Common/Loggers/ILogger.h"

class TSystem{
protected:
  IFileSystem* pDefaultFilesystem;
  IFileSystem* pTraceFilesystem;

  TSystem();
  ~TSystem();

public:

  static TSystem& Instance();

  void LogData(const ILogEvent& event);
};

#endif
