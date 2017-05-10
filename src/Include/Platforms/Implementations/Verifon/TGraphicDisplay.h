#ifndef T_RENDER_LCD_PAX
#define T_RENDER_LCD_PAX

#include "UI/IGraphicDisplay.h"

#define SCREEN_WIDTH   240
#define SCREEN_HEIGHT  320 
const char DummyFont[]        = "F:dummy.fon";       // dummy graphics font

class TGraphicDisplay : public IGraphicDisplay
{
public:
	TGraphicDisplay();
	~TGraphicDisplay();
	virtual void PaintCanvas(TCanvas *canvas);
	virtual Error Open(){return 0;}
	virtual void Close(){}
	virtual void  Backlight(BOOLEAN On){}
private:
    unsigned int screenSize;
	BYTE bColorDisplay;
	int fd;
	char *gFb;
	//struct fb_var_screeninfo gFbVarInfo;

	void InitDisplay(void);
	int OpenFrameBuffer(void);
	int CloseFrameBuffer(void);

};

#endif
