#define F_CPU					8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "usart.h"

#define TRIG	PB1
#define ECHO	PB2

volatile uint8_t edge = 0;
volatile uint8_t timer_overflow = 0;
uint32_t count = 0;
float distance;

void interrupt_init(void)
{
	MCUCSR = (1 << ISC2);	// interrupt trigger when rising edge
	GICR = (1 << INT2);		// enable ext int2
}

ISR(TIMER0_OVF_vect)
{
	timer_overflow++;
}

ISR(INT2_vect)
{
	if (edge == 0)
	{
		TCCR0 = (1 << CS00) | (1 << CS01);	// 64 prescale
		TCNT0 = 0;
		MCUCSR &= ~(1 << ISC2);	// detect falling edge on next interrupt
		edge = 1;
	}
	else
	{
		TCCR0 = 0;
		count = timer_overflow * 255 + TCNT0;
		timer_overflow = 0;
		MCUCSR |= (1 << ISC2);	// detect rising edge on next interrupt
		edge = 0;
	}	
}

int main(void)
{
	interrupt_init();
	usart_init(9600);

	DDRB |= (1 << TRIG);	// set trigger pin as output
	DDRB &= ~(1 << ECHO);	// set echo pin as input
	PORTB |= (1 << ECHO);	// enable internal pull up

	TIMSK = (1 << TOIE0);

	char str[15];
	sei();

	while (1)
	{
		PORTB |= (1 << TRIG);
		_delay_us(10);
		PORTB &= ~(1 << TRIG);

		distance = count * 0.1372;
		usart_puts("Distance: ");
		dtostrf(distance, 6, 2, str);
		strcat(str, " cm\r\n");
		usart_puts(str);
		_delay_ms(200);
	}

	return 0;
}