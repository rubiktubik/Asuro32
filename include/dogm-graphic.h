#ifndef DOGMGRAPHIC_H_INCLUDED
#define DOGMGRAPHIC_H_INCLUDED

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "font.h"

/////////////////////////////////////////////////////////////////////
//Set these defines according to the hardware setup:
/////////////////////////////////////////////////////////////////////
//size of the LCD
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

//size of LCD RAM
#define LCD_RAM_PAGES 8

//A0 Port
#define PORT_A0  PORTA
#define DDR_A0   DDRA
#define PIN_A0   4

//Reset Port
#define PORT_RST PORTA
#define DDR_RST  DDRA
#define PIN_RST  7



//Wait until SPI interface is idle
#define spi_wait_for_idle() while(! (SPSR & _BV(SPIF)))
//Write to SPI data register
#define spi_write(i) SPDR = i
//End hardware setup


//SPI interface has to be initialized on startup!

//Example SPI setup (Atmega162)
//init spi: msb first, update falling, read rising, 9 MHz


/////////////////////////////////////////////////////////////////////
//Public functions
/////////////////////////////////////////////////////////////////////

//initializes the display in 6 o'clock mode & 4x booster for 2.4-3.3V supply voltage
//for EA-DOGM132 and EA_DOGM128
void lcd_init();

//write data word or command to the LCD
void lcd_data     (uint8_t data);
void lcd_command  (uint8_t cmd);

//User functions to write raw data from flash
void lcd_put_raw     (PGM_P data, uint8_t length);

//Move cursor
void lcd_moveto_xy  (uint8_t page, uint8_t column);
void lcd_move_xy    (int8_t pages, int8_t columns);
void spi_start();
//Text functions are included in font.c / font.h




/////////////////////////////////////////////////////////////////////
//Command Codes
/////////////////////////////////////////////////////////////////////

//1: Display on/off
#define LCD_DISPLAY_ON       0xAF  //switch display on
#define LCD_DISPLAY_OFF      0xAE  //switch display off

//2: display start line set (lower 6 bits select first line on lcd from 64 lines in memory)
#define LCD_START_LINE       0x40

//3: Page address set (lower 4 bits select one of 8 pages)
#define LCD_PAGE_ADDRESS     0xB0

//4: column address (lower 4 bits are upper / lower nibble of column address)
#define LCD_COL_ADDRESS_MSB  0x10
#define LCD_COL_ADDRESS_LSB  0x00  //second part of column address

//8: select orientation (black side of the display should be further away from viewer)
#define LCD_BOTTOMVIEW    0xA1  //6 o'clock view
#define LCD_TOPVIEW       0xA0  //12 o'clock view

//9: select normal (white background, black pixels) or reverse (black background, white pixels) mode
#define LCD_DISPLAY_POSITIVE 0xA6  //not inverted mode
#define LCD_DISPLAY_INVERTED 0xA7  //inverted display

//10: show memory content or switch all pixels on
#define LCD_SHOW_NORMAL      0xA4  //show dram content
#define LCD_SHOW_ALL_POINTS  0xA5  //show all points

//11: lcd bias set
#define LCD_BIAS_1_9         0xA2
#define LCD_BIAS_1_7         0xA3

//14: Reset Controller
#define LCD_RESET_CMD        0xE2

//15: output mode select (turns display upside-down)
#define LCD_SCAN_DIR_NORMAL  0xC0  //normal scan direction
#define LCD_SCAN_DIR_REVERSE 0xC8  //reversed scan direction

//16: power control set (lower 3 bits select operating mode)
//Bit 0: Voltage follower on/off - Bit 1: Voltage regulator on/off - Bit 2: Booster circuit on/off
#define LCD_POWER_CONTROL     0x28  //base command
#define LCD_POWER_LOW_POWER   0x2F
#define LCD_POWER_WIDE_RANGE  0x2F
#define LCD_POWER_LOW_VOLTAGE 0x2B

//17: voltage regulator resistor ratio set (lower 3 bits select ratio)
//selects lcd voltage - 000 is low (~ -2V), 111 is high (~ - 10V), also depending on volume mode. Datasheet suggests 011
#define LCD_VOLTAGE          0x20

//18: Volume mode set (2-byte command, lower 6 bits in second word select value, datasheet suggests 0x1F)
#define LCD_VOLUME_MODE_1    0x81
#define LCD_VOLUME_MODE_2    0x00

//19: static indicator (2-byte command), first on/off, then blinking mode
#define LCD_INDICATOR_ON       0xAD  //static indicator on
#define LCD_INDICATOR_OFF      0xAC  //static indicator off
#define LCD_INDICATOR_MODE_OFF 0x00
#define LCD_INDICATOR_MODE_1HZ 0x01
#define LCD_INDICATOR_MODE_2HZ 0x10
#define LCD_INDICATOR_MODE_ON  0x11

