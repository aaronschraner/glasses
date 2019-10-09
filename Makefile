#microcontroller information
MCU=attiny84
AVRDUDEMCU=t84
OBJ2HEX=avr-objcopy
AVRDUDE=avrdude
PORT=/dev/ttyUSB0

CPPFILES=main.cpp spi.cpp
CC=avr-g++
HFILES=pin.h spi.h nrf.h nrf_defs.h
CLKFREQ=8MHz
CFLAGS=-g -Os -std=c++11 -Wall -Wno-reorder -mcall-prologues -mmcu=$(MCU) 
PROGRAMMER=usbasp
TARGET=glasses

###fuse configuration###
ifeq ($(CLKFREQ),8MHz)
#use 8MHz internal clock
LFUSE=0xE2
CPU_FREQ=8000000UL
else
#use 1MHz internal clock
LFUSE=0x62
CPU_FREQ=1000000UL
endif

HFUSE=0xDF
EFUSE=0xFF

CFLAGS += -DF_CPU=$(CPU_FREQ)

build: $(CPPFILES) $(HFILES)
	$(CC) $(CFLAGS) $(CPPFILES) -o $(TARGET).out
	$(OBJ2HEX) -R .eeprom -O ihex $(TARGET).out $(TARGET).hex

upload: build
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -c $(PROGRAMMER) -U flash:w:$(TARGET).hex #-P $(PORT)

fuse:
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -c $(PROGRAMMER) \
		-U lfuse:w:$(LFUSE):m -P $(PORT) # hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m -P $(PORT)


clean:
	rm -fv $(TARGET).out $(TARGET).hex

