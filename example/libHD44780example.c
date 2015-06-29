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
 * Example of using HD44780 library on Atmega32.The CPU freq is 4MHz defined with F_CPU. 
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

#define F_CPU 4000000UL

#include <avr/io.h>
#include "HD44780.h"


int main(void)
{
    
	//Define custom special characters. In this example those are Cyrillic letters: sh and d.
	uint8_t specCharMatrix[2][8]={{0x15,0x15,0x15,0x15,0x15,0x15,0x1F,0x00},{0x0E,0x0A,0x0A,0x0A,0x0A,0x1F,0x11,0x00}};
								
	TSHD44780 HD44780Display;
	TSHD44780 *pHD44780Display = &HD44780Display;
	
	/*
		Init a display module with 2 lines.
		In case of 4 line module, replace TWO_LINES_DISPLAY with FOUR_LINES_DISPLAY.	
	*/	
	LCDInit4(pHD44780Display,&PORTD,&PORTB,PD4,PD7,PD6,TWO_LINES_DISPLAY);
	LCDDefineSpecialChars4(pHD44780Display,specCharMatrix,2);
	
	LCDSendCommand4(pHD44780Display,DISPLAY_ON); 

	LCDShowString4(pHD44780Display,"libHD44780");
	LCDSendCommand4(pHD44780Display,LCD16x2_SELECT_LINE_2);
	LCDShowString4(pHD44780Display,"Tester!!!");
	
	LCDSetCursorPosition4(pHD44780Display,LCD16x2_SELECT_LINE_1,12);
	LCDSendCommand4(pHD44780Display,CURSOR_ON);
	LCDSendCommand4(pHD44780Display,BLINKING_CURSOR_ON); 
	
	//Show the data from the CGRAM
	LCDShowCharacter4(pHD44780Display,0x00);
	LCDShowCharacter4(pHD44780Display,0x01);
	
	while(1)
	{
        
	}
}
