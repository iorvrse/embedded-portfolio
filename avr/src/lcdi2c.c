#include "lcdi2c.h"
#include <util/delay.h>
#include "twi.h"

static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _displayfunction;
static uint8_t _backlightval;

void write4bits(uint8_t data)
{
	twi_start(LCD_ADDRESS);

	twi_write(data | LCD_EN | _backlightval);
    _delay_us(1);
    twi_write((data & ~LCD_EN) | _backlightval);
    _delay_us(50);

	twi_stop();
}

void lcd_command(uint8_t command)
{
    uint8_t nib = (command & 0xF0);
	write4bits(nib);

    nib = ((command << 4) & 0xF0);
	write4bits(nib);
}

void lcd_write(uint8_t data)
{
    uint8_t nib = (data & 0xF0) | LCD_RS;
	write4bits(nib);
	
    nib = ((data << 4) & 0xF0) | LCD_RS;
	write4bits(nib);
}

void lcd_init(void)
{
	_delay_ms(50);	// Wait for >15 ms after VCC rises to 4.5 V

	lcd_nobacklight();
	
	_delay_ms(1000);

	write4bits(0x30);
	_delay_us(4500);	// Wait for >4.1 ms
	write4bits(0x30);
	_delay_us(4500);	// Wait for >4.1 ms
	write4bits(0x30);
	_delay_us(150);		// Wait for >100 us
	write4bits(0x20);

	_displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
	lcd_command(LCD_FUNCTIONSET | _displayfunction);
	
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_display();

	lcd_clear();

	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
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

void lcd_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t row[] = {0x00, 0x40};
	lcd_command(LCD_SETDDRAMADDR | (x + row[y]));
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
	_displaycontrol &= ~LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_display(void)
{
	_displaycontrol |= LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_nocursor(void)
{
	_displaycontrol &= ~LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_cursor(void)
{
	_displaycontrol |= LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_noblink(void)
{
	_displaycontrol &= ~LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd_blink(void)
{
	_displaycontrol |= LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
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
	_displaymode |= LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_right_to_left(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}

void lcd_noautoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
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

void lcd_backlight(void)
{
	_backlightval = LCD_BACKLIGHT;
	twi_start(LCD_ADDRESS);
	twi_write(_backlightval);
	twi_stop();
}

void lcd_nobacklight(void)
{
	_backlightval = LCD_NOBACKLIGHT;
	twi_start(LCD_ADDRESS);
	twi_write(_backlightval);
	twi_stop();
}