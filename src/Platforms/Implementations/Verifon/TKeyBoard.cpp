#include "Platforms/Implementations/Verifon/TKeyBoard.h"
/*
#include "Platforms/Implementations/PaxProlin/ft/keyboard.h"
#include "Platforms/Implementations/PaxProlin/ft/header.h"
#include "Platforms/Implementations/PaxProlin/ft/constant.h"
*/
#include "UI/Keys.h"

#define KEY_0           0xB0
#define KEY_1           0xB1
#define KEY_2           0xB2
#define KEY_3           0xB3
#define KEY_4           0xB4
#define KEY_5           0xB5
#define KEY_6           0xB6
#define KEY_7           0xB7
#define KEY_8           0xB8
#define KEY_9           0xB9
#define KEY_ENTER       0x8D
#define KEY_BACKSPACE   0x88
#define KEY_CANCEL      0x9B

Error TKeyBoard::Open(){
	upos_keys[KEY_0] = UPOS_KEY_0;
	upos_keys[KEY_1] = UPOS_KEY_1;
	upos_keys[KEY_2] = UPOS_KEY_2;
	upos_keys[KEY_3] = UPOS_KEY_3;
	upos_keys[KEY_4] = UPOS_KEY_4;
	upos_keys[KEY_5] = UPOS_KEY_5;
	upos_keys[KEY_6] = UPOS_KEY_6;
	upos_keys[KEY_7] = UPOS_KEY_7;
	upos_keys[KEY_8] = UPOS_KEY_8;
	upos_keys[KEY_9] = UPOS_KEY_9;
	upos_keys[KEY_CANCEL] = UPOS_KEY_ESCAPE;
	upos_keys[KEY_ENTER] = UPOS_KEY_ENTER;
	upos_keys[KEY_BACKSPACE] = UPOS_KEY_BACKSPACE;

	return true;
}

void TKeyBoard::Close(){
	
}

BYTE TKeyBoard::GetKey(){
  BYTE rByte, res;
  res = 0;

  if (read(STDIN, (char*)&rByte, 1)>0) {
    //debug_Print(0, "Key: 0x%02X\n", rByte);
    return upos_keys[rByte];
  }
  else
    return res;

}
