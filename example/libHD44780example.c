/*
 * libHD44780.c
 *
 * Created: 13-Jun-15 13:58:22
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Example of using HD44780 library on Atmega32. The CPU freq is 16MHz 
 * defined with F_CPU.
 * 
 * JHD 162A 16x2 LCD module is used for the test.
 * 
 * Connection between MCU and LCD:
 * PORTB0 -> LCD D4
 * PORTB1 -> LCD D5
 * PORTB2 -> LCD D6
 * PORTB3 -> LCD D7
 * PORTD4 -> LCD RS
 * PORTD6 -> LCD R/W
 * PORTD7 -> LCD E
 * 
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "HD44780.h"


int main(void)
{
    
	// Define custom special characters. 
        // In this example those are the letters from the Macedonian alphabet for "d" and "sh".
	uint8_t specCharMatrix[2][8]={ 
		{ 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x1F, 0x00 },
		{ 0x0E, 0x0A, 0x0A, 0x0A, 0x0A, 0x1F, 0x11, 0x00 }
	};
								
	TSHD44780 HD44780Display;
	
	/*
		Use:
		- TWO_LINES_DISPLAY to init 2 lines module;
		- FOUR_LINES_DISPLAY to init 4 lines LCD.		
		
		Although single line LCD has only one line for presenting the content,
		the DDRAM memory in most of the modules out there is split in two 8 bytes long buffers.
		Those single line modules must be initialized using TWO_LINES_DISPLAY
		as argument of the init function. If that is not the case use 
		SINGLE_LINE_DISPLAY during the init process.

	*/
	LCDInit4(&HD44780Display, &PORTD, &PORTB, PD4, PD7, PD6, TWO_LINES_DISPLAY);
	LCDDefineSpecialChars4(&HD44780Display, specCharMatrix, 2);
	
	LCDSendData4(&HD44780Display, DATA_TYPE_INSTRUCTION, DISPLAY_ON); 

	LCDShowString4(&HD44780Display, "libHD44780");
	LCDSendData4(&HD44780Display, DATA_TYPE_INSTRUCTION, LCD16x2_SELECT_LINE_2);
	LCDShowString4(&HD44780Display, "Tester-v1.2.0!");
	
	/*
		To manually set the cursor position in case of single line LCD,
		use the strating addresses of the two 8 bytes
		wide DDRAM memory segments as base addresses.
		LCD16x1_SELECT_DDRAM_1ST_HALF
		LCD16x1_SELECT_DDRAM_2ND_HALF 
	*/
	LCDSetCursorPosition4(&HD44780Display, LCD16x2_SELECT_LINE_1, 12);
	LCDSendData4(&HD44780Display, DATA_TYPE_INSTRUCTION, CURSOR_ON);
	LCDSendData4(&HD44780Display, DATA_TYPE_INSTRUCTION, BLINKING_CURSOR_ON); 
	
	//Show the data from the CGRAM
	LCDSendData4(&HD44780Display, DATA_TYPE_DATA, 0x00);
	LCDSendData4(&HD44780Display, DATA_TYPE_DATA, 0x01);
	
	while(1) {
      	   _delay_us(100);
 	}
}
