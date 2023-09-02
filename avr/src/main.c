#define F_CPU	8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "nrf24l01.h"
#include "usart.h"
#include <string.h>

int main(void)
{
    DDRA |= _BV(DDA0) | ~_BV(DDA1);
    PORTA |= _BV(PA1);
    uint8_t addr[5] = {0x00, 0xDD, 0xCC, 0xBB, 0xAA};

    usart_init(9600);
    spi_master_init();
    nrf24_init();
    nrf24_tx(addr);

    nrf24_print_register_value();

    uint8_t string[] = "haloo";

    while (1)
    {
        if (nrf24_transmit(string))
        {
            PORTA ^= _BV(PA0);
        }
        _delay_ms(500);
    }
}