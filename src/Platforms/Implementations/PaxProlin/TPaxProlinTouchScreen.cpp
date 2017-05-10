#include "Platforms/Implementations/PaxProlin/TPaxProlinTouchScreen.h"
#include "Base/Types/error.h"

#include "osal.h"
#include "fcntl.h"

#include <unistd.h>
#include "linux/input.h"

/*???
const unsigned short TOUCHPAD_X_VALUE_MIN = 280;
const unsigned short TOUCHPAD_X_VALUE_MAX = 3850;
const unsigned short TOUCHPAD_Y_VALUE_MIN = 140;
const unsigned short TOUCHPAD_Y_VALUE_MAX = 3900;
*/

const unsigned short TOUCHPAD_X_VALUE_MIN = 0;
const unsigned short TOUCHPAD_X_VALUE_MAX = 240;
const unsigned short TOUCHPAD_Y_VALUE_MIN = 0;
const unsigned short TOUCHPAD_Y_VALUE_MAX = 320;


#define MAX_UPOS_DISPLAY_WIDTH      240
#define MAX_UPOS_DISPLAY_HEIGHT     320


struct TouchPad
{
	TouchPad(){
		presence = 0;
		eventStream = -1;
		x = 0;
		y = 0;
		state = 0;
		last = 0;
	}
	unsigned short presence;
	int eventStream;
	unsigned int x;
	unsigned int y;
	long state;
	unsigned int last;
};

#define TOUCHSCREEN_NAME	"/dev/input/event2"

static struct TouchPad Tpad;

Error TPaxProlinTouchScreen::Open(){
	state = 0;
	xx = 0;
	yy = 0;

	Tpad.x = 0;
	Tpad.y = 0;
	Tpad.state = 0;
	if (Tpad.eventStream == -1)
	{
		Tpad.eventStream = open(TOUCHSCREEN_NAME, O_RDONLY|O_NONBLOCK);
	}
	if ( Tpad.eventStream != -1 )
	{
		Tpad.presence = 1;
		return Error(0ul);
	}
	return Error(-1ul);
}

void TPaxProlinTouchScreen::Close(){
	if (Tpad.eventStream != -1)
	{
		close(Tpad.eventStream);
		Tpad.eventStream = -1;
	}
	//return Error(0ul);
}

long pcc_GetMSec(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (unsigned long)((tv.tv_sec%1000000)*1000 + tv.tv_usec/1000);
}

void TPaxProlinTouchScreen::Update(){
	unsigned int lastx = 0, lasty = 0;
	unsigned char lastDetect = 0;
	long sys_ticks;

	if (Tpad.eventStream != -1)
	{
		struct input_event tcEvent;

		while( read(Tpad.eventStream, &tcEvent, sizeof(tcEvent)) == sizeof(tcEvent))
		{
			switch( tcEvent.type )
			{
			case 0:
				//empty message, change state of touchpad
				if( state != 0 )
				{
					Tpad.state = 1;
					Tpad.x = xx;
					Tpad.y = yy;
				}
				break;

			case 1:
				if ( tcEvent.code==330 )
				{
					if ( tcEvent.value == 0 )
					{
						//untouch (end of sequense)
						Tpad.state = 0;
						state = 0;
						sys_ticks = pcc_GetMSec();
						if ((sys_ticks - Tpad.last) < 400 )
						{
							lastx = Tpad.x;
							lasty = Tpad.y;
							lastDetect = 1;
						}
						Tpad.x = 0;
						Tpad.y = 0;
						xx = 0;
						yy = 0;
					}
					else
					{
						//touch (start new sequence)
						Tpad.state = 0;
						Tpad.x = 0;
						Tpad.y = 0;
						state = 0;
						xx = 0;
						yy = 0;
					}
				}
				break;

			case 3:
				switch( tcEvent.code )				//get coordinate
				{
				case 0://y - coodinate (up-max, done-min)
					state = 1;
					if ( tcEvent.value<TOUCHPAD_Y_VALUE_MIN ){
						yy = MAX_UPOS_DISPLAY_HEIGHT-1;
					}
					else if ( tcEvent.value>=TOUCHPAD_Y_VALUE_MAX ){
						yy = 0;
					}
					else{
						yy = (MAX_UPOS_DISPLAY_HEIGHT-1)-((tcEvent.value-TOUCHPAD_Y_VALUE_MIN)*MAX_UPOS_DISPLAY_HEIGHT/(TOUCHPAD_Y_VALUE_MAX-TOUCHPAD_Y_VALUE_MIN));
					}
					break;

				case 1://x - coordinate (left-min, right-max)
					state = 1;
					if ( tcEvent.value<TOUCHPAD_X_VALUE_MIN ){
						xx = 0;
					}
					else if ( tcEvent.value>=TOUCHPAD_X_VALUE_MAX ){
						xx = MAX_UPOS_DISPLAY_WIDTH-1;
					}
					else{
						xx = (tcEvent.value-TOUCHPAD_X_VALUE_MIN)*MAX_UPOS_DISPLAY_WIDTH/(TOUCHPAD_X_VALUE_MAX-TOUCHPAD_X_VALUE_MIN);
					}
					break;
				}
				break;
			}
		}

		if ( Tpad.state != 0 )
		{
			TouchPoint p;
			p.x = Tpad.x;
			p.y = Tpad.y;
			PutTouch(p);
		}
//		else if ( (lastDetect != 0) && ((lastx|lasty) != 0) ) //untouch, commented
//		{
//			TouchPoint p;
//			p.x = Tpad.x;
//			p.y = Tpad.y;
//			PutTouch(p);
//		}
	}
	Tpad.last = pcc_GetMSec();
}
