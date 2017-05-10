#include "Platforms/Implementations/PC/Win/Simulation/TSimulation_Keyboard.h"
#include "UI/Keys.h"

std::string TSimulation_Keyboard::GetNextKeystroke()
{
  if(Index == std::string::npos)
    return std::string();

  size_t nextIdx = SimulatedKeystrokes.find_first_not_of(", ", Index);
  if(nextIdx == std::string::npos)
    return std::string();

  Index = SimulatedKeystrokes.find_first_of(", ", nextIdx);
  return SimulatedKeystrokes.substr(nextIdx, Index);
}

BYTE TSimulation_Keyboard::GetKey()
{
  BYTE key = 0;
  if(!SkipKeyFlag)
  {
    std::string strkey = GetNextKeystroke();
    if(strkey == "ENTER")
      key = UPOS_KEY_ENTER;
    else
    if(strkey == "ESCAPE")
      key = UPOS_KEY_ESCAPE;
    else
    if(strkey == "BACKSPACE")
      key = UPOS_KEY_BACKSPACE;
    else
    if(strkey == "UP")
      key = UPOS_KEY_UP;
    else
    if(strkey == "DOWN")
      key = UPOS_KEY_DOWN;
    else
    if(strkey.substr(0, 4) == "WAIT")
      ; // @TODO!!
    else
    if(strkey.length())
      key = strkey[0];
  }
  SkipKeyFlag = !SkipKeyFlag;
  return key;
}


Error TSimulation_Keyboard::Open()
{
  return SUCCESS;
}

void TSimulation_Keyboard::Close()
{
}
