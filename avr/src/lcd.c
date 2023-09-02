#include "lcd.h"
#include <util/delay.h>

void write4bits(uint8_t data)
{
	LCD_PORT = data | _BV(LCD_EN);
	_delay_us(1);
	LCD_PORT = data & ~_BV(LCD_EN);
	_delay_us(50);
}

void lcd_command(uint8_t command)
{
	uint8_t nib = (command & 0xF0) & ~(_BV(LCD_RS) | _BV(LCD_RW));
	write4bits(nib);

	nib = (command << 4) & ~(_BV(LCD_RS) | _BV(LCD_RW));
	write4bits(nib);
}

void lcd_write(uint8_t data)
{
	uint8_t nib = (data & 0xF0) | (_BV(LCD_RS) & ~_BV(LCD_RW));
	write4bits(nib);
	
	nib = (data << 4) | (_BV(LCD_RS) & ~_BV(LCD_RW));
	write4bits(nib);
}

void lcd_init(void)
{
	LCD_DDR = 0xF7;
	_delay_ms(15);

	write4bits(0x30);
	_delay_us(4100);
	write4bits(0x30);
	_delay_us(4100);
	write4bits(0x30);
	_delay_us(100);
	write4bits(0x20);

	displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
	lcd_command(LCD_FUNCTIONSET | displayfunction);
	
	displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);

	lcd_clear();

	displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	lcd_command(LCD_ENTRYMODESET | displaymode);
}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t row[] = {0x00, 0x40};
	lcd_command(LCD_SETDDRAMADDR | (x + row[y]));
}


void lcd_clear(void)
{
	lcd_command(LCD_CLEARDISPLAY);
	_delay_ms(2);
}

void lcd_return_home(void)
{
	lcd_command(LCD_RETURNHOME);
	_delay_ms(2);
}

void lcd_puts(uint8_t *str)
{
	uint8_t i = 0;
	while (str[i] != '\0')
	{
		lcd_write(str[i]);
		i++;
	}
}

void lcd_nodisplay(void)
{
	displaycontrol &= ~LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_display(void)
{
	displaycontrol |= LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_nocursor(void)
{
	displaycontrol &= ~LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_cursor(void)
{
	displaycontrol |= LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_noblink(void)
{
	displaycontrol &= ~LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_blink(void)
{
	displaycontrol |= LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | displaycontrol);
}

void lcd_scrolldisplay_left(void)
{
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scrolldisplay_right(void)
{
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_left_to_right(void)
{
	displaymode |= LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | displaymode);
}

void lcd_right_to_left(void)
{
	displaymode &= ~LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | displaymode);
}

void lcd_autoscroll(void)
{
	displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | displaymode);
}

void lcd_noautoscroll(void)
{
	displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | displaymode);
}

void lcd_create_char(uint8_t loc, uint8_t charmap[])
{
	loc &= 0x7;
	lcd_command(LCD_SETCGRAMADDR | loc << 3);
	for (uint8_t i = 0; i < 8; i++)
	{
		lcd_write(charmap[i]);
	}
	
}