#include <string.h>
#include "stm32f10x.h"
#include "i2c_lcd_driver.h"
#include "i2c.h"
#include "util.h"
#include "IOdef.h"
#include <stdio.h>

//void i2c_sendbyte(uint8_t addr, uint8_t data);
//void sleep(double);

//Data payload 

//D7	DB7
//D6	DB6
//D5	DB5
//D4	DB4
//D3	Backlight
//D2	EN
//D1	RW
//D0	RS


// Write an address and single byte to the i2c inteface

void lcd_i2c_write(uint8_t addr, uint8_t data)
{      i2c_sendbyte(addr, data);
      //sleep_us(100);
}
//# Write a command and argument
//   def write_cmd_arg(self, cmd, data):
//      bus.write_byte_data(addr, cmd, data)
//      sleep(0.0001)

//# Write a block of data
//   def write_block_data(self, cmd, data):
//      bus.write_block_data(addr, cmd, data)
//      sleep(0.0001)

//# Read a single byte
//   def read(self):
//      return bus.read_byte(addr)

//# Read
//   def read_data(self, cmd):
//      return bus.read_byte_data(addr, cmd)

//# Read a block of data
//   def read_block_data(self, cmd):
//      return bus.read_block_data(addr, cmd)


////# commands
//#define LCD_CLEARDISPLAY 0x01
//#define LCD_RETURNHOME 0x02
//#define LCD_ENTRYMODESET 0x04
//#define LCD_DISPLAYCONTROL 0x08
//#define LCD_CURSORSHIFT 0x10
//#define LCD_FUNCTIONSET 0x20
//#define LCD_SETCGRAMADDR 0x40
//#define LCD_SETDDRAMADDR 0x80
//#define LCD_LN1		0X80	//	;	Set DDRAM to start of line 1
//#define LCD_LN2		0XC0 //		; Set DDRAM to start of line 2


////# flags for display entry mode
//#define LCD_ENTRYRIGHT 0x00
//#define LCD_ENTRYLEFT 0x02
//#define LCD_ENTRYSHIFTINCREMENT 0x01
//#define LCD_ENTRYSHIFTDECREMENT 0x00

////# flags for display on/off control
//#define LCD_DISPLAYON 0x04
//#define LCD_DISPLAYOFF 0x00
//#define LCD_CURSORON 0x02
//#define LCD_CURSOROFF 0x00
//#define LCD_BLINKON 0x01
//#define LCD_BLINKOFF 0x00

////# flags for display/cursor shift
//#define LCD_DISPLAYMOVE 0x08
//#define LCD_CURSORMOVE 0x00
//#define LCD_MOVERIGHT 0x04
//#define LCD_MOVELEFT 0x00

////# flags for function set
//#define LCD_8BITMODE 0x10
//#define LCD_4BITMODE 0x00
//#define LCD_2LINE 0x08
//#define LCD_1LINE 0x00
//#define LCD_5x10DOTS 0x04
//#define LCD_5x8DOTS 0x00

////# flags for backlight control
//#define LCD_BACKLIGHT 0x08
//#define LCD_NOBACKLIGHT 0x00

//#define LCD_ENA 0x04 // # Enable bit
//#define LCD_RW 0x02 //# Read/Write bit
//#define LCD_RS_DAT 0x01 //# Register select bit
//#define LCD_RS_CMD 0x00 //# Register select bit


void lcd_init(uint8_t addr)
{
			//uint8_t addr = 0x27;
			sleep_ms(20);
			lcd_write_cmd(addr, 0x03);
			sleep_ms(10);
			lcd_write_cmd(addr, 0x03);
			sleep_us(1000);
			lcd_write_cmd(addr, 0x03);
			sleep_us(1000);      	
			lcd_write_cmd(addr, 0x02);
      sleep_us(1000);
      lcd_write_cmd(addr, LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE);
      lcd_write_cmd(addr, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON);
      lcd_write_cmd(addr, LCD_CLEARDISPLAY);
	    sleep_us(2000);
      lcd_write_cmd(addr, LCD_ENTRYMODESET | LCD_ENTRYLEFT);
	
 			GPIOA->BSRR = NUC_GREEN_ON;
			sleep_ms(2000);
			GPIOA->BSRR = NUC_GREEN_OFF;
}

//   # clocks EN to latch command

void lcd_strobe(uint8_t addr, uint8_t data)
{      
			lcd_i2c_write(addr , data | LCD_ENA | LCD_BACKLIGHT );
      sleep_us(100);
      lcd_i2c_write(addr , ((data & ~LCD_ENA ) | LCD_BACKLIGHT));
      sleep_us(100);
}
void lcd_write_four_bits(uint8_t addr, uint8_t data)
{     lcd_i2c_write(addr , (data | LCD_BACKLIGHT));
			sleep_us(100);
      lcd_strobe(addr, data);
}
  // # write a command to lcd
void lcd_write_cmd(uint8_t addr, uint8_t data)
{
      lcd_write_four_bits(addr , (LCD_RS_CMD | (data & 0xF0)));
      lcd_write_four_bits(addr , (LCD_RS_CMD | ((data << 4) & 0xF0)));

}
   //# write a character to lcd

void lcd_write_dat(uint8_t addr, uint8_t data)
{ 
      lcd_write_four_bits(addr , (LCD_RS_DAT | (data & 0xF0)));
      lcd_write_four_bits(addr , (LCD_RS_DAT | ((data << 4) & 0xF0)));
}
  
//   # put string function with optional char positioning
//   def lcd_display_string(self, string, line=1, pos=0):
//    if line == 1:
//      pos_new = pos
//    elif line == 2:
//      pos_new = 0x40 + pos
//    elif line == 3:
//      pos_new = 0x14 + pos
//    elif line == 4:
//      pos_new = 0x54 + pos

//    lcd_write(0x80 + pos_new)

//    for char in string:
//      lcd_write(ord(char), Rs)

//void stringToLCD(uint8_t addr,char * message)
//{
//	int i=0;
//	uint16_t messageLength = strlen(message);
//	for (i=0; i<messageLength; ++i)
//	{
//		//dataToLCD(*message);
//		lcd_write_dat(addr, *message);
//		++message;
//	}
//}

void intToLCD(uint8_t addr, int number)
{
	uint8_t my_lcd_addr = addr;
  char buffer[16]; // buffer to hold the number as a string
  sprintf(buffer, "%d", number); // convert the number to a string
  stringToLCD(my_lcd_addr,buffer);			// Output a string using a char array
}

void stringToLCD(uint8_t addr,char * message)
{
	int i=0;
	while(message[i] != 0)
		{
		//dataToLCD(*message);
		lcd_write_dat(addr, message[i]);
		i++;
		sleep_us(1000);	
		}
}

//   # clear lcd and set to home
//   def lcd_clear(self):
void lcd_clear_home(uint8_t addr)
{	
      lcd_write_cmd(addr, LCD_CLEARDISPLAY);
		  sleep_us(2000);
      lcd_write_cmd(addr, LCD_RETURNHOME);
		  sleep_us(2000);

}	
//   # define backlight on/off (lcd.backlight(1); off= lcd.backlight(0)
//   def backlight(self, state): # for state, 1 = on, 0 = off
//      if state == 1:
//         lcd_device.write_cmd(LCD_BACKLIGHT)
//      elif state == 0:
//         lcd_device.write_cmd(LCD_NOBACKLIGHT)

//   # add custom characters (0 - 7)
//   def lcd_load_custom_chars(self, fontdata):
//      lcd_write(0x40);
//      for char in fontdata:
//         for line in char:
//            lcd_write_char(line) 