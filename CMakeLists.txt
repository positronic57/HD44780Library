##########################################################
#
# Created: 28-May-2025
# Author: Goce Boshkovski
#
# CMakeLists.txt file for building HD44780library static library
# for AVR MCUs with CMake using either GNU avr-gcc or
# Microchip XC8 compiler.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License.
#
##########################################################

cmake_minimum_required(VERSION 3.7)

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/avr" CACHE STRING "Installation prefix")

# Define the project name
project(HD44780library)

# Show the compiler commands during the build 
set(CMAKE_VERBOSE_MAKEFILE ON)

# Define the variables/command line arguments for target MCU and MCU clock frequency
set(MCU "atmega32" CACHE STRING "Target micro-controller with default value atmega32")
set(CPU_FREQ "16000000L" CACHE STRING "MCU CPU frequency")

# Define the extra include directories here
#include_directories(/usr/lib/avr/include)


if (NOT MICROCHIP_XC8_COMPILER)
  # Define linker options
  add_link_options(-mmcu=${MCU} -Wl,-Map=${PROJECT_NAME}.map)

  # Add mmcu as compiler option
  add_compile_options(-mmcu=${MCU})
else()
  # Microchip XC8 compiler	
  # Do not append -mmcu to linker options, aready done in toolchain file
  add_link_options(-Wl,-Map=${PROJECT_NAME}.map)
endif()

# Define the F_CPU macro required for some avr-libc functions
add_definitions(-DF_CPU=${CPU_FREQ})

# Define the list of source files
set (SOURCE_LIST
HD44780.c
)

# Build the project as a static library
add_library(HD44780 STATIC ${SOURCE_LIST})
target_include_directories(HD44780 PRIVATE /usr/lib/avr/include)

install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${MCU})
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

install(TARGETS HD44780 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${MCU})

install(FILES HD44780.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

