/*------------------------------------------------------------
* FileName: framebuffer.h
* Author: xlu
* Date: 2016-03-15
------------------------------------------------------------*/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "constant.h"

/**
 * Open FrameBuffer.
 * [in] deviceName device name for frame buffer.
 * return FrameBuffer handler
 */
int OpenFrameBuffer(const char *deviceName);

/**
 * Close FrameBuffer.
 * [in] fd FrameBuffer handler
 * return 0 if successful, else return -1
 */
int CloseFrameBuffer(int fd);

/**
 * Get frame buffer size (width and height).
 * [out] w width
 * [out] h height
 * return 0 if successful, else return -1
 */
int GetFrameBufferSize(int *w, int *h);

/**
 * Draw a rectangle
 * [in] x0 starting x coordinate, range [0, xres]
 * [in] y0 starting y coordinate, range [0, yres]
 * [in] x1 ending x coordinate, range [x0, xres]
 * [in] y1 ending y coordinate, range [y0, yres]
 * [in] color color to fill
 * return 0 if successful
 */
int DrawFrameBuffer(int x0, int y0, int x1, int y1, COLOR32 color);

/**
 * Start drawing a rectangle.
 * x coordinate is from bottom to top, and y coordinate is from left to right
 * [in] x0 starting x coordinate, range [0, xres]
 * [in] y0 starting y coordinate, range [0, yres
 * [in] x1 ending x coordinate, range [x0, xres]
 * [in] y1 ending y coordinate, range [y0, yres]
 * return 0 if successful
 */
int StartDrawFrameBuffer(int x0, int y0, int x1, int y1);

#endif
