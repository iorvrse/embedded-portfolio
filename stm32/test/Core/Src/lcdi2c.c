#include "lcdi2c.h"

void write4bits(uint8_t data)
{
	I2C_Start(LCD_I2C);
    I2C_SendAddress(LCD_I2C, LCD_ADDRESS);

	I2C_TransmitByte(LCD_I2C, data | LCD_EN | _backlightval);
    Delay_us(1);
    I2C_TransmitByte(LCD_I2C, (data & ~LCD_EN) | _backlightval);
    Delay_us(50);

	I2C_Stop(LCD_I2C);
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
	Delay_ms(50);		// Wait for >15 ms after VCC rises to 4.5 V

	lcd_nobacklight();
	
	Delay_ms(1000);

	write4bits(0x30);
	Delay_us(4500);		// Wait for >4.1 ms
	write4bits(0x30);
	Delay_us(4500);		// Wait for >4.1 ms
	write4bits(0x30);
	Delay_us(150);		// Wait for >100 us
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
	Delay_ms(2);
}

void lcd_return_home(void)
{
	lcd_command(LCD_RETURNHOME);
	Delay_ms(2);
}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t row[] = {0x00, 0x40};
	lcd_command(LCD_SETDDRAMADDR | (x + row[y]));
}

void lcd_puts(char * str)
{
	while (*str)
	{
		lcd_write(*str);
		str++;
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
	I2C_Start(LCD_I2C);
    I2C_SendAddress(LCD_I2C, LCD_ADDRESS);
	I2C_TransmitByte(LCD_I2C, _backlightval);
	I2C_Stop(LCD_I2C);
}

void lcd_nobacklight(void)
{
	_backlightval = LCD_NOBACKLIGHT;
	I2C_Start(LCD_I2C);
    I2C_SendAddress(LCD_I2C, LCD_ADDRESS);
	I2C_TransmitByte(LCD_I2C, _backlightval);
	I2C_Stop(LCD_I2C);
}