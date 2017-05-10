#include "Common/Loggers/TLoggerToFile.h"
#include "Platforms/Interfaces/ISystem.h"
#include "Common/Utils/FormatConverters.h"

#include <stdio.h>

void TLoggerToFile::TrimFile()
{
  if(MaxFileSize == 0)
    return;

  if(!pFileSystem)
    return;

  DWORD size = pFileSystem->GetFileLength(LogFileName.c_str());
  if(size < MaxFileSize)
    return;

  const char *temp_fname = "tmpflnm";
  pFileSystem->RenameFile(LogFileName.c_str(), temp_fname);
  IFileReadable* pOldFile = pFileSystem->OpenForRead(temp_fname);
  if(!pOldFile)
    return;
  IFileWritable* pNewFile = pFileSystem->OpenForWrite(LogFileName.c_str(), FO_APPEND);
  if(!pNewFile){
    delete pOldFile;
    return;
  }

  const int chunk_size = 1000;
  BYTE buf[chunk_size];
  DWORD offset = size - MaxFileSize / 2;
  pOldFile->Seek(offset);
  while(offset < size){
    DWORD bytes_left = size - offset;
    DWORD amount = chunk_size;
    if(bytes_left < chunk_size)
      amount = bytes_left;
    pOldFile->Read(buf, amount);
    pNewFile->Write(buf, amount);
    offset += amount;
  }

  delete pNewFile;
  delete pOldFile;
  pFileSystem->EraseFile(temp_fname);
}

void TLoggerToFile::LogData(const ILogEvent& event)
{
  char linebuf[129];

  if(!pFileSystem)
    return;

  TrimFile();

  DWORD dwDate = 0, dwTime = 0;
  ISystem::Instance()->GetDateTime(&dwDate, &dwTime);

  IFileWritable* pFile = pFileSystem->OpenForWrite(LogFileName.c_str(), FO_APPEND);
  if(!pFile)
    return;

  sprintf(linebuf, "%02lu.%02lu %02lu:%02lu:%02lu ",
                    dwDate%100UL,
                    dwDate/100UL%100UL,
                    dwTime/10000UL,
                    dwTime/100UL%100UL,
                    dwTime%100UL);
  pFile->Write((const BYTE*)linebuf, (DWORD)strlen(linebuf));

  std::string Marker(IntToStdString(event.GetEventCode(), 0));

  const TPrintableLogEvent* pEvt = dynamic_cast<const TPrintableLogEvent*>(&event);
  if(pEvt){
    Marker = pEvt->GetName();
    switch(pEvt->GetMode()){
      case LMT_ASCII:
        AsciiDataToFile(Marker, pEvt->GetData(), pFile);  
        break;
      case LMT_HEX:
        HexDataToFile(Marker, pEvt->GetData(), pFile);
        break;
      case LMT_DUMP:
        DumpDataToFile(Marker, pEvt->GetData(), pFile);
        break;
    }
  }
  else
    HexDataToFile(Marker, event.GetData(), pFile);
  delete pFile;
}

void TLoggerToFile::DumpDataToFile(const std::string& Marker, const T_BinaryData& data, IFileWritable* pFile)
{
  char linebuf[129];
  char* dest;

  DWORD len = data.Length();
  DWORD lim = DWORD((len >> 4) + DWORD((len & 0x000F) ? 1 : 0));
  DWORD m = 0;

  for(DWORD i=0; i < lim; i++){
    dest = linebuf;
    *dest++ = Marker[0];
    for(WORD k=0;k<16;k++){
      if(((i<<4)+k) < len)
        sprintf(dest+(k*3),"%02X ",data.GetData()[(i<<4)+k]);
      else
        memset(dest+(k*3),' ',3);
    }
    
    *(dest+(16*3-1)) = '|';

    for(WORD k=0;k<16;k++){
      if(((i<<4)+k) < len)
        *(dest+(16*3)+k) = (data.GetData()[(i<<4)+k] < 0x20) ? '.' : data.GetData()[(i<<4)+k];
      else
        *(dest+(16*3)+k) = ' ';
    }

    dest += 16*4;
    if(((i+1)<<4) < len){
      strcpy(dest,"\r\n");
      m = 14+1;
      memset(dest+2,' ',m);
      dest += 2+m;
    }
    
    pFile->Write((const BYTE*)linebuf, (DWORD)(dest-linebuf));
  }
}

void TLoggerToFile::HexDataToFile(const std::string& Marker, const T_BinaryData& data, IFileWritable* pFile)
{
  std::string s(Marker + " " + data.ToAscii() + "\r\n");
  pFile->Write((const BYTE*)s.c_str(), (DWORD)s.length());
}

void TLoggerToFile::AsciiDataToFile(const std::string& Marker, const T_BinaryData& data, IFileWritable* pFile)
{
  std::string s(Marker + " " + data.ToString() + "\r\n");
  pFile->Write((const BYTE*)s.c_str(), (DWORD)s.length());
}
