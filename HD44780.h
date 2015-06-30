/*
 * HD44780.h
 *
 * Created: 13-Jun-15 22:42:20
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License. 
 *
 */ 


#ifndef HD44780_H_
#define HD44780_H_

#include <stdint.h>

//Define MCU CPU Freq for the time delay functions
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LCDSetCursorPosition4(LCD,lineBaseAddress,position) LCDSendCommand4(LCD,lineBaseAddress+position)

//List of LCD commands
#define CLEAR_DISPLAY 0x01
#define RETURN_CURSOR_HOME 0x02

#define ENTRY_MODE 0x04
#define CURSOR_MOVE_DIRECTION_INC 0x02
#define DISPLAY_SHIFT 0x01

#define DISPLAY_OFF 0x08
#define DISPLAY_ON 0x0C
#define CURSOR_ON 0x0D
#define CURSOR_OFF 0x0C
#define BLINKING_CURSOR_ON 0x0F
#define BLINKING_CURSOR_OFF 0x0D

#define SHIFT_CONTENT_LEFT 0x18
#define SHIFT_CONTENT_RIGTH 0x1C
#define MOVE_CURSOR_LEFT 0x10
#define MOVE_CURSOR_RIGTH 0x14

#define FUNCTION_SET 0x20
#define FOUR_BIT_DATA_LENGTH 0x00
#define TWO_LINES_DISPLAY 0x08
#define FOUR_LINES_DISPLAY 0x08
#define SINGLE_LINE_DISPLAY 0x00
#define FONT_SIZE 0x04

#define SET_ONE_LINE_MODE 0x20
#define SET_TWO_LINE_MODE 0x28

/* 
   DDRAM of single line LCD in most of the cases
   is divided in two 8 bytes wide memory 
   segments. The starting addresses of 
   those segments are defined as:
	- LCD16x1_SELECT_DDRAM_1ST_HALF
	- LCD16x1_SELECT_DDRAM_2ND_HALF
   Those LCD modules although they have a single line,
   due to the DDRAM layout must be configured as
   two line modules.
*/
#define LCD16x1_SELECT_DDRAM_1ST_HALF 0x80
#define LCD16x1_SELECT_DDRAM_2ND_HALF 0xC0
#define LCD16x2_SELECT_LINE_1 0x80
#define LCD16x2_SELECT_LINE_2 0xC0

#define LCD16x4_SELECT_LINE_1 0x80
#define LCD16x4_SELECT_LINE_2 0xC0
#define LCD16x4_SELECT_LINE_3 0x90
#define LCD16x4_SELECT_LINE_4 0xD0

//LCD CGRAM address space
#define LCD_CGRAM_START_ADDRESS 0x40

//LCD DDRAM address space
#define LCD16x2_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD16x2_SECOND_LINE_DDRAM_START_ADDRESS 0x40
#define LCD16x4_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD16x4_SECOND_LINE_DDRAM_START_ADDRESS 0x40
#define LCD16x4_THIRD_LINE_DDRAM_START_ADDRESS 0x10
#define LCD16x4_FORTH_LINE_DDRAM_START_ADDRESS 0x50


typedef struct SHD44780
{
	volatile uint8_t *HD44780_CMD_PORT;
	volatile uint8_t *HD44780_DATA_PORT;
	uint8_t HD44780_RS;
	uint8_t HD44780_E;
	uint8_t HD44780_RW;
} TSHD44780;


static inline void delayloop16(uint16_t count)
{
	asm volatile (  "cp  %A0,__zero_reg__ \n\t"  \
	"cpc %B0,__zero_reg__ \n\t"  \
	"breq L_Exit_%=       \n\t"  \
	"L_LOOP_%=:           \n\t"  \
	"sbiw %0,1            \n\t"  \
	"brne L_LOOP_%=       \n\t"  \
	"L_Exit_%=:           \n\t"  \
	: "=w" (count)
	: "0"  (count)
	);
}
// delayloop16(x) eats 4 cycles per x
#define DELAY_US_CONV(us) ((uint16_t)(((((us)*1000L)/(1000000000/F_CPU))-1)/4))
#define delay_us(us)	  delayloop16(DELAY_US_CONV(us))

/* delay function for millisec
  (6 cycles per x + 20(?) overhead) */
void delayloop32( uint32_t l); // not inline
#define DELAY_MS_CONV(ms) ( (uint32_t) (ms*(F_CPU/6000L)) ) 
#define delay_ms(ms)  delayloop32(DELAY_MS_CONV(ms))


//HD44780 functions

//Delay loop function
void delayloop32(uint32_t loops);

// Init function for HD44789 
void LCDInit4(TSHD44780 *pHD44780,volatile uint8_t *HD44780_CMD_PORT,volatile uint8_t *HD44780_DATA_PORT,uint8_t HD44780_RS,uint8_t HD44780_E,uint8_t HD44780_RW,uint8_t lineNumbers);

//Send commands to the LCD over 4-but bus
void LCDSendCommand4(TSHD44780 *pHD44780,uint8_t Command2Send);

//Send character to the LCD
void LCDShowCharacter4(TSHD44780 *pHD44780,char Character2Show);

//Send string to the LCD
void LCDShowString4(TSHD44780 *pHD44780,char *String2Show);

//Define custom special characters
void LCDDefineSpecialChars4(TSHD44780 *pHD44780,uint8_t (*charMatrix)[8],uint8_t numberOfSpecChars);

#endif /* HD44780_H_ */
