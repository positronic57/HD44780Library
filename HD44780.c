/** 
 * @file HD44780.c
 * @brief Implements the functions defined in the header file.
 *
 * @author Goce Boshkovski
 * @date 13-Jun-15
 * @copyright GNU General Public License v2.
 */

#include "HD44780.h"
#include <avr/io.h>
#include <util/delay.h>

// Configure LCD for 40bit data transfer
void LCDInit4(TSHD44780 *pHD44780,volatile uint8_t *HD44780_CMD_PORT,volatile uint8_t *HD44780_DATA_PORT,uint8_t HD44780_RS,uint8_t HD44780_E,uint8_t HD44780_RW,uint8_t activeLines)
{
	uint8_t DATA_PORT = *HD44780_DATA_PORT & 0xF0;
	
	_delay_ms(50);

	//Init ports/pins to which HD44780 is attached. The data bus is 4 bit
	pHD44780->HD44780_CMD_PORT = HD44780_CMD_PORT;
	pHD44780->HD44780_DATA_PORT = HD44780_DATA_PORT;
	pHD44780->HD44780_E = HD44780_E;
	pHD44780->HD44780_RS = HD44780_RS;
	pHD44780->HD44780_RW = HD44780_RW;

	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_RS) |_BV(HD44780_E) |_BV(HD44780_RW)); //Set the control bus PINs to logical 0
	*(pHD44780->HD44780_CMD_PORT-1) |= (_BV(HD44780_RS)|_BV(HD44780_E)|_BV(HD44780_RW));  // Define the control bus PINs as outputs

	DATA_PORT |= 0x03;
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_DATA_PORT-1) |= 0x0F; //Define the lower 4 bits from the DATA_PORT as output PINs
	
	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	_delay_ms(5);

	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	_delay_us(200);

	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	_delay_us(200);
	
	DATA_PORT = *HD44780_DATA_PORT & 0xF0;
	DATA_PORT |= 0x02;
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_CMD_PORT) |= _BV(HD44780_E);
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_E));
	_delay_us(200);

	LCDSendCommand4(pHD44780,(FUNCTION_SET|FOUR_BIT_DATA_LENGTH|activeLines));
	LCDSendCommand4(pHD44780,DISPLAY_OFF); 
	LCDSendCommand4(pHD44780,CLEAR_DISPLAY);
	LCDSendCommand4(pHD44780,(ENTRY_MODE|CURSOR_MOVE_DIRECTION_INC));
	
}


//Send command to HD44780
void LCDSendCommand4(TSHD44780 *pHD44780, uint8_t Command2Send)
{
	uint8_t DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_RS)|_BV(pHD44780->HD44780_E)); 
	
	DATA_PORT |= (Command2Send >> 4); 	
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT; 
		
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E); 
	
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E)); 
	
	DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	DATA_PORT |= (Command2Send & 0x0F);
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E); 
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E)); 
	_delay_ms(2);

}

//Display a character on LCD
void LCDShowCharacter4(TSHD44780 *pHD44780, char Character2Show)
{
	uint8_t DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	
	_delay_ms(1);
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_RS);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

	DATA_PORT |= (Character2Show >> 4); 	
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E);
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

	DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
	DATA_PORT |= Character2Show & 0x0F;
	*(pHD44780->HD44780_DATA_PORT) = DATA_PORT;
	*(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E);
	_delay_us(5);
	*(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

	//Time delay for the HD44780 to process the command
	_delay_ms(1);
	
}


//Display a string on the LCD
void LCDShowString4(TSHD44780 *pHD44780,char *String2Show)
{
	while(*String2Show)
	LCDShowCharacter4(pHD44780,*String2Show++);
}

//Define custom special characters
void LCDDefineSpecialChars4(TSHD44780 *pHD44780,uint8_t (*charMatrix)[8],uint8_t numberOfSpecChars)
{
	uint8_t CGAddress = LCD_CGRAM_START_ADDRESS;
	uint8_t i,j;
	
	if (numberOfSpecChars>8)
		numberOfSpecChars=8;
		
	for(i=0;i<numberOfSpecChars;i++)
		for(j=0;j<8;j++)
		{
			//Set CGRAM address
			LCDSendCommand4(pHD44780,CGAddress++);
			//Write data to CGRAM
			LCDShowCharacter4(pHD44780,*(*(charMatrix+i)+j));
		}
	
	LCDSendCommand4(pHD44780,RETURN_CURSOR_HOME);
		
}

