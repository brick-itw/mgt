#include "Platforms/Implementations/PaxProlin/TPaxProlinGraphicDisplay.h"
#include "Base/Types/inttypes.h"
#include "Common/Utils/FormatConverters.h"
#include <osal.h>

DWORD PaxColor(DWORD color)
{
  return RGB565(((color ) & 0x000000FFUL),
		        ((color >> 8) & 0x000000FFUL),
		        ((color >> 16) & 0x000000FFUL)) ;
}

int TPaxProlinGraphicDisplay::OpenFrameBuffer(const char *deviceName)
{
	int fd = 0;
	unsigned int screenSize = 0;

	fd = open(deviceName, O_RDWR);// open the FrameBuffer device.
	if (fd < 0)
		return -1;

	if (ioctl(fd, FBIOGET_VSCREENINFO, &gFbVarInfo) < 0)// get the variable screen info
		return -1;

	char buf[255];
	memset(buf, 0, sizeof(buf));
	int len =  OsRegGetValue("ro.fac.lcd.rotate", buf);
	rotation = 0;
	if(len)
	  rotation = StrToInt(buf, strlen(buf));

	screenSize = (gFbVarInfo.xres * gFbVarInfo.yres * gFbVarInfo.bits_per_pixel) >> 3;

	SetSizeX(gFbVarInfo.xres);
	SetSizeY(gFbVarInfo.yres);

	gFb = mmap(0, screenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (gFb == MAP_FAILED)
		return -1;

	return fd;
}

/**
 * Close FrameBuffer.
 * [in] fd FrameBuffer handler
 * return 0 if successful, else return -1
 */
int TPaxProlinGraphicDisplay::CloseFrameBuffer(int fd)
{
	int ret = munmap(gFb, (gFbVarInfo.xres * gFbVarInfo.yres * gFbVarInfo.bits_per_pixel) >> 3);
	close(fd);
	return ret;
}

TPaxProlinGraphicDisplay::TPaxProlinGraphicDisplay() : IGraphicDisplay(){
	fd = OpenFrameBuffer(LCD_NAME);
}

TPaxProlinGraphicDisplay::~TPaxProlinGraphicDisplay(){
	CloseFrameBuffer(fd);
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

void TPaxProlinGraphicDisplay::PaintCanvas(TCanvas *canvas){
	if(!canvas)
		return;

	int pos_fb = 0;
	int pos_canv = 0;
	int bytesPerPixel = gFbVarInfo.bits_per_pixel >> 3;
	TPixel pix;
	char r=0,g=0,b=0;
	int fb_x = 0, fb_y = 0;
	for(int y = 0; y < canvas->GetHeight(); ++y)
	{
		for(int x=0; x<canvas->GetWidth(); ++x){
			pix = canvas->GetPixel(x, y);

			fb_x = x;
			fb_y = y;
			if(rotation != 0)
			  Coord(rotation, canvas->GetWidth(), canvas->GetHeight(), x, y, &fb_x, &fb_y);

			pos_fb = (fb_y * gFbVarInfo.xres + fb_x)* bytesPerPixel;
			pos_canv = (y * canvas->GetWidth() + x) * 4;
			r = 0xff & (pix >> 16);
			g = 0xff & (pix >> 8);
			b = 0xff & (pix);
			WORD pax_pixel = RGB565(r, g, b);
			memcpy((char*)gFb + pos_fb, (unsigned char*)&pax_pixel, 2);
		}
	}
}
