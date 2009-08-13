#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <avr/io.h>
#include <avr/pgmspace.h>

/////////////////////////////////////////////////////////////////////
//Select which of the available fonts will be included
/////////////////////////////////////////////////////////////////////
//If a font is not needed, then you don't need to put it to the Flash
//Just comment one of the defines below

#define FONTS_INCLUDE_font_proportional_16px
#define FONTS_INCLUDE_font_proportional_8px
#define FONTS_INCLUDE_font_fixed_8px
#define FONTS_INCLUDE_symbols_8px
#define FONTS_INCLUDE_symbols_16px




/////////////////////////////////////////////////////////////////////
//Structure holds information about fonts
/////////////////////////////////////////////////////////////////////


struct font_info {
    uint16_t size;       //size of data array
    uint8_t  width;      //(maximum) width of character
    uint8_t  height;     //height of character
    uint8_t  firstchar;  //the number of the first included character (often 0x20)
    uint8_t  lastchar;   //the last included character (often 0xFF)
    PGM_P    widthtable; //Pointer to the table holding character widths (NULL for monospaced fonts)
    PGM_P    data;       //Pointer to data arrray
    };
typedef const struct font_info PROGMEM* FONT_P;


/////////////////////////////////////////////////////////////////////
//All font structures + verbose name defines
/////////////////////////////////////////////////////////////////////
#ifdef FONTS_INCLUDE_font_fixed_8px
  extern const struct font_info font_fixed_8px PROGMEM ;
  #define FONT_FIXED_8 &font_fixed_8px
#endif

#ifdef FONTS_INCLUDE_font_proportional_16px
  extern const struct font_info font_proportional_16px PROGMEM ;
  #define FONT_PROP_16 &font_proportional_16px
#endif

#ifdef FONTS_INCLUDE_font_proportional_8px
  extern const struct font_info font_proportional_8px PROGMEM;
  #define FONT_PROP_8 &font_proportional_8px
#endif

#ifdef FONTS_INCLUDE_symbols_8px
  extern const struct font_info symbols_8px PROGMEM;
  #define FONT_SYMBOL_8 &symbols_8px
#endif

#ifdef FONTS_INCLUDE_symbols_16px
  extern const struct font_info symbols_16px PROGMEM;
  #define FONT_SYMBOL_16 &symbols_16px
#endif


/////////////////////////////////////////////////////////////////////
//Functions to move cursor and write data to LCD
/////////////////////////////////////////////////////////////////////
#define LCD_MOVE(x,y)   lcd_move_xy((x),(y))
#define LCD_WRITE(x)    lcd_data((x))

extern void lcd_move_xy  (int8_t page, int8_t column);
extern void lcd_data     (uint8_t data);


/////////////////////////////////////////////////////////////////////
//Internal Functions to locate and display characters
/////////////////////////////////////////////////////////////////////

/*
PGM_P   font_data              (FONT_P charset);
PGM_P   font_widthtable        (FONT_P charset);
uint8_t font_get_height_bytes  (FONT_P charset);
uint8_t font_get_type          (FONT_P charset);
uint8_t font_get_add_space     (FONT_P charset, unsigned char character);
uint8_t font_get_char_width    (FONT_P charset, unsigned char character);
int16_t font_get_char_number   (FONT_P charset, unsigned char character);
PGM_P   font_get_char_position (FONT_P charset, unsigned char character);
*/



/////////////////////////////////////////////////////////////////////
//Functions to display characters and strings
/////////////////////////////////////////////////////////////////////
void lcd_put_string   (FONT_P font, uint8_t size, char* str);
void lcd_put_string_P (FONT_P font, uint8_t size, PGM_P str);
void lcd_put_char     (FONT_P font, uint8_t size, unsigned char c);

//where font is a pointer to a FONT_P struct,
//size is one of the following four defines for different font sizes
//and str and c are the string or the character to be displayed


/////////////////////////////////////////////////////////////////////
//Define font sizes
/////////////////////////////////////////////////////////////////////
#define NORMAL_SIZE   0
#define DOUBLE_HEIGHT 1
#define DOUBLE_WIDTH  2
#define DOUBLE_SIZE   3

#define DOUBLE_HEIGHT_BIT 1
#define DOUBLE_WIDTH_BIT  2

/////////////////////////////////////////////////////////////////////
//Documentation: Example Font File
//A c-template to use with Hagen Reddmanns font generator is
//included (template_simplefont.c)
/////////////////////////////////////////////////////////////////////
/*
#include "font.h"
#ifdef %FONT INCLUDE IDENTIFIER%
const char fontname_length[] PROGMEM = {...}
const char fontname_data[] PROGMEM = {...}
const struct font_info fontname PROGMEM = {...};
#endif
*/



#endif
