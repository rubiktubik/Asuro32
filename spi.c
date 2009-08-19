#include <avr\io.h>
#include "include\Settings.h"
#include "include\spi.h"
#include <util\delay.h>

void InitSpi()
{
   SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(1<<SPR0);
   SPSR =(0<<SPI2X);
}


void spi_master_send_char (unsigned char c)
{
  SPDR = c;                    // sende Zeichen
  while (!(SPSR & (1<<SPIF)))  // warten bis Senden moeglich
  {
  }
  _delay_us(100);
}