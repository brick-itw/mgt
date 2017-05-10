#ifndef _U2_SIMULATION_KEYBOARD_H
#define _U2_SIMULATION_KEYBOARD_H

#include "Platforms/Implementations/TKeyboard.h"
#include <string>

class TSimulation_Keyboard : public TKeyboard
{
protected:
  int Index;
  bool SkipKeyFlag;
  std::string SimulatedKeystrokes;

  std::string GetNextKeystroke();

public:
  TSimulation_Keyboard(const std::string keys) : SimulatedKeystrokes(keys), Index(0), SkipKeyFlag(true)
  {};

	virtual Error Open();
	virtual void  Close();
	virtual BYTE  GetKey();
};

#endif
