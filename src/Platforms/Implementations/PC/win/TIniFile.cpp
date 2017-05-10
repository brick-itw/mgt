#include "Platforms/Implementations/PC/Win/TIniFile.h"
#include <windows.h>

TIniFile::TIniFile(const std::string& fname)
{
  char fn[260];
  GetCurrentDirectory(sizeof(fn), fn);
  FileName = std::string(fn) + "\\" + fname;  
}

std::string TIniFile::GetString(const std::string& section, const std::string& entry)
{
  char buf[1024];
  GetPrivateProfileString(section.c_str(), entry.c_str(), "", buf, sizeof(buf), FileName.c_str());
  return std::string(buf);
}

std::string TIniFile::GetIndexString(const std::string& section, const std::string& entry, int index)
{
  char buf[1024];
  GetPrivateProfileString(section.c_str(), entry.c_str(), "", buf, sizeof(buf), FileName.c_str());

  char* p = strtok(buf, ",\r\n\t;");
  for(int i=0;(i<index) && p;++i)
    p = strtok(NULL, ",\r\n\t;"); 
  
  return std::string(p);
}

