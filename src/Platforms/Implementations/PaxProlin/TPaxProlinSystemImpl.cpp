#include "Platforms/Implementations/PaxProlin/TPaxProlinSystem.h"

ISystem* ISystem::Instance()
{
  static TPaxProlinSystem* _inst = 0;

  if(!_inst)
    _inst = new TPaxProlinSystem;

  return _inst;
}
