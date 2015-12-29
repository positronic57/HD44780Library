#Makefile that builds HD44780 library for 8-bit AVR MCU.

#Project name
LIBRARY_NAME = libHD44780

#AVR Microcontroller
MCU = atmega32

#CPU frequency 
CPU_FREQ = 4000000L

#Source files
SOURCES += HD44780.c

#List of object files
OBJECTS += $(SOURCES:.c=.o)

#Library header file
HEADER += HD44780.h

#Library installation folder
PREFIX = /usr/lib/avr

#Define C compiler flags
override CFLAGS = -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ)

#Define avr-ar options
override ARAGS = rcs


all: $(LIBRARY_NAME).a

$(SOURCES): $(HEADER)

#Compile all of the source files
$(OBJECTS): $(SOURCES)
	@echo Compiling file: $<
	avr-gcc $(CFLAGS) -c $^ -o $@
	@echo

#Builing the library
$(LIBRARY_NAME).a: $(OBJECTS)
	@echo Building target file: $@. 
	avr-ar $(ARAGS) $@ $^

#Install the library
install: $(LIBRARY_NAME).a
	if [ ! -d $(PREFIX)/lib/$(MCU) ]; then mkdir $(PREFIX)/lib/$(MCU); fi
	install -m 0644 $(LIBRARY_NAME).a $(PREFIX)/lib/$(MCU)
	install -m 0644 $(HEADER) $(PREFIX)/include

#Uninstall the library	
.PHONY: uninstall
uninstall:
	rm $(PREFIX)/lib/$(MCU)/$(LIBRARY_NAME).a
	rm $(PREFIX)/include/$(HEADER)

.PHONY: clean
clean:
	rm $(LIBRARY_NAME).a $(OBJECTS)

