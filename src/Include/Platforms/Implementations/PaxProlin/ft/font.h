/*------------------------------------------------------------
* FileName: font.h
* Author: xlu
* Date: 2016-03-15
------------------------------------------------------------*/
#ifndef FONT_H
#define FONT_H

#include "constant.h"

/**
 * Open font.
 * [in] filename font file name, such as "paxfont.ttf"
 * return font handle(>=0)
 */
int OpenFont(const char *filename);

/**
 * Close font.
 */
void CloseFont();

/**
 * Get displaying length of a string.
 * If you want to shown a string in the center horizontally of the LCD,
 * just set starting x position: (screen_width - GetStringLength()) / 2
 * [in] ftDots dots array of the string
 * [in] ftDotsNum element number of dots array
 * [in] height initialized height of the string
 * return displaying length if successful, else return -1
 */
int GetStringLength(const char* str, int height);

/**
 * Draw string.
 * [in] str string to draw
 * [in] height initialized height
 * [in] x starting x coordinate
 * [in] y starting y coordinate
 * [in] color color to draw
 * return 0 if successful, else return -1
 */
int DrawString(const char *str, int height, int x, int y, COLOR32 color);

#endif
