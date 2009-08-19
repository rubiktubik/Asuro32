/*
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

#include <avr/io.h>
#include <stdio.h>
#include <util\delay.h>
#include "include\uart.h"
#include "include\Settings.h"
#include "include\spi.h"
#include "include\lcd.h"


int main(void)
{
    DDRB = 0xFF;
    PORTB = 0xFF;
    DDRA = 0xFF;
    PORTA =0xFF;

    InitSpi();
    LcdInit();
    _delay_ms(1000);

    disp_clear();
    _delay_ms(1000);

    while(1)
    ;

    return 0;
}

/*~~~~~~~~~~~~~~~~*/

	while(1)
    {



    }



    return 0;
}
