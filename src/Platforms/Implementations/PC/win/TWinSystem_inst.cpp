#include "Platforms/Implementations/Pc/win/TWinSystem.h"

ISystem* ISystem::Instance()
{
  static TWinSystem* _inst = 0;

  if(!_inst)
    _inst = new TWinSystem;

  return _inst;
}

