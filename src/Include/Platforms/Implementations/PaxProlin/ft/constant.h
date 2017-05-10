/*------------------------------------------------------------
* FileName: constant.h
* Author: xlu
* Date: 2016-03-14
------------------------------------------------------------*/
#ifndef CONSTANT_H
#define CONSTANT_H

/**
 * Frequently used constant.
 */
/*
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
*/


#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })




/**
 * Basic data type.
 */
/*
#ifndef BYTE
#define BYTE	unsigned char
#endif

#ifndef WORD
#define WORD	unsigned short
#endif

#ifndef DWORD
#define DWORD	unsigned int
#endif
*/
/**
 * RGB 888.
 */
// the r, g, b mask of RGB888 color is 0xFF0000, 0xFF00, 0xFF
#define R_RGB888(x)		(unsigned char) (((x) >> 16) & 0xFF)
#define G_RGB888(x)		(unsigned char) (((x) >> 8) & 0xFF)
#define B_RGB888(x)		(unsigned char) ((x) & 0xFF)

// Generating RGB565 pixel by composing r, g, b components
#ifndef RGB888
#define RGB888(r, g ,b)  ((unsigned int) (((unsigned int) (r) << 16| ((unsigned short) (g) << 8)) | ((unsigned int) (unsigned char) (b))))
#endif

#ifndef COLOR32
#define COLOR32	unsigned int
#endif

#define WHITE	RGB888(0xff, 0xff, 0xff)
#define BLACK	RGB888(0x00, 0x00, 0x00)
#define RED		RGB888(0xff, 0, 0)
#define GREEN	RGB888(0, 0xff, 0)
#define BLUE	RGB888(0, 0, 0xff)
#define GRAY	RGB888(0xbe, 0xbe, 0xbe)

/**
 * RGB 565.
 */
// the r, g, b value of RGB565 color is 0xF800, 0x7E0, 0x1F
// R R R R    R G G G    G G G B    B B B B
#define R_RGB565(x)		(unsigned char) (((x) >> 8) & 0xF8)
#define G_RGB565(x)		(unsigned char) (((x) >> 3) & 0xFC)
#define B_RGB565(x)		(unsigned char) ((x) << 3)

// Generating RGB565 pixel by composing r, g, b components.
#ifndef RGB565
#define RGB565(r, g, b)	(unsigned short) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#endif

#ifndef COLOR16
#define COLOR16	unsigned short
#endif

// Convert 32-bit color (RGB888) to 16-bit color (RGB565).
#define COLOR_32_TO_16(x) RGB565(R_RGB888(x), G_RGB888(x), B_RGB888(x))


#define FONT_NAME			"paxfont.ttf"
#define KEYBOARD_NAME		"/dev/keypad"
#define TOUCHSCREEN_NAME	"/dev/tp"
#define LCD_NAME			"/dev/fb"

#endif
