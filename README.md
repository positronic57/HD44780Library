### AVR Library for LCD with HD44780 controller 

#### Short Description

HD44780library is a C library for sending commands and data to LCD with HD44780 display controller for Microchip AVR 8-bit microcontrollers. The library does not offer any high level of abstraction or methods for displaying content on LCD modules. It only povides low level calls for communication
with the LCD module. Any additional abstraction level can be build on top of it.

The library implements 4-bit data bus for communication with the HD44780 LCD controller. 
The communication is done in one direction only, from the microcontroller towards HD44780.
The library can only write data to HD44780. The value of R/W pin is all the time set to 0 by the library. 
Busy Flag checks are replaced by time delay functions.


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

#### Doc

Doxygen generated documentation is available in the  [doc](doc/index.html) folder.

#### Usage

The code comes with Makefile for building a static library using the make tool. The library can be linked to projects where needed. Other option is to copy the header and the source files direclty in the source tree of the target project.  

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