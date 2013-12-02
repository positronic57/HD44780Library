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

#include "hd44780-4bit.h"

// Configure LCD for 40bit data transfer
void LCDInit4(void)
{

	delay_ms(50);

	CMD_PORT &= ~(_BV(HD44780_RS)|_BV(HD44780_RW)|_BV(HD44780_E)); //Postavi gi pinovite CMD_PORT.7, CMD_PORT.6 i CMD_PORT.4 na logichka 0
	CMD_DDR |= (_BV(HD44780_RS)|_BV(HD44780_RW)|_BV(HD44780_E));  // Definiraj gi CMD_PORT.7 i CMD_PORT.6 i CMD_PORT.4 kako izlezni pinovi

	DATA_PORT = 0x03; 
	DATA_DDR |= 0x0F; //Definiraj gi dolnite 4-ri pinovi od DATA_PORT kako izlezni
	
	CMD_PORT |= _BV(HD44780_E); //Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na pin CMD_PORT.7
	delay_ms(5);

	CMD_PORT |= _BV(HD44780_E); //Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na pin CMD_PORT.7
	delay_us(200);

	CMD_PORT |= _BV(HD44780_E); //Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na pin CMD_PORT.7
	delay_us(200);
	
	DATA_PORT = 0x02;
	CMD_PORT |= _BV(HD44780_E); //Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na pin CMD_PORT.7
	delay_us(200);


    LCDSendCommand4(0x28); 
	LCDSendCommand4(0x0C); //TurnON the monitor without cursor
	LCDSendCommand4(0x06);
	LCDSendCommand4(CLEAR_DISPLAY);
	
}

// 
void LCDSendCommand4(uint8_t Command2Send)
{
	
	CMD_PORT &= ~(_BV(HD44780_RS)|_BV(HD44780_E)); //Postavi gi pinovite CMD_PORT.4 i CMD_PORT.7 na logichka 0
	CMD_DDR |= (_BV(HD44780_RS)|_BV(HD44780_E));  // Definiraj gi CMD_PORT.4 i CMD_PORT.7 kako izlezni pinovi
	 
	DATA_PORT = Command2Send >> 4; //Gornite 4-ri bita od Command na porta B
	DATA_DDR |= 0x0F; //Definiraj gi dolnite 4-ri pinovi od DATA_PORT kako izlezni
	
	CMD_PORT |= _BV(HD44780_E); //Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na pin DATA_PORT.0
	
	//delay_us(50);

	DATA_PORT = Command2Send & 0x0F;
	CMD_PORT |= _BV(HD44780_E); //Postavi go pinot DATA_PORT.0 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na pin DATA_PORT.0

	delay_ms(2);

}

//Display a character on LCD
void LCDShowCharacter4(char Character2Show)
{

	delay_ms(1);
	CMD_PORT |= _BV(HD44780_RS);	//Postavi go pinot CMD_PORT.4 na logichka 1;
	CMD_PORT &= ~(_BV(HD44780_E));  //Postavi go pinot CMD_PORT.7 na logichka 0;
	CMD_DDR |= (_BV(HD44780_RS)|_BV(HD44780_E));	//Definiraj gi pinovite CMD_PORT.7 i CMD_PORT.4 kako izlezni pinovi

	DATA_PORT = Character2Show >> 4;
	DATA_DDR |= 0x0F;	//Definiraj gi dplnite 4 pinovi od DATA_PORT kako izlezni;
	
	CMD_PORT |= _BV(HD44780_E);	//Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na DATA_PORT.0

	DATA_PORT = Character2Show & 0x0F;
	CMD_PORT |= _BV(HD44780_E);	//Postavi go pinot CMD_PORT.7 na logichka 1
	delay_us(5);
	CMD_PORT &= ~(_BV(HD44780_E)); //Postavi logichka nula na CMD_PORT.7

	//Vremensko docnenje za procesiranje na naredbata od strana na LCD-to
	delay_ms(1);
	
}

//Display a string on the LCD
void LCDShowString4(char *String2Show)
{
	while(*String2Show)
		LCDShowCharacter4(*String2Show++);
}

void LCDSetCursorPosition4(uint8_t redica, uint8_t kolona)
{
	uint8_t LineStartAddress[2]={FIRST_LINE_DDRAM_START_ADDRESS,SECOND_LINE_DDRAM_START_ADDRESS};

	LCDSendCommand4(LineStartAddress[redica-1]+kolona);

}
