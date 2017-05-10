/**
 * @Author aakhartov.sbt@sberbank.ru
 * @date   August 2016
 * @brief  Error class implementation
 */

#ifndef _UPOS_ERROR_H
#define _UPOS_ERROR_H

#include "Base/Types/errors.h"

#ifndef NO_EMULATE_ATOMIC
inline unsigned long atomic_decrement(unsigned long& value)
{
  return --value;
}

inline unsigned long atomic_increment(unsigned long& value)
{
  return ++value;
}
#endif

class ErrorData
{
  friend class Error;

  unsigned long RefCount;
  union {
    unsigned long Code;
    struct {
      unsigned short Module;
      unsigned short CodeLow;
    };
  };
  ErrorData* SubData;

public:

  ErrorData(unsigned long error = ERROR_UNKNOWN)
  {
    RefCount = 1;
    Code = error;
    SubData = 0;
  }

  // Makes Error Data copy without suberror
  ErrorData* MakeCopy() const
  {
    ErrorData* data = new ErrorData(Code);
    data->RefCount = 1;
    data->SubData = 0;
    return data;
  }
};

class Error
{ 
  ErrorData* Data;

  inline void DeleteData(ErrorData* data)
  {
    while (data != 0)
    {
      if (atomic_decrement(data->RefCount) != 0)
        break;
      ErrorData* next = data->SubData;
      delete data;
      data = next;
    }
  }
public:
  Error(unsigned long error)
    : Data(new ErrorData(error))
  {
  }

  inline ~Error()
  {
    DeleteData(Data);
  }

  inline Error(const Error& other) :
    Data(other.Data)
  {
    if (Data)
      atomic_increment(Data->RefCount);
  }

  inline explicit Error(ErrorData* data) :
    Data(data)
  {
    if (data)
      atomic_increment(data->RefCount);
  }

  inline Error& operator=(const Error& other)
  {
    ErrorData* data = other.Data;
    if (data)
      atomic_increment(data->RefCount);
    DeleteData(Data);
    Data = data;
    return *this;
  }

  inline unsigned long Code() const
  {
    return Data ? Data->Code : 0;
  }

  operator unsigned long() const
  {
    return Code();
  }

  inline Error GetSubError() const
  {
    return Data? Error(Data->SubData) : Error(0UL);
  }

  inline Error FindSubError(unsigned long code) const
  {
    ErrorData* data = Data;
    while (data != 0 && data->Code != code)
      data = data->SubData;
    return Error(data);
  }

  inline Error& AddSubError(const Error& other)
  {
    ErrorData* suberror = other.Data;
    if (suberror)
      atomic_increment(suberror->RefCount);
    if (Data == 0)                // if current error empty replace by suberror
      Data = suberror;
    else if (Data->RefCount == 1) // replace suberror by new suberror
    {
      DeleteData(Data->SubData);
      Data->SubData = suberror;
    }
    else
    {
      // if current error has copies we cannot change them. let's create new error, replace current and add suberror
      ErrorData* newData = Data->MakeCopy();
      if (newData == 0)
        DeleteData(suberror);
      else
      {
        newData->SubData = suberror;
        DeleteData(Data);
        Data = newData;
      }
    }
    return *this;
  }
};

#endif // _UPOS_ERROR_H
