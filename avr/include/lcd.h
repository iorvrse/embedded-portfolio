#ifndef _LCD_H_
#define _LCD_H_

#ifndef F_CPU
#define F_CPU   8000000UL
#endif /* F_CPU */

#include <avr/io.h>

#if ( !defined LCD_DDR || !defined LCD_PORT )
#warning "LCD_DDR & LCD_PORT not defined! Using default port (PORTD)"
#define LCD_DDR		DDRD
#define LCD_PORT	PORTD
#endif

#ifndef LCD_RS
#warning "LCD_RS, LCD_RW, & LCD_EN not defined! LCD using default pin"
#define LCD_RS		0
#define LCD_RW		1
#define	LCD_EN		2
#endif

// commands
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// flags for entry mode set
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display control
#define LCD_DISPLAYON   0x04
#define LCD_DISPLAYOFF  0x00
#define LCD_CURSORON    0x02
#define LCD_CURSOROFF   0x00
#define LCD_BLINKON     0x01
#define LCD_BLINKOFF    0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// flags for function set
#define LCD_8BITMODE    0x10
#define LCD_4BITMODE    0x00
#define LCD_2LINE       0x08
#define LCD_1LINE       0x00
#define LCD_5x10DOTS    0x04
#define LCD_5x8DOTS     0x00

uint8_t displaycontrol;
uint8_t displaymode;
uint8_t displayfunction;

void lcd_command(uint8_t command);

void lcd_write(uint8_t data);

void lcd_init(void);

void lcd_gotoxy(uint8_t x, uint8_t y);

void lcd_puts(uint8_t *str);

void lcd_clear(void);

void lcd_return_home(void);

void lcd_nodisplay(void);

void lcd_display(void);

void lcd_nocursor(void);

void lcd_cursor(void);

void lcd_noblink(void);

void lcd_blink(void);

void lcd_scrolldisplay_left(void);

void lcd_scrolldisplay_right(void);

void lcd_left_to_right(void);

void lcd_right_to_left(void);

void lcd_autoscroll(void);

void lcd_noautoscroll(void);

void lcd_create_char(uint8_t loc, uint8_t charmap[]);

#endif  /* _LCD_H_ */