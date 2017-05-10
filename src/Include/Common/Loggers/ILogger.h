#ifndef _U2_LOGGER_H
#define _U2_LOGGER_H

#include "Base/Types/types.h"
#include "Common/Utils/T_BinaryData.h"
#include <string>

class ILogEvent
{
public:
  virtual DWORD GetEventCode() const = 0;
  virtual const T_BinaryData& GetData() const = 0;
};

class ILogger{
public:

  virtual void LogData(const ILogEvent& event) = 0;
};

#endif
