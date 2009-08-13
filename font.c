//=============================================================================
//Font Generator
//designed for GLCD with memory organized in vertical bytes
//
//Own character sets or symbols can easily be added:
//The structure of font data is compatible with Hagen Reddmanns FontEditor 
//http://www.mikrocontroller.net/topic/99701#865331
//when using the attached template_simplefont.c and template_simplefont.h files,
//saving in uncompressed format and using font heights of multiples of 8.
//
//Fixed width fonts are treated as proportional fonts, but do not have a table 
//for the width of each character (D'OH!)
//
//When using with other GLCDs, make sure the byte orientation of the LCDs memory
//matches the design of the ea-dogm series or link the LCD access functions (see 
//header file) to functions converting the data.
//
//My thanks go to Hagen Reddmann for his Font Editor software 
//(http://www.mikrocontroller.net/topic/99701#865331)
//and Benedikt K. for his various LCD fonts 
//(http://www.mikrocontroller.net/topic/54860)
//
//Author:  Jan Michel (jan at mueschelsoft dot de)
//License: GNU General Public License, version 3
//Version: v0.9 July 2009
//=============================================================================





#include "font.h"


//=============================================================================
//Helper Functions to find, retrieve and display characters
//=============================================================================

//load pointer to data table
inline PGM_P font_data(FONT_P charset) {
  PGM_P tmp;
  if (sizeof(tmp) == 2)
    tmp = (PGM_P)pgm_read_word(&(charset->data));
  else
    memcpy_P((char*)&tmp,&(charset->data),sizeof(tmp));
  return tmp;
  }



//load pointer to width table
inline PGM_P font_widthtable(FONT_P charset) {
  PGM_P tmp;
  if (sizeof(tmp) == 2)
    tmp = (PGM_P)pgm_read_word(&(charset->widthtable));
  else
    memcpy_P((char*)&tmp,&(charset->widthtable),sizeof(tmp));
  return tmp;
  }
  


//load height of font in bytes
inline uint8_t font_get_height_bytes(FONT_P charset) {
  uint8_t t = pgm_read_byte(&charset->height);
  return ((t-1)/8+1);
  }



//is additional space needed to the right of the character?
inline uint8_t font_get_add_space(FONT_P charset, unsigned char character) {
  PGM_P type = font_widthtable(charset);
  if ( type != 0 ) //if there is a width table, then it's a proportional font
    return 1;
  else
    return 0;
  }



//get character number in charset
inline int16_t font_get_char_number(FONT_P charset, unsigned char character) {
  uint8_t first = pgm_read_byte(&charset->firstchar);
  uint8_t last  = pgm_read_byte(&charset->lastchar);
  if (character < first || character > last) return -1;
  else return character - first;
  }



//get width of character
inline uint8_t font_get_char_width(FONT_P charset, unsigned char character) {
  PGM_P table = font_widthtable(charset);
  if (table)
    return pgm_read_byte(table+font_get_char_number(charset,character));
  else
    return pgm_read_byte(&charset->width);
  }



//find position of character in flash
PGM_P font_get_char_position(FONT_P charset, unsigned char character) {
  uint16_t ret         = 0;
  int16_t  charnum_ret = font_get_char_number(charset, character);
  uint8_t  charnum     = charnum_ret;
  PGM_P    base        = font_widthtable(charset);

  if (charnum_ret < 0)          //char not found
    return 0;
  if(base == 0)            //fixed width
    return font_data(charset) + (uint16_t)charnum * (uint8_t)(font_get_height_bytes(charset) * font_get_char_width(charset,character));
  if (charnum)             //proportional width
    while(charnum--)
      ret += pgm_read_byte(base++);
  return (font_data(charset))+ret*font_get_height_bytes(charset);
  }



//for double-height fonts: double all bits of a upper or lower nibble
inline unsigned char double_bits(uint8_t part, unsigned char c) {
  unsigned char t = 0;
  if (part) c >>= 4;
  if (c & 0x08) t |= 0xC0;
  if (c & 0x04) t |= 0x30;
  if (c & 0x02) t |= 0x0C;
  if (c & 0x01) t |= 0x03;
  return t;
  }

//=============================================================================
//Puts a character of a font to the display
//=============================================================================
void lcd_put_char(FONT_P charset, uint8_t size, unsigned char character) {
  int8_t  i;
  uint8_t row = 0; //current row of char
  uint8_t hc  = (size & DOUBLE_HEIGHT_BIT)?1:0; //height changed
  uint8_t wc  = (size & DOUBLE_WIDTH_BIT)?1:0; //width changed
  uint8_t tmp;

  //load information about character
  uint8_t char_width    = font_get_char_width(charset,character); 
  uint8_t font_height   = font_get_height_bytes(charset);
  uint8_t free_space    = font_get_add_space(charset,character);
  PGM_P   tableposition = font_get_char_position(charset,character);

  //final size of character
  uint8_t char_final_width  = (char_width+free_space) << wc;
  uint8_t char_final_height = font_height << hc; 

  //write chracter
  do {
    for(i=(row>>hc);i<char_width*font_height;i+=font_height) {
      tmp = pgm_read_byte(tableposition+i);
      if(hc)
        tmp = double_bits((row&1),tmp);
      LCD_WRITE(tmp);
      if(wc) 
        LCD_WRITE(tmp);
      }
    if (free_space) {
      LCD_WRITE(0);
      if(wc) 
        LCD_WRITE(0);
      }
    LCD_MOVE(1,-char_final_width);
    } while (++row < char_final_height);

  //move cursor to upper right corner of character
  LCD_MOVE(-char_final_height,char_final_width);
  }




//=============================================================================
//Puts a string from RAM or Flash to the display
//=============================================================================
void lcd_put_string_P(FONT_P font, uint8_t size, PGM_P str) {
  unsigned char t;
  while((t = pgm_read_byte(str++))) 
    lcd_put_char(font,size,t);
  return;
  }


void lcd_put_string(FONT_P font, uint8_t size, char* str) {
  unsigned char t;
  while((t = *str++))
    lcd_put_char(font,size,t);
  return;
  }


