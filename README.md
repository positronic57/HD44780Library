### AVR Library for LCD with HD44780 controller 

#### Short Description

HD44780library is a C library for sending commands and data to LCD with HD44780 display controller from Microchip AVR 8-bit microcontrollers. The library does not offer any high level of abstraction or methods for displaying content on LCD modules. It only provides low level calls for communication
with the LCD module. Any additional abstraction level can be build on top of it.

The library supports only 4-bit data bus for communication with the HD44780 LCD controller. The code expects that all lines of the data bus (data lines D4..7 on HD44780 controller) are connected to the lower nibble of one of the AVR MCU ports.
For example: selecting AVR MCU PORTB for the data lines, the code will use PB0..3 for the data lines D4..D7.
The signaling lines: R/W, E and RS are expected to be pins on one AVR MCU port, and can be any pins of that port. For example: PC1, PC5, PC6.
 
The communication is done in one direction only, from the microcontroller towards HD44780. The library can only write data to HD44780. The value of R/W pin is all the time set to 0 by the library. The pulses on E signal line required for driving the transfer on the data lines are software generated using a time delay function.
  
HD44780 controller provides a Busy Flag (D7) for signaling the MCU that the processing of the last instruction is not done and all further data on the data lines will be ignored. The next data transfer will be accepted only when the Busy Flag is set back to 0. The library does not checks this flag. Instead, it uses a time delay functions between data transfers. 

#### Supported MCU Hardware

The library is compatible with every 8-bit Microchip AVR microcontroller with enough ports/pins for driving HD44780 based display module.

#### Supported LCD modules

Supported LCD modules based on HD44780 display controller:

 - single line;
 - two lines;
 - four lines displays.

40x4 LCD modules with two HD44780 controller chips that have two enable pins are not supported. 


#### Implemented HD44780 functionalities

Implemented HD44780 features:

 - send characters via 4-bit data bus;
 - send a string;
 - send HD44780 commands;
 - define custom special characters.


#### Usage

The easiest way to use the code is to copy the header and the source files directly in the source tree of the target project. The other option is to create a static library which later can be linked with other projects where needed. INSTALL file is a short HOWTO for building and installing the code as a static library. 

#### Test Hardware

Tested on Atmel Atmega32 microcontroller with: 

 - JHD 162A 2x16 LCD module;
 - EA W164B-NLW 4x16 LCD module;
 - EA W161B-ENLW 1x16 LCD module.

**WARNING:**
The source is provided as it is without any warranty. Use it on your own risk!
The author does not take any responsibility for the damage caused while using this software.

**DISCLAIMER:**
The code is a result of a hobby work and the author is not affiliated with any of the hardware/components/boards manufacturers mentioned in the code, documentation or the description of this project. All trademarks are the property of the respective owners.