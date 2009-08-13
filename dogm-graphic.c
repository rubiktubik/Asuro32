//=============================================================================
//Display Library
//for EA-DOGM132 GLCD (132px x 32px)
//and EA-DOGM128 GLCD (128px x 64px)
//
//Provides all basic functions to access the display
//Since no graphics ram is used, the memory footprint is rather small but also
//does not allow to change single pixels. Data written to the LCD can not be
//read back!
//Text can be displayed using the attached font generator containing several
//character sets and font variants.
//
//Author:  Jan Michel (jan at mueschelsoft dot de)
//License: GNU General Public License, version 3
//Version: v0.9 July 2009
//=============================================================================


#include "include/dogm-graphic.h"


#include "include/Settings.h"
#include <util/delay.h>
//=============================================================================
//keeping track of current position in ram - necessary for big fonts & bitmaps
//=============================================================================

uint8_t lcd_current_page = 0;
uint8_t lcd_current_column = 0;


//change page address by s
uint8_t lcd_inc_page(int8_t s) {
  lcd_current_page += s;
  while (lcd_current_page >= LCD_RAM_PAGES)
    lcd_current_page -= LCD_RAM_PAGES;
  return lcd_current_page;
  }

//change column address by s,
uint8_t lcd_inc_column(int8_t s) {
  lcd_current_column += s;
//  while (lcd_current_column >= LCD_WIDTH)
//    lcd_current_column -= LCD_WIDTH;
  return lcd_current_column;
  }


//Move cursor to new position and update internal address
void lcd_moveto_xy(uint8_t page, uint8_t column) {
  LCD_GOTO_ADDRESS(page,column);
  lcd_current_column = column;
  lcd_current_page = page;
  }

void lcd_move_xy(int8_t pages, int8_t columns) {
  lcd_moveto_xy(lcd_inc_page(pages),lcd_inc_column(columns));
  }


//=============================================================================
//Basic Byte Access to Display
//=============================================================================

//Writes one byte to the data ram
void lcd_data(uint8_t data) {
  spi_wait_for_idle();
  LCD_DRAM();
  spi_write(data);
  lcd_inc_column(1);
  }

//Writes one command byte
void lcd_command(uint8_t cmd) {
  spi_wait_for_idle();
  LCD_CMD();
  spi_write(cmd);
  }


//=============================================================================
//Puts raw data from Flash to the Display
//=============================================================================
void lcd_put_raw (PGM_P data, uint8_t length) {
  lcd_inc_column(length);
  while(length--)
    lcd_data(pgm_read_byte(data++));
  }


//=============================================================================
//initializes the display in 6 o'clock mode, 4x booster for 2.4-3.3V supply voltage
//scheme according to datasheet
//Compatible to both Display types: 128x64 and 132x32
//=============================================================================
void lcd_init() {
  DDR_A0  |= _BV(PIN_A0);             //set outputs
  DDR_RST |= _BV(PIN_RST);

  LCD_RESET_ON();                     //Apply Reset to the Display Controller
  _delay_ms(1);
  LCD_RESET_OFF();

  LCD_SET_BOTTOM_VIEW();              //6 o'clock mode, normal orientation
  LCD_ORIENTATION_NORMAL();
  LCD_SET_MODE_INVERTED();           //positive display
  LCD_SET_BIAS_RATIO_1_9();           //bias 1/9
  LCD_SET_POWER_CONTROL(7);           //power control mode: all features on
  LCD_SET_BIAS_VOLTAGE(3);            //set voltage regulator R/R
  LCD_SET_VOLUME_MODE(0x1F);          //volume mode set
  LCD_SET_INDICATOR_OFF();            //switch indicator off, no blinking
  LCD_SWITCH_ON();                    //Switch display on
  uint8_t i,j;                        //clear display content
  for(j=0;j<8;j++) {
    lcd_moveto_xy(j,0);
    for (i=0;i<132;i++)
      lcd_data(0);
    }
  lcd_moveto_xy(0,0);                 //Set write pointer
  LCD_SET_FIRST_LINE(0);              //first bit in RAM is on the first line of the LCD


  return;
  }

void spi_start()
{
    sbi(PORTB, 7);	// set SCK hi
	sbi(DDRB, 7);	// set SCK as output
	cbi(DDRB, 6);	// set MISO as input
	sbi(DDRB, 5);	// set MOSI as output
	sbi(DDRB, 4);	// SS must be output for Master mode to work

SPCR = 0 << SPIE | 1 << SPE | 0 << DORD | 1 << MSTR | 1 << CPOL | 1 << CPHA | 0 << SPR1 | 0 << SPR0;
SPSR = 1 << SPI2X;
SPDR = 0;
_delay_ms(100);
}
