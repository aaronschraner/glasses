#include <avr/io.h>
#include <util/delay.h>
#include "nrf.h"

void init_timer0() {
    // PWM mode operation, clear OCR0A+B on compare match
    TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00); 
    OCR0A = 20; // 50/255 = approx 20% duty cycle on power supply
    OCR0B = 25; // 25/255 = approx 10% duty cycle on LED current regulator
    TCCR0B = _BV(CS00); // no prescaler
    DDRA |= _BV(7);
    DDRB |= _BV(2);

}

Pin nrf_cs(PORTA, 1, OUTPUT),
          nrf_ce(PORTA, 3, OUTPUT);
const uint8_t station_address[6] = "1Node"; // receiver address

NRF nrf(nrf_ce, nrf_cs);

int main() {

    init_timer0();
    _delay_ms(50);

    spi_init();

    nrf.init();
    nrf.setup_rx_pipe(1, station_address, 1); 
    nrf.start_listening();

    uint8_t packet[32];

    while(true) {
        if(nrf.available())
        {
            nrf.stop_listening();
            nrf.read(packet);
            nrf.start_listening();
            switch(packet[0]) {
                case '+': OCR0A = OCR0A < 100 ? OCR0A + 1 : 100; break;
                case '-': OCR0A = OCR0A > 1   ? OCR0A - 1 : 1;   break;
                case '?': 
                          if(OCR0A > 5)
                              OCR0A = 1;
                          else
                              OCR0A = 100;
                          break;
                default: break;
            }
        }
        _delay_ms(2);
        
        //for(uint8_t t = 5; t < 60; t++) {
        //    OCR0A = t;
        //    _delay_ms(50);
        //}
        //for(uint8_t t = 60; t > 5; t--) {
        //    OCR0A = t;
        //    _delay_ms(50);
        //}
    }
}

