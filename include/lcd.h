#ifndef LCD_H
#define LCD_H

//Definition der PINs zum Display
#define A0 PA4
#define CS PA5
#define RESET PA7

void LcdInit();
void LcdData(char Byte);
void LcdBefehl(char Byte);
void LcdSetPage(uint8_t Page);
#endif
