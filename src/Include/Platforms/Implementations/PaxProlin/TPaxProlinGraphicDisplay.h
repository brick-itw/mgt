#ifndef T_RENDER_LCD_PAX
#define T_RENDER_LCD_PAX

#include "UI/IGraphicDisplay.h"
#include "ft/framebuffer.h"
#include "ft/header.h"


class TPaxProlinGraphicDisplay : public IGraphicDisplay
{
public:
	TPaxProlinGraphicDisplay();
	~TPaxProlinGraphicDisplay();
	virtual void PaintCanvas(TCanvas *canvas);
	virtual Error Open(){return SUCCESS;}
	virtual void Close(){}
	virtual void  Backlight(BOOLEAN On){}
private:
	int fd;
	void *gFb;
	int rotation;//rotation in degrees
	struct fb_var_screeninfo gFbVarInfo;

	int OpenFrameBuffer(const char *deviceName);
	int CloseFrameBuffer(int fd);

};

#endif
