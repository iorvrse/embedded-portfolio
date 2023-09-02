#include "usart.h"

void usart_init(uint32_t baud)
{
	UCSRB = _BV(TXEN) | _BV(RXEN);						// Enable transmit & receive
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);		// Use 8 bit character
	UBRRL = BAUD_CALC(baud);							// Set lower bit baudrate
	UBRRH = (BAUD_CALC(baud) >> 8);						// Set higher bit baudrate
}

void usart_putchar(uint8_t byte)
{
	loop_until_bit_is_set(UCSRA, UDRE);		// Wait transmit buffer empty
	UDR = byte;
}

char usart_getchar(void)
{
	loop_until_bit_is_set(UCSRA, RXC);		// Wait receive buffer for unread data
	return UDR;
}

void usart_putstring(const uint8_t *str, uint16_t size)
{
	while (size--)
	{
		usart_putchar(*str++);
	}
}

void usart_uint8(const uint8_t *str)
{
	if (*str >= 100)
	{
		usart_putchar('0' + (*str / 100));
		usart_putchar('0' + (*str / 10) % 10);
	}
	else if (*str >= 10)
	{
		usart_putchar('0' + (*str / 10));
	}
	usart_putchar('0' + (*str % 10));
}