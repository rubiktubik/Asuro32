#ifndef LCD_H
#define LCD_H

void LcdInit();
void LcdData(char Byte);
void LcdBefehl(char Byte);
void disp_send_frame();
void disp_clear();
#endif
