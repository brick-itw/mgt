#include "Platforms/Implementations/PaxProlin/TPaxProlinKeyBoard.h"

#include "Platforms/Implementations/PaxProlin/ft/keyboard.h"
#include "Platforms/Implementations/PaxProlin/ft/header.h"
#include "Platforms/Implementations/PaxProlin/ft/constant.h"
#include "osal.h"

#include "UI/Keys.h"

Error TPaxProlinKeyBoard::Open(){
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
	upos_keys[102]   = UPOS_KEY_DOT;
	upos_keys[KEY_CANCEL] =    UPOS_KEY_ESCAPE;
	upos_keys[KEY_ENTER] =     UPOS_KEY_ENTER;
	upos_keys[KEY_BACKSPACE] = UPOS_KEY_BACKSPACE;
	upos_keys[KEY_MENU] =      UPOS_KEY_MENU;
	upos_keys[KEY_UP]   =      UPOS_KEY_UP;
	upos_keys[KEY_DOWN] =      UPOS_KEY_DOWN;

	gfd = open(KEYBOARD_NAME, O_RDWR);
	if(gfd >= 0)
		return true;
	return false;
}

void TPaxProlinKeyBoard::Close(){
	close(gfd);
}

BYTE TPaxProlinKeyBoard::GetKey(){
	int i = 0;
	int ret = 0;
	int eventNum = 0;
	struct input_event ev[64];
	struct pollfd events[1];
	int size = sizeof(struct input_event);
	int rd = 0;
	memset(events, 0, sizeof(events));
	events[0].fd = gfd;
	events[0].events = POLLIN | POLLERR;
	ret = poll(events, 1, 0);
	if ((ret <= 0) || (events[0].revents & POLLERR))
	{
		return -1;
	}
	if (events[0].revents & POLLIN)
	{
		memset(ev, 0, sizeof(ev));
		rd = read(gfd, ev, sizeof(ev));
		if (rd < size)
		{
			return -1;
		}
		eventNum = rd / size;
		for (i = 0; i < eventNum; ++i)
		{
			/* EV_KEY means type is key (not mouse, etc) */
			if (ev[i].type == EV_KEY && ev[i].value == 1)
			{
				return upos_keys[ev[i].code];
			}
		}
	}

	return 0;
}
