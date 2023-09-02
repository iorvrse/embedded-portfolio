#ifndef _USART_H_
#define _USART_H_

#ifndef F_CPU
#define F_CPU   8000000UL
#endif /* F_CPU */

#include <avr/io.h>

#define BAUD_CALC(baudrate) ((F_CPU + 8UL * (baudrate)) / (16UL * (baudrate)) - 1UL)

void usart_init(uint32_t baud);

void usart_putchar(uint8_t byte);

char usart_getchar(void);

void usart_uint8(const uint8_t *str);

void usart_putstring(const uint8_t *str, uint16_t size);

#endif	/* _USART_H_ */