#ifndef KEYBOARD_PAX_H
#define KEYBOARD_PAX_H

#include "Platforms/Implementations/TKeyboard.h"

class TPaxProlinKeyBoard : public TKeyboard
{
public:
	virtual Error Open();
	virtual void Close();
	virtual BYTE GetKey();

private:
	int gfd;
};

#endif
