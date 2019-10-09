#include "spi.h"
#include <avr/io.h>
//////////////////////////////
// spi.cpp
//
// driver for software SPI on attiny84
// Copyright Aaron Schraner, 2019
// 

#include "pin.h"
#include <util/delay.h>

void spi_init()
{
    const Pin
        SCK(PORTA, 4, OUTPUT),
        MOSI(PORTA, 6, OUTPUT),
        MISO(PORTA, 5, INPUT);
    SCK = 0;
}
// send <data>, MSB first
// return SPI input data
uint8_t spi_send(uint8_t data)
{
    uint8_t out = 0;
    const Pin
        SCK(PORTA, 4, OUTPUT),
        MOSI(PORTA, 6, OUTPUT),
        MISO(PORTA, 5, INPUT);
    SCK = 0;
    for(uint8_t x=0x80; x; x>>=1) {
        MOSI = !!(data & x);
        SCK = 1;
        if(MISO)
            out |= x;
        SCK = 0;
    }
    return out;
}

