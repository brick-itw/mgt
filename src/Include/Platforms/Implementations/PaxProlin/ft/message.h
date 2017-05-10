/*
 * message.h
 *
 *  Created on: 07.11.2016
 *      Author: sbt-gavriluk-uv
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "osal.h"
#include "ft/font.h"
#include "ft/header.h"
#include "ft/lcd.h"
#include "ft/keyboard.h"

int Time2Str(ST_TIME* time, char* str);
int Finish();
int MessageOnScreen(const char* str, char *str1="");
int WaitKey();



#endif /* MESSAGE_H_ */
