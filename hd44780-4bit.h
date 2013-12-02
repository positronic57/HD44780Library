/*
*
* AVR-libc library for a LCD with HD44780 display controller for AVR 8-bit microcontrollers.
* 
* Copyright (c) 2013 Goce Boshkovski
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License.
*
*/

#include <stdlib.h>
#include <avr/io.h>
#include "conf.h"

#ifndef F_CPU
#define F_CPU 4000000UL
#endif

#ifndef HD44780_CMD_PORT
#define HD44780_CMD_PORT D
#endif

#ifndef HD44780_RS
#define HD44780_RS PORT4
#endif

#ifndef HD44780_E
#define HD44780_E PORT7
#endif

#ifndef HD44780_RW
#define HD44780_RW PORT6
#endif

#ifndef HD44780_DATA_PORT
#define HD44780_DATA_PORT B
#endif

#include "delay.h"

#define GLUE(a,b) a##b
#define PORT(x) GLUE(PORT,x)
#define DDR(x) GLUE(DDR,x)
#define CMD_PORT PORT(HD44780_CMD_PORT)
#define CMD_DDR	DDR(HD44780_CMD_PORT)
#define DATA_PORT PORT(HD44780_DATA_PORT)
#define DATA_DDR DDR(HD44780_DATA_PORT)

// List od LCD commands
#define CLEAR_DISPLAY 0x01
#define RETURN_CURSOR_HOME 0x02
#define TURN_OFF 0x08
#define SHIFT_CONTENT_LEFT 0x18
#define SHIFT_CONTENT_RIGTH 0x1C
#define MOVE_CURSOR_LEFT 0x10
#define MOVE_CURSOR_RIGTH 0x14
#define SET_ONE_LINE_MODE 0x20
#define SET_TWO_LINE_MODE 0x28
#define NEW_LINE 0xC0
#define SELECT_LINE_1 0x80
#define SELECT_LINE_2 0xC0

// LCD address space
#define FIRST_LINE_DDRAM_START_ADDRESS 0x80
#define SECOND_LINE_DDRAM_START_ADDRESS 0xC0


// Init function for LCD and 4-bit bus
extern void LCDInit4();

//Send commands to the LCD over 4-but bus
extern void LCDSendCommand4(uint8_t );

//Send character to the LCD
extern void LCDShowCharacter4(char );

//Time dalay function
extern void LCDDelay(uint8_t );

//Send string to the LCD
extern void LCDShowString4(char *);

//Set cursor position on the LCD
extern void LCDSetCursorPosition4(uint8_t, uint8_t);
