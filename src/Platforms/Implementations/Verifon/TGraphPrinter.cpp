#include "Platforms/Implementations/Verifon/TGraphPrinter.h"
#include "Common/Utils/FormatConverters.h"
#include "UI/TMonochromeImage.h"

#include <printer.h>

Error TGraphPrinter::StartPrinting(TCanvas& canvas){
	BYTE* p = NULL;
	WORD width = canvas.GetWidth();
	WORD height = canvas.GetHeight();

	TMonochromeImage image(canvas.GetWidth(), canvas.GetHeight());


	for(int y=0; y<height; y++){
		for(int x=0; x<width; ++x){
			if(canvas.GetPixel(x, y) > 0)//inverse color
				image.SetPixel(x, y, 0x00);
			else
				image.SetPixel(x, y, 0xff);
		}
	}

    Print((BYTE*)image.GetDataPtr(), height, width);
	return 0;
}

Error TGraphPrinter::Open(){

  if (ValidHandle(HandlePrn))
    return ERR_OK;

  if ( !ValidHandle(HandlePrn = open("/dev/com4", 0)) )
    return DRV_FAILURE;

  return ERR_OK;

}

void TGraphPrinter::Close(){

  char buffer[4];
  if (ValidHandle(HandlePrn)){
    while ( get_port_status( HandlePrn, buffer) > 0 ){
      SVC_WAIT(1);
    }
    close(HandlePrn);
    HandlePrn=DRV_NO_HANDLE;
  }

}
BYTE TGraphPrinter::ValidHandle(int iHandle)
{
  return (BYTE)(iHandle != -1);
}
void TGraphPrinter::WaitPrinterReady(void)
{
  char  bf[4];
  int res;

  for(;;){

    res = get_port_status(HandlePrn, bf);

    if(res <= 0)
      break;

    if(bf[2] > 16)
      break;

      SVC_WAIT(50);    
  }

}


BYTE TGraphPrinter::Print(BYTE* pData, DWORD height, DWORD width)
{
  DWORD i, k;
  BYTE  cTerminator;
  char  pBuf4[70];
  int   BufCnt, delta;
  BYTE* p;
  DWORD CorWidth;
  #define MAX_BMP_WIDTH 48

  if (!ValidHandle(HandlePrn))
    return DRV_FAILURE;

  width /=  8;
  if((CorWidth = width) > MAX_BMP_WIDTH)
    CorWidth = MAX_BMP_WIDTH ;

  for(i=0;i<height;i++){

    if((i % 12) == 0)
      WaitPrinterReady();

    delta  = 0;
    BufCnt = 0;
    if(!i){
      delta = 2;
      pBuf4[BufCnt++] = 0x1B;
      pBuf4[BufCnt++] = 'g';
    }

    for(k=0;k<CorWidth;k+=3){
      // convert 3 bytes into 4 6-bit characters
      memset(pBuf4+BufCnt, 0x40, 4);
      p = pData + i*width + k;
      pBuf4[BufCnt++] |=  (p[0] >> 2) & 0x3F;
      if((k+1)<width)
        pBuf4[BufCnt++] |=  ((p[0] & 0x03) << 4) | ((p[1] >> 4) & 0x0F);
      if((k+2)<width){
        pBuf4[BufCnt++] |=  ((p[1] & 0x0F) << 2) | ((p[2] >> 6) & 0x03);
        pBuf4[BufCnt++] |=  p[2] & 0x3F;
      }
    }

    cTerminator = 0x20;
    if((i+1) == height)
      cTerminator |= 0x08;

    if(BufCnt < (64+delta))
      pBuf4[BufCnt++] = cTerminator;

    if((BufCnt >= (64+delta)) && ((i+1) == height))
      pBuf4[BufCnt++] = cTerminator;

    write(HandlePrn, pBuf4, BufCnt);
  }


  return ERR_OK;
}


