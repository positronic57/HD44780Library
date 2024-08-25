/**
 * @file HD44780.h
 * @brief 
 * Header file of HD44780 library. It contains the prototypes of all
 * functions available in the library, definitions of all macros
 * and constans.
 * 
 * @author Goce Boshkovski
 * @date 13-Jun-15
 * @copyright GNU General Public License v2.
 * 
 */

#ifndef HD44780_H_
#define HD44780_H_

#include <stdint.h>

/**
 * @brief Define MCU CPU Freq used for the time delay functions.
 * 
 * If not defined, the library will use the default vale of 4MHz.
 * 
 */
#ifndef F_CPU
#define F_CPU 4000000UL
#endif

/** \defgroup libMacros HD44780 library macros */
/** @{ */
/**
 * @def LCDSetCursorPosition4(LCD,lineBaseAddress,position)
 * Sets the new cursor position defined by
 * \a as display line and \a position in that line.
 * 
 * The new position is send as a command to HD44780 via
 * LCDSendCommand4() function.
 */
#define LCDSetCursorPosition4(LCD, lineBaseAddress, position) LCDSendCommand4(LCD, lineBaseAddress+position)
/** @} */

/** \defgroup LCDCommands List of general LCD Commands */
/** @{ */
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

#define FONT_SIZE 0x04

#define SET_ONE_LINE_MODE 0x20
#define SET_TWO_LINE_MODE 0x28
/** @} */


/** \defgroup LCDLines List of LCD Commands for selecting a display line */
/** @{ */
/** 
* @par
* DDRAM of single line LCD in most of the cases
* is divided in two 8 bytes wide memory
* segments. The starting addresses of 
* those segments are defined as:
*  - LCD16x1_SELECT_DDRAM_1ST_HALF
*  - LCD16x1_SELECT_DDRAM_2ND_HALF
* Those LCD modules although they have a single line,
* due to the DDRAM layout must be configured as
* two line modules.
*/
#define LCD16x1_SELECT_DDRAM_1ST_HALF 0x80
#define LCD16x1_SELECT_DDRAM_2ND_HALF 0xC0

#define LCD16x2_SELECT_LINE_1 0x80
#define LCD16x2_SELECT_LINE_2 0xC0

#define LCD16x4_SELECT_LINE_1 0x80
#define LCD16x4_SELECT_LINE_2 0xC0
#define LCD16x4_SELECT_LINE_3 0x90
#define LCD16x4_SELECT_LINE_4 0xD0

#define LCD20x2_SELECT_LINE_1 0x80
#define LCD20x2_SELECT_LINE_2 0xC0

#define LCD20x4_SELECT_LINE_1 0x80
#define LCD20x4_SELECT_LINE_2 0xC0
#define LCD20x4_SELECT_LINE_3 0x94
#define LCD20x4_SELECT_LINE_4 0xD4

#define LCD40x2_SELECT_LINE_1 0x80
#define LCD40x2_SELECT_LINE_2 0xC0
/** @} */

/** \defgroup LCDCGRAM LCD CGRAM Starting Address */
/** @{ */
#define LCD_CGRAM_START_ADDRESS 0x40
/** @} */

/** \defgroup LCDDDRAM LCD DDRAM Address Segments*/
/** @{ */
#define LCD16x1_FIRST_DDRAM_SEGMENT_START_ADDRESS 0x00
#define LCD16x1_SECOND_DDRAM_SEGMENT_START_ADDRESS 0x40
#define LCD16x2_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD16x2_SECOND_LINE_DDRAM_START_ADDRESS 0x40
#define LCD16x4_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD16x4_SECOND_LINE_DDRAM_START_ADDRESS 0x40
#define LCD16x4_THIRD_LINE_DDRAM_START_ADDRESS 0x10
#define LCD16x4_FORTH_LINE_DDRAM_START_ADDRESS 0x50
#define LCD20x2_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD20x2_SECOND_LINE_DDRAM_START_ADDRESS 0x40
#define LCD20x4_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD20x4_SECOND_LINE_DDRAM_START_ADDRESS 0x40
#define LCD20x4_THIRD_LINE_DDRAM_START_ADDRESS 0x14
#define LCD20x4_FORTH_LINE_DDRAM_START_ADDRESS 0x54
#define LCD40x2_FIRST_LINE_DDRAM_START_ADDRESS 0x00
#define LCD40x2_SECOND_LINE_DDRAM_START_ADDRESS 0x40
/** @} */

/** \defgroup LCDSizeInLines Display size in number of lines */
/** @{ */
/**
 * Enum for size of the display in numer of lines
 */
enum DISPLAY_SIZE_IN_LINES {
	SINGLE_LINE_DISPLAY = 0x00,
	TWO_LINES_DISPLAY = 0x08,
	FOUR_LINES_DISPLAY = 0x08
};
/** @} */

