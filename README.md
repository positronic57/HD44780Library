### AVR Library for LCD with HD44780 controller 

#### Short Description

HD44780library is C library for LCD with HD44780 display controller for AVR 8-bit microcontrollers.
The library implements 4-bit data bus for communication with the HD44780 LCD controller. 
The communication is done in one direction only, from the microcontroller towards HD44780.
The library can only write data to HD44780. The value of R/W pin is all the time set to 0 by the library. 
Busy Flag checks are replaced by time delay functions.

#### Supported MCU Hardware

The library is compatible with every 8-bit AVR microcontroller.

#### Supported LCD modules

Supported LCD modules based on HD44780:

 - single line;
 - two lines;
 - four lines displays.

40x4 LCD modules with two HD44780 controller chips that have two enable pins are not supported. 


#### Implemented HD44780 functionalities

Implemented HD44780 features:

 - send characters via 4bit data bus;
 - send a string;
 - send HD44780 commands;
 - define custom special characters.

#### Doc

A detailed description of the library is available in the doc folder.

#### Test Hardware

Tested on Atmel Atmega32 microcontroller with: 

 - JHD 162A 2x16 LCD module;
 - EA W164B-NLW 4x16 LCD module;
 - EA W161B-ENLW 1x16 LCD module.

**WARNING:**
The source is provided as it is without any warranty. Use it on your own risk!
The author does not take any responsibility for the damage caused while using this software.
