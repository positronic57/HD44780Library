/*
 * HD44780.c
 *
 * Created: 13-Jun-15 23:01:49
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License. 
 *
 */ 

#include "HD44780.h"
#include <avr/io.h>

//Delay loop functions
void delayloop32(uint32_t loops)
{
	__asm__ volatile ( "cp  %A0,__zero_reg__ \n\t"  \
	"cpc %B0,__zero_reg__ \n\t"  \
	"cpc %C0,__zero_reg__ \n\t"  \
	"cpc %D0,__zero_reg__ \n\t"  \
	"breq L_Exit_%=       \n\t"  \
	"L_LOOP_%=:           \n\t"  \
	"subi %A0,1           \n\t"  \
	"sbci %B0,0           \n\t"  \
	"sbci %C0,0           \n\t"  \
	"sbci %D0,0           \n\t"  \
	"brne L_LOOP_%=            \n\t"  \
	"L_Exit_%=:           \n\t"  \
	: "=w" (loops)              \
	: "0"  (loops)              \
	);                             \
	
	return;
}

// Configure LCD for 40bit data transfer
void LCDInit4(TSHD44780 *pHD44780,volatile uint8_t *HD44780_CMD_PORT,volatile uint8_t *HD44780_DATA_PORT,uint8_t HD44780_RS,uint8_t HD44780_E,uint8_t HD44780_RW,uint8_t lineNumbers)
{
	uint8_t DATA_PORT = *HD44780_DATA_PORT & 0xF0;
	
	delay_ms(50);

	//Init ports/pins to which HD44780 is attached. The data bus is 4 bit
	pHD44780->HD44780_CMD_PORT = HD44780_CMD_PORT;
	pHD44780->HD44780_DATA_PORT = HD44780_DATA_PORT;
	pHD44780->HD44780_E = HD44780_E;
	pHD44780->HD44780_RS = HD44780_RS;
		
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_RS) |_BV(HD44780_E) |_BV(HD44780_RW)); //Set the control bus PINs to logical 0
	*(pHD44780->HD44780_CMD_PORT-1) |= (_BV(HD44780_RS)|_BV(HD44780_E)|_BV(HD44780_RW));  // Define the control bus PINs as outputs

	DATA_PORT |= 0x03;
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_DATA_PORT-1) |= 0x0F; //Define the lower 4 bits from the DATA_PORT as output PINs
	
	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	delay_ms(5);

	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	delay_us(200);

	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	delay_us(200);
	
	DATA_PORT = *HD44780_DATA_PORT & 0xF0;
	DATA_PORT |= 0x02;
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	delay_us(200);

    	LCDSendCommand4(pHD44780,(FUNCTION_SET|4BIT_DATA_LENGTH|lineNumbers));
	LCDSendCommand4(pHD44780,(DISPLAY_CONTROL|LCD_TURN_ON|CURSOR_ON)); 
	LCDSendCommand4(pHD44780,(ENTRY_MODE|CURSOR_MOVE_DIRECTION_INC));
	LCDSendCommand4(pHD44780,CLEAR_DISPLAY);
	
}


//Send command to HD44780
void LCDSendCommand4(TSHD44780 *pHD44780, uint8_t Command2Send)
{
	uint8_t DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_RS)|_BV(pHD44780->HD44780_E)); 
	
	DATA_PORT |= (Command2Send >> 4); 	
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT; 
		
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E); 
	
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E)); 
	
	DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	DATA_PORT |= (Command2Send & 0x0F);
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E); 
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E)); 
	delay_ms(2);

}

//Display a character on LCD
void LCDShowCharacter4(TSHD44780 *pHD44780, char Character2Show)
{
	uint8_t DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	
	delay_ms(1);
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_RS);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

	DATA_PORT |= (Character2Show >> 4); 	
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E);
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

	DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	DATA_PORT |= Character2Show & 0x0F;
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E);
	delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

	//Time delay for the HD44780 to process the command
	delay_ms(1);
	
}


//Display a string on the LCD
void LCDShowString4(TSHD44780 *pHD44780,char *String2Show)
{
	while(*String2Show)
	LCDShowCharacter4(pHD44780,*String2Show++);
}

void LCDSetCursorPosition4(TSHD44780 *pHD44780, uint8_t raw, uint8_t column)
{
	uint8_t LineStartAddress[2]={FIRST_LINE_DDRAM_START_ADDRESS,SECOND_LINE_DDRAM_START_ADDRESS};

	LCDSendCommand4(pHD44780,LineStartAddress[raw]+column);

}