/**
 * @brief Represents the implementation
 * of the data and command bus used for connecting LCD module with the
 * AVR microcontroller.
 */
typedef struct SHD44780
{
	volatile uint8_t *HD44780_CMD_PORT; /**< address of the MCU PORT register used for the command bus.*/
	volatile uint8_t *HD44780_DATA_PORT; /**< address of the MCU PORT register used for the data bus.*/
	uint8_t HD44780_RS; /**< PIN from the CMD PORT connected to the RS pin of HD44780.*/
	uint8_t HD44780_E; /**< PIN from the CMD PORT connected to the E pin of HD44780.*/
	uint8_t HD44780_RW; /**< PIN from the CMD PORT connected to the RW pin of HD44780.*/
} TSHD44780;

/** \defgroup libFunctions HD44780 library functions */
/** @{ */
/**
 * @brief  Init function for HD44780 controller.
 * The function defines and configure 
 * the microcontroller pins that are used as interface for the LCD module.
 * It performs 4-bit initializations of HD44780 and defines the number
 * of display lines.
 * 
 * @example libHD44780example.c
 * 
 * @param[in,out] pHD44780 pointer to the structur that represent HD44780
 * @param[in] HD44780_CMD_PORT pointer, addredess of the MCU PORT used for cmd bus
 * @param[in] HD44780_DATA_PORT  pointer, address of the MCU PORT used for data bus
 * @param[in] HD44780_RS defines the MCU pin connected to RS pin of HD44780
 * @param[in] HD44780_RS MCU defines the pin connected to RS pin of HD44780
 * @param[in] HD44780_E MCU defines the pin connected to E pin of HD44780
 * @param[in] HD44780_RW MCU defines the pin connected to RW pin of HD44780 
 * @param[in] activeLines enum for the number of display lines.
 * @parblock Possible values are:
 * SINGLE_LINE_DISPLAY, TWO_LINES_DISPLAY, FOUR_LINE_DISPALY.
 * DDRAM of single line LCD in most of the cases
 * is divided in two 8 bytes wide memory 
 * segments. The starting addresses of 
 * those segments are defined as:
 * 	- LCD16x1_SELECT_DDRAM_1ST_HALF;
 * 	- LCD16x1_SELECT_DDRAM_2ND_HALF.
 * Those LCD modules although they have a single line,
 * due to the DDRAM layout must be configured as
 * two line modules.
 * @endparblock
 * @return void.
 */
void LCDInit4(TSHD44780 *pHD44780, 
	volatile uint8_t *HD44780_CMD_PORT, 
	volatile uint8_t *HD44780_DATA_PORT, 
	uint8_t HD44780_RS, 
	uint8_t HD44780_E, 
	uint8_t HD44780_RW, 
	enum DISPLAY_SIZE_IN_LINES activeLines
);

/**
 * @brief The function is used to send a command to HD44780. 
 * The supported commands are defined in this header file.
 * 
 * @param[in] pHD44780 pointer to the structur that represent HD44780
 * @param[in] Command2Send HD44780 command.
 */
void LCDSendCommand4(const TSHD44780 *pHD44780, uint8_t Command2Send);

/**
 * @brief Display a character on the LCD at the current cursor position.
 *  
 * @param[in] pHD44780 pointer to the structur that represent HD44780
 * @param[in] Character2Show A character to be desplayed.
 * 
 * @return void.
 */
void LCDShowCharacter4(const TSHD44780 *pHD44780, char Character2Show);

/**
 * @brief Display a string on the LCD starting from the current cursor 
 * position. The function doesn't move to a new line on the LCD if an 
 * end of line is reached.
 * 
 * 
 * @param[in] pHD44780 pointer to the structur that represent HD44780
 * @param[in] String2Show a string to be desplayed.
 * 
 * @return void.
 */
void LCDShowString4(const TSHD44780 *pHD44780, const char *String2Show);

/**
 * @brief HD44780 supports up to 8 user defined characters. 
 * The definitions of the characters are stored in the CGRAM of the controller.
 * CGRAM is 64 bytes long where each character is defined by 8 bytes.
 * The function defines those special characters.
 * 
 * 
 * @param[in] pHD44780 pointer to the structur that represent HD44780
 * @param[in] charMatrix pointer to a matrix with 8 columns and up to 8 raws.
 * @parblock
 * Each raw holds definition of one custom character.
 * @endparblock
 * @param[in] numberOfSpecChars numbur of custom character (number of raws of the charMatrix)
 * 
 * @return void.
 */
void LCDDefineSpecialChars4(const TSHD44780 *pHD44780, const uint8_t (*charMatrix)[8],uint8_t numberOfSpecChars);
/** @} */

#endif /* HD44780_H_ */
