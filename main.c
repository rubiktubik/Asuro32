/*
 */
#include <avr/io.h>
#include <stdio.h>
#include <util\delay.h>
#include "include\uart.h"
#include "include\Settings.h"
#include "include\spi.h"
#include "include\lcd.h"
#include "include\Settings.h"

//Typen durch sizeof:   uint8 = 1Byte
//                      uint16 = 2Byte
//                      char = 1Byte
//                      float = 4Byte

int main(void)
{

/*~~~~~~~~~~~~~~~~*/

/*
 */

    DDRB = 0xFF;
    PORTB = 0xFF;
    DDRA = 0xFF;
    PORTA =0xFF;

    InitSpi();
    LcdInit();
    _delay_ms(1000);


    while(1)
    ;

    return 0;

}
