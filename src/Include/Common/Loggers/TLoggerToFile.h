#ifndef _U2_FILE_LOGGER
#define _U2_FILE_LOGGER

#include "Common/Loggers/ILogger.h"
#include "Platforms/Interfaces/IFile.h"
#include "Platforms/Interfaces/IFileSystem.h"

#include <string>

typedef enum{
  LMT_ASCII,
  LMT_HEX,
  LMT_DUMP
}TLOGGINGMODE;

class TPrintableLogEvent: public ILogEvent{
protected:
  DWORD Code;
  T_BinaryData Data;
  TLOGGINGMODE Mode;
  std::string  Name;

public:

  TPrintableLogEvent(DWORD code,
                     const T_BinaryData& data = T_BinaryData(),
                     TLOGGINGMODE mode = LMT_ASCII,
                     const std::string& name = "") 
    : Code(code), Data(data), Mode(mode), Name(name)
  {};

  virtual DWORD GetEventCode() const {return Code;};
  virtual const T_BinaryData& GetData() const {return Data;};

  TLOGGINGMODE GetMode() const {return Mode;};
  std::string  GetName() const {return Name;};
};

class TNoDataPrintableEvent : public TPrintableLogEvent{
public: TNoDataPrintableEvent(DWORD code, const char* name) : TPrintableLogEvent(code, T_BinaryData(), LMT_ASCII, name){};
};

class TLoggerToFile : public ILogger {
protected:
  std::string   LogFileName;
  IFileSystem*  pFileSystem;
  DWORD         MaxFileSize;

  void DumpDataToFile  (const std::string& Marker, const T_BinaryData& data, IFileWritable* pFile);  
  void HexDataToFile   (const std::string& Marker, const T_BinaryData& data, IFileWritable* pFile);  
  void AsciiDataToFile (const std::string& Marker, const T_BinaryData& data, IFileWritable* pFile);  

  void TrimFile(); //check file size and if it exceeds max size, trim it

public:
  virtual void LogData(const ILogEvent& event);

  TLoggerToFile(const std::string& logName, 
                IFileSystem* fileSystem, DWORD max_size_bytes_ = 0UL) //set max_size_bytes_ to 0, for unlimited size of log
  {
    MaxFileSize = max_size_bytes_;
    LogFileName = logName;
    pFileSystem = fileSystem;
  };
};

#endif
