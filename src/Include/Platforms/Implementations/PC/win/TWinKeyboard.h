#ifndef WINDOWS_KEYBOARD_H
#define WINDOWS_KEYBOARD_H

#include "Platforms/Implementations/TKeyboard.h"

class TWinKeyboard : public TKeyboard
{
public:
	virtual Error Open();
	virtual void  Close();
	virtual BYTE  GetKey();
};

#endif
