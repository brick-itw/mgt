#include "Platforms/Implementations/Verifon/TGraphicDisplay.h"
#include "Base/Types/inttypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <svctxo.h>
  #include <svc.h>
  #include <vos_ioctl.h>

//#define RGB565(r, g, b)	(COLOR) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

WORD RGB2PIXEL565(DWORD r, DWORD g, DWORD b)
{
  return (WORD)((((r) & 0xf8) << 8) |
                (((g) & 0xfc) << 3) |
                (((b) & 0xf8) >> 3));
}

WORD verColor(DWORD color)
{
  return RGB2PIXEL565((color      ) & 0x000000FFUL,
                      (color >>  8) & 0x000000FFUL,
                      (color >> 16) & 0x000000FFUL);
}

int TGraphicDisplay::OpenFrameBuffer(void)
{
	screenSize = 0;
	screenSize = ((SCREEN_WIDTH * SCREEN_HEIGHT)*2);

	gFb = (char*)malloc(screenSize);

	if (!gFb)
	  return -1;

	return 1;
}

void TGraphicDisplay::InitDisplay(void)
{
  if(bColorDisplay){
    resetdisplay("", 2);
    window(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT+2);
    set_display_coordinate_mode (PIXEL_MODE);
  }
  else{
    resetdisplay(DummyFont, 2);
    window(1, 1, 16, (SCREEN_HEIGHT+2)/2);
  }
}

/**
 * Close FrameBuffer.
 * [in] fd FrameBuffer handler
 * return 0 if successful, else return -1
 */
int TGraphicDisplay::CloseFrameBuffer()
{
  if(gFb){
    free(gFb);
	gFb = NULL;
  }
  return 1;
}

TGraphicDisplay::TGraphicDisplay() : IGraphicDisplay(){
	InitDisplay();
	bColorDisplay = TRUE;
}

TGraphicDisplay::~TGraphicDisplay(){
	CloseFrameBuffer();
}

//w - lcd width
//h - lcd height
void Coord(int gAngle, int w, int h, int x, int y, int *_x, int *_y)
{
	int angle = gAngle % 360;
	switch (angle) {
	case 0:
		*_x = x;
		*_y = y;
		break;
	case 90:
		*_x = h - y;
		*_y = x;
		break;
	case 180:
		*_x = w - x;
		*_y = h - y;
		break;
	case 270:
		*_x = y;
		*_y = w - x;
		break;
	}
}

void TGraphicDisplay::PaintCanvas(TCanvas *canvas){
	if(!canvas)
		return;

	rotation = 90;

	int pos_fb = 0;

	TPixel pix;
	char r=0,g=0,b=0;
	int fb_x = 0, fb_y = 0;
	for(int y = 0; y < canvas->GetHeight(); ++y)
	{
		for(int x=0; x<canvas->GetWidth(); ++x){
			pix = canvas->GetPixel(x, y);
			Coord(rotation, canvas->GetWidth(), canvas->GetHeight(), x, y, &fb_x, &fb_y);
			pos_fb = (fb_y * SCREEN_WIDTH + fb_x);
			r = 0xff & (pix >> 16);
			g = 0xff & (pix >> 8);
			b = 0xff & (pix);
            WORD pixel = RGB2PIXEL565(r,g,b);
			memcpy((char*)gFb + pos_fb, (unsigned char*)&pixel, 2);
		}
	}
	gotoxy(1,1);
    putpixelcol(gFb,screenSize);
}
