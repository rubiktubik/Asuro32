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
#include "include\spi.h"
#include "include\dogm-graphic.h"

//Typen durch sizeof:   uint8 = 1Byte
//                      uint16 = 2Byte
//                      char = 1Byte
//                      float = 4Byte


int main(void)
{

/*~~~~~~~~~~~~~~~~*/


spi_init2();
lcd_init();


/*~~~~~~~~~~~~~~~~*/

	while(1)
    {



    }



    return 0;
}
