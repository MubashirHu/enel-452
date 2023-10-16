#include "stm32f10x.h"
void intToLCD(uint8_t addr, int number);
void lcd_i2c_write(uint8_t addr, uint8_t data);
void lcd_init(uint8_t addr);
void lcd_strobe(uint8_t addr, uint8_t data);
void lcd_write_four_bits(uint8_t addr, uint8_t data);
void lcd_write_cmd(uint8_t addr, uint8_t data);
void lcd_write_dat(uint8_t addr, uint8_t data);
void stringToLCD(uint8_t addr,char * message);
void lcd_clear_home(uint8_t addr);

//# commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define LCD_LN1		0X80	//	;	Set DDRAM to start of line 1
#define LCD_LN2		0XC0 //		; Set DDRAM to start of line 2


//# flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

//# flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

//# flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

//# flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

//# flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define LCD_ENA 0x04 // # Enable bit
#define LCD_RW 0x02 //# Read/Write bit
#define LCD_RS_DAT 0x01 //# Register select bit
#define LCD_RS_CMD 0x00 //# Register select bit