//20: booster ratio set (2-byte command)
#define LCD_BOOSTER_SET      0xF8  //set booster ratio
#define LCD_BOOSTER_234      0x00  //2x-4x
#define LCD_BOOSTER_5        0x01  //5x
#define LCD_BOOSTER_6        0x03  //6x

//22: NOP command
#define LCD_NOP              0xE3



/////////////////////////////////////////////////////////////////////
//Makros to execute commands
/////////////////////////////////////////////////////////////////////

#define LCD_SWITCH_ON()              lcd_command(LCD_DISPLAY_ON)
#define LCD_SWITCH_OFF()             lcd_command(LCD_DISPLAY_OFF)
#define LCD_SET_FIRST_LINE(i)        lcd_command(LCD_START_LINE | ((i) & 0x3F))
#define LCD_SET_PAGE_ADDR(i)         lcd_command(LCD_PAGE_ADDRESS | ((i) & 0x0F))
#define LCD_SET_COLUMN_ADDR(i)       lcd_command(LCD_COL_ADDRESS_MSB | ((i>>4) & 0x0F)); \
                                     lcd_command(LCD_COL_ADDRESS_LSB | ((i) & 0x0F))
#define LCD_GOTO_ADDRESS(page,col);  lcd_command(LCD_PAGE_ADDRESS | ((page) & 0x0F)); \
                                     lcd_command(LCD_COL_ADDRESS_MSB | ((col>>4) & 0x0F)); \
                                     lcd_command(LCD_COL_ADDRESS_LSB | ((col) & 0x0F));

#define LCD_SET_BOTTOM_VIEW()        lcd_command(LCD_BOTTOMVIEW)
#define LCD_SET_TOP_VIEW()           lcd_command(LCD_TOPVIEW)
#define LCD_SET_MODE_POSITIVE()      lcd_command(LCD_DISPLAY_POSITIVE)
#define LCD_SET_MODE_INVERTED()      lcd_command(LCD_DISPLAY_INVERTED)
#define LCD_SHOW_ALL_PIXELS_ON()     lcd_command(LCD_SHOW_ALL_POINTS)
#define LCD_SHOW_ALL_PIXELS_OFF()    lcd_command(LCD_SHOW_NORMAL)
#define LCD_SET_BIAS_RATIO_1_7()     lcd_command(LCD_BIAS_1_7)
#define LCD_SET_BIAS_RATIO_1_9()     lcd_command(LCD_BIAS_1_9)
#define LCD_RESET()                  lcd_command(LCD_RESET_CMD)
#define LCD_ORIENTATION_NORMAL()     lcd_command(LCD_SCAN_DIR_NORMAL)
#define LCD_ORIENTATION_UPSIDEDOWN() lcd_command(LCD_SCAN_DIR_REVERSE)
#define LCD_SET_POWER_CONTROL(i)     lcd_command(LCD_POWER_CONTROL | ((i) & 0x07))
#define LCD_SET_LOW_POWER()          lcd_command(LCD_POWER_LOW_POWER)
#define LCD_SET_WIDE_RANGE()         lcd_command(LCD_POWER_WIDE_RANGE)
#define LCD_SET_LOW_VOLTAGE()        lcd_command(LCD_POWER_LOW_VOLTAGE)
#define LCD_SET_BIAS_VOLTAGE(i)      lcd_command(LCD_VOLTAGE | ((i) & 0x07))
#define LCD_SET_VOLUME_MODE(i)       lcd_command(LCD_VOLUME_MODE_1); \
                                     lcd_command(LCD_VOLUME_MODE_2 | ((i) & 0x3F))
#define LCD_SET_INDICATOR_OFF()      lcd_command(LCD_INDICATOR_OFF); \
                                     lcd_command(LCD_INDICATOR_MODE_OFF)
#define LCD_SET_INDICATOR_STATIC()   lcd_command(LCD_INDICATOR_ON); \
                                     lcd_command(LCD_INDICATOR_MODE_ON)
#define LCD_SET_INDICATOR_1HZ()      lcd_command(LCD_INDICATOR_ON); \
                                     lcd_command(LCD_INDICATOR_MODE_1HZ)
#define LCD_SET_INDICATOR_2HZ()      lcd_command(LCD_INDICATOR_ON); \
                                     lcd_command(LCD_INDICATOR_MODE_2HZ)
#define LCD_SET_INDICATOR(i,j)       lcd_command(LCD_INDICATOR_OFF | ((i) & 1)); \
                                     lcd_command(((j) & 2))

#define LCD_SLEEP_MODE             lcd_command(LCD_INDICATOR_OFF); \
                                   lcd_command(LCD_DISPLAY_OFF); \
                                   lcd_command(LCD_SHOW_ALL_POINTS)

//Control A0 input of LCD
#define LCD_DRAM() PORT_A0 |= _BV(PIN_A0);
#define LCD_CMD()  PORT_A0 &= ~_BV(PIN_A0);
//Control reset input of LCD
#define LCD_RESET_OFF()  PORT_RST |= _BV(PIN_RST);
#define LCD_RESET_ON()   PORT_RST &= ~_BV(PIN_RST);


#endif
