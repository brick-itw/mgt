#include <osal.h>
#include "Platforms/Implementations/PaxProlin/TPaxProlinGraphPrinter.h"
#include "Common/Utils/FormatConverters.h"
#include "Common/Utils/ByteOrder.h"
#include "UI/TMonochromeImage.h"

Error TPaxProlinGraphPrinter::StartPrinting(TCanvas& canvas){
	WORD width = canvas.GetWidth();
	WORD height = canvas.GetHeight();

	TMonochromeImage image(canvas.GetWidth(), canvas.GetHeight());

	int y_offset = 0;
	char print_buf[20000];

	for(int y=0; y<height; y++){
		for(int x=0; x<width; ++x){
			if(canvas.GetPixel(x, y) > 0)//inverse color
				image.SetPixel(x, y, 0x00);
			else
				image.SetPixel(x, y, 0xff);
		}
	}

	while(y_offset < height){
		int delta = 100;
		if((height - y_offset) < 100)
			delta = height - y_offset;

		print_buf[0] = delta;

		for(int y=0; y<delta; y++){
			WORD_TO_MSB(width/8, (BYTE*)&print_buf[1+y*(width/8+2)]);
			memmove((BYTE*)&print_buf[1+y*(width/8+2)+2], image.GetDataPtr() + (y_offset+y)*width/8, width/8);
		}

		OsPrnPutImage((unsigned char*)print_buf);

		y_offset += delta;
	}
	int res = OsPrnStart();
	return PaxErrorCodeToUposErrorCode(res);
}

Error TPaxProlinGraphPrinter::Open(){
	//int res = OsPrnOpen(PRN_REAL, REAL_PRN_NAME);
  int res = OsPrnOpen(PRN_REAL, NULL);
	//OsPrnSelectFontSize(24, 48, 48, 48);
	return PaxErrorCodeToUposErrorCode(res);
}

void TPaxProlinGraphPrinter::Close(){
	OsPrnClose();
}

Error TPaxProlinGraphPrinter::PaxErrorCodeToUposErrorCode(int code){
  switch(code){
  case RET_OK:                return SUCCESS;
  case ERR_DEV_NOT_EXIST:     return ERROR_PRN_NO_PRINTER;
  case ERR_PRN_BUSY:          return ERROR_PRN_BUSY;
  case ERR_PRN_PAPEROUT:      return ERROR_PRN_PAPEROUT;
  case ERR_PRN_WRONG_PACKAGE: return ERROR_PRN_WRONG_PACKAGE;
  case ERR_PRN_OVERHEAT:      return ERROR_PRN_OVERHEAT;
  case ERR_PRN_OUTOFMEMORY:   return ERROR_PRN_OUTOFMEMORY;
  case ERR_INVALID_PARAM:     return ERROR_PRN_INVALID_PARAM;
  default:                    return ERROR_NO_SUPPORT;
  };
  return ERROR_NO_SUPPORT;
}

