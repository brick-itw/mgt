/*------------------------------------------------------------
* FileName: lcd.h
* Author: xlu
* Date: 2016-03-15
------------------------------------------------------------*/
#ifndef LCD_H
#define LCD_H

#include "constant.h"

/**
 * Open LCD.
 * return 0 if successful, else return -1
 */
int OpenLcd();

/**
 * Close LCD.
 * return 0 if successful, else return -1
 */
int CloseLcd();

/**
 * Draw LCD rectangle.
 * [in] x0 starting x coordinate, range [0, w]
 * [in] y0 starting y coordinate, range [0, h]
 * [in] x1 ending x coordinate, range [x0, w]
 * [in] y1 ending y coordinate, range [y0, h]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawLcd(int x0, int y0, int x1, int y1, COLOR32 color);

/**
 * Start draw LCD rectangle.
 * [in] x0 starting x coordinate, range [0, w]
 * [in] y0 starting y coordinate, range [0, h]
 * [in] x1 ending x coordinate, range [x0, w]
 * [in] y1 ending y coordinate, range [y0, h]
 * return 0 if successful
 */
int StartDrawLcd(int x0, int y0, int x1, int y1);

/**
 * Draw a point.
 * [in] x x coordinate, range [0, xres]
 * [in] y y coordinate, range [0, yres]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawPoint(int x, int y, COLOR32 color);

/**
 * Get LCD size.
 * Call "OpenLcd" and "setLcdRotation" first before use this function
 * [out] w LCD width
 * [out] h LCD height
 * return 0 if successful
 */
int GetLcdSize(int* w, int* h);

/**
 * Set LCD rotation angle.
 * [in] angle the rotation, can be 0, 90, 180, 270, ...
 */
void setLcdRotation(int angle);

/**
 * Change LCD coordinate to frame buffer coordinate.
 * [in] x x coordinate in LCD
 * [in] y y coordinate in LCD
 * [out] _x x coordinate in frame buffer
 * [out] _y y coordinate in frame buffer
 */
void Lcd2Fb(int x, int y, int *_x, int *_y);

#endif
