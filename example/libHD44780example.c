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
 * Example of using HD44780 library on Atmega32.
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
    
	TSHD44780 HD44780Display;
	TSHD44780 *pHD44780Display = &HD44780Display;
	
	LCDInit4(pHD44780Display,&PORTD,&PORTB,PD4,PD7,PD6,TWO_LINES_DISPLAY);
	LCDSendCommand4(pHD44780Display,DISPLAY_ON); 

	LCDShowString4(pHD44780Display,"libHD44780");
	LCDSendCommand4(pHD44780Display,LCD16x2_SELECT_LINE_2);
	LCDShowString4(pHD44780Display,"Tester!!!");
	
	LCDSendCommand4(pHD44780Display,CURSOR_ON);
	LCDSendCommand4(pHD44780Display,BLINKING_CURSOR_ON);
	
	while(1)
	{
        
	}
}