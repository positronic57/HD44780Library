#Makefile for bor building the example distributed 
#with HD44780Library.

#Project name
PROJECT = libHD44780example

#AVR Microcontroller
ifdef mcu
	MCU = $(mcu)
else
	MCU = atmega32
endif

#CPU frequency (must be the same as the value used while building DS18S20Library).
ifndef cpu_freq
	CPU_FREQ = 4000000L
else
	CPU_FREQ = $(cpu_freq)
endif

#Header files path (Update this with the library installation folder).
HEADERS_PATH = /usr/local/include/avr8

#Library path
LIBRARY_PATH = /usr/local/lib/avr8/$(MCU)

#List of source files
SOURCES += $(PROJECT).c

#List of object files
OBJS += $(SOURCES:.c=.o)

#Define C compiler flags
override CFLAGS = -Wall -I$(HEADERS_PATH) -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=$(MCU) -DF_CPU=$(CPU_FREQ)

all: hex

#Compile all of the source files
%.o: %.c
	@echo Building file: $<
	avr-gcc $(CFLAGS) -c $^ -o $@
	@echo

#Generate ELF file from the object files using linker
$(PROJECT).elf: $(PROJECT).o
	@echo Building target file: $@. 
	avr-gcc -Wl,-Map,$(PROJECT).map -mmcu=$(MCU) -o $@ $^ -L$(LIBRARY_PATH) -lHD44780
	@echo 
	@echo $@ memory footprint:
	avr-size --format=avr --mcu=atmega32 $@

hex: $(PROJECT).hex
$(PROJECT).hex : $(PROJECT).elf
	@echo Creating object file in ihex format.
	avr-objcopy -j .text -j .data -O ihex $< $@
	@echo

.PHONY: clean

clean:
	rm -f *.o *.elf *.hex *.map
 
