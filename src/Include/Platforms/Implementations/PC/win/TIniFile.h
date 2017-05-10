#ifndef _U2_INI_FILE_H
#define _U2_INI_FILE_H

#include <string>

class TIniFile{
protected:
  std::string FileName;  

public:
  TIniFile(const std::string& fname);
  std::string GetString(const std::string& section, const std::string& entry);
  std::string GetIndexString(const std::string& section, const std::string& entry, int index);
};

#endif
