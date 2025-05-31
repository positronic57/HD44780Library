/** 
 * @file HD44780.c
 * @brief Implements the functions defined in the header file.
 *
 * @author Goce Boshkovski
 * @date 13-Jun-15
 * @copyright GNU General Public License v2.
 */

#include <avr/io.h>
#include <util/delay.h>

#include "HD44780.h"

/* Function as a macro. It generates the a pulse with logical '1' duration
 * of _pulse_lenght_us micro seconds
 */
#define GENERATE_E_PULSE(_pulse_high_us)                              \
    *(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_E);          \
    _delay_us(_pulse_high_us);                                        \
    *(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));       \
    _delay_us(_pulse_high_us);


void LCDSendData4(const TSHD44780 *pHD44780, enum HD4478_DATA_TYPE data_type, uint8_t data_value)
{
    uint8_t DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;

    // Pull the R/W line to low, for write
    *(pHD44780->HD44780_CMD_PORT) &= ~_BV(pHD44780->HD44780_RW);

    switch(data_type)
    {
    case DATA_TYPE_INSTRUCTION:
        // Select an instruction register for write as a target
        *(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_RS));
        break;
    case DATA_TYPE_DATA:
        // Select a data register for write as a target
        *(pHD44780->HD44780_CMD_PORT) |= _BV(pHD44780->HD44780_RS);
        break;
    }
    // Pull E pin low
   *(pHD44780->HD44780_CMD_PORT) &= ~(_BV(pHD44780->HD44780_E));

    // Place the first 4 bits of the data on the data bus/lines
    DATA_PORT |= (data_value >> 4);
    *(pHD44780->HD44780_DATA_PORT) = DATA_PORT;

    // Generate an impulse on E line to imform the controller that the data are available
    GENERATE_E_PULSE(5);

    // Place the next 4 bits of the data on the data lines
    DATA_PORT = (*pHD44780->HD44780_DATA_PORT) & 0xF0;
    DATA_PORT |= data_value & 0x0F;
    *(pHD44780->HD44780_DATA_PORT) = DATA_PORT;

    // Generate an impulse on E line to inform the controller that the data are available
    GENERATE_E_PULSE(5);

    // Wait for the LCD controller to be ready fo rthe next data/command by
    _delay_ms(2);
}


// Configure LCD for 40bit data transfer
void LCDInit4(TSHD44780 *pHD44780, 
	volatile uint8_t *HD44780_CMD_PORT, 
	volatile uint8_t *HD44780_DATA_PORT, 
	uint8_t HD44780_RS, 
	uint8_t HD44780_E, 
	uint8_t HD44780_RW, 
	enum DISPLAY_SIZE_IN_LINES activeLines
)
{
	//Init ports/pins to which HD44780 is attached. The data bus is 4 bit
	pHD44780->HD44780_CMD_PORT = HD44780_CMD_PORT;
	pHD44780->HD44780_DATA_PORT = HD44780_DATA_PORT;
	pHD44780->HD44780_E = HD44780_E;
	pHD44780->HD44780_RS = HD44780_RS;
	pHD44780->HD44780_RW = HD44780_RW;

    *(pHD44780->HD44780_CMD_PORT - 1) |= (_BV(HD44780_RS) | _BV(HD44780_E) | _BV(HD44780_RW));  // Define the control bus PINs as outputs
    *(pHD44780->HD44780_DATA_PORT-1) |= 0x0F; //Define the lower 4 bits from the DATA_PORT as output PINs

    *(pHD44780->HD44780_CMD_PORT) &= ~(_BV(HD44780_RS) | _BV(HD44780_E) | _BV(HD44780_RW)); // Set the control bus PINs to logical 0
    *(pHD44780->HD44780_DATA_PORT) |= 0x03;

    GENERATE_E_PULSE(5);
    _delay_ms(5);

    GENERATE_E_PULSE(5);
	_delay_us(200);

    GENERATE_E_PULSE(5);
	_delay_us(200);
	
    *HD44780_DATA_PORT &= 0xF0;
    *(pHD44780->HD44780_DATA_PORT) |= 0x02;

    GENERATE_E_PULSE(5);
	_delay_us(200);

    LCDSendData4(pHD44780, DATA_TYPE_INSTRUCTION, (FUNCTION_SET | FOUR_BIT_DATA_LENGTH | activeLines));
    LCDSendData4(pHD44780, DATA_TYPE_INSTRUCTION, DISPLAY_OFF);
    LCDSendData4(pHD44780, DATA_TYPE_INSTRUCTION, CLEAR_DISPLAY);
    LCDSendData4(pHD44780, DATA_TYPE_INSTRUCTION, (ENTRY_MODE|CURSOR_MOVE_DIRECTION_INC));
}


//Display a string on the LCD
void LCDShowString4(const TSHD44780 *pHD44780, const char *String2Show)
{
	while(*String2Show) {
        LCDSendData4(pHD44780, DATA_TYPE_DATA, *String2Show++);
	}
}

//Define custom special characters
void LCDDefineSpecialChars4(const TSHD44780 *pHD44780, const uint8_t (*charMatrix)[8], uint8_t numberOfSpecChars)
{
	uint8_t CGAddress = LCD_CGRAM_START_ADDRESS;
	uint8_t i,j;
	
    if (numberOfSpecChars > 8) {
        numberOfSpecChars = 8;
    }

    for(i = 0; i < numberOfSpecChars; i++)
    {
        for(j = 0; j < 8; j++)
		{
			//Set CGRAM address
            LCDSendData4(pHD44780, DATA_TYPE_INSTRUCTION, CGAddress++);
			//Write data to CGRAM
            LCDSendData4(pHD44780, DATA_TYPE_DATA, *(*(charMatrix + i) + j));
		}
    }

    LCDSendData4(pHD44780, DATA_TYPE_INSTRUCTION, RETURN_CURSOR_HOME);
}



