/*------------------------------------------------------------
* FileName: keyboard.h
* Author: xlu
* Date: 2016-03-14
------------------------------------------------------------*/
#ifndef KEYBOARD_H
#define KEYBOARD_H

/**
 * Open keyboard.
 * [in] filename keyboard file name, such as "/dev/keypad"
 * return keyboard device(>=0)
 */
int OpenKeyboard(const char* filename);

/**
 * Close keyboard.
 * return 0 if closing keyboard successfully, else return -1
 */
int CloseKeyboard();

/**
 * Get key value (>=0), see macro in linux/input.h, such as KEY_1, KEY_CANCEL, etc.
 * return key value
 */
int GetKey();

#endif
