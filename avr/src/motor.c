#define F_CPU					8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "usart.h"

#define MOTOR_ENA	PD5
#define MOTOR_ENB	PD4

#define MOTOR_IN1	PD2
#define MOTOR_IN2	PD3

#define MOTOR_IN3	PD6
#define MOTOR_IN4	PD7

void adc_init(void)
{
	DDRA = 0;
	ADCSRA = (1 << ADEN)
		| (1 << ADPS2)
		| (1 << ADPS1);		// ADC enabled, clk/64

	ADMUX = (1 << REFS0);	// AVcc as voltage reference
}

int adc_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xE0) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC);
	loop_until_bit_is_set(ADCSRA, ADIF);
	_delay_ms(1);
	
	return ADCW;
}

void pwm_init(void)
{
	DDRD = DDRD
		| (1 << MOTOR_ENA)
		| (1 << MOTOR_ENB);

	TCCR1A = (1 << WGM10)
		| (1 << COM1A1)
		| (1 << COM1B1);
		
	TCCR1B = (1 << CS10);
}

inline int map(int *value, int oldmin, int oldmax, int newmin, int newmax)
{
	return ((float)(*value - oldmin) / (oldmax - oldmin)) * (newmax - newmin) + newmin;
}

int main(void)
{
	adc_init();
	pwm_init();
	usart_init(9600);

	DDRD = DDRD
		| (1 << MOTOR_IN1)
		| (1 << MOTOR_IN2)
		| (1 << MOTOR_IN3)
		| (1 << MOTOR_IN4);

	int y_axis;
	int x_axis;
	uint8_t motor_r;
	uint8_t motor_l;

	char str[5];

	while (1)
	{
		y_axis = adc_read(0);
		usart_puts("y_adc: ");
		itoa(y_axis, str, 10);
		usart_puts(str);
		
		x_axis = adc_read(1);
		usart_puts("\tx_adc: ");
		itoa(x_axis, str, 10);
		usart_puts(str);
		
		// y axis joystick control forward & backward
		if (y_axis < 473)
		{
			// Set all motors backward
			PORTD = PORTD
				| (1 << MOTOR_IN2)
				| (1 << MOTOR_IN3);
			PORTD = PORTD
				& ~(1 << MOTOR_IN1)
				& ~(1 << MOTOR_IN4);

			motor_r = map(&y_axis, 473, 0, 0, 255);
			motor_l = motor_r;
		}
		else if (y_axis > 550)
		{
			// set all motors forward
			PORTD = PORTD
				& ~(1 << MOTOR_IN2)
				& ~(1 << MOTOR_IN3);
			PORTD = PORTD
				| (1 << MOTOR_IN1)
				| (1 << MOTOR_IN4);
			
			motor_r = map(&y_axis, 550, 1023, 0, 255);
			motor_l = motor_r;
		}
		else
		{
			// stop motor when joystick in the middle position
			PORTD = PORTD
				& ~(1 << MOTOR_IN1)
				& ~(1 << MOTOR_IN2)
				& ~(1 << MOTOR_IN3)
				& ~(1 << MOTOR_IN4);

			motor_r = 0;
			motor_l = 0;
		}

		if (x_axis < 473)
		{
			uint8_t x_map = map(&x_axis, 473, 0, 0, 255);
			motor_l -= x_map;
			motor_r += x_map;
		}
		else if (x_axis > 550)
		{
			uint8_t x_map = map(&x_axis, 550, 1023, 0, 255);
			motor_l += x_map;
			motor_r -= x_map;
		}

		OCR1A = motor_r;
		OCR1B = motor_l;

		usart_puts("\tmotor_r: ");
		itoa(motor_r, str, 10);
		usart_puts(str);
		usart_puts("\tmotor_l: ");
		itoa(motor_l, str, 10);
		usart_puts(str);
		usart_puts("\r\n");
	}
	
	return 0;
}