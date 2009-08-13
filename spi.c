/*! \file spi.c \brief SPI interface driver. */
//*****************************************************************************
//
// File Name	: 'spi.c'
// Title		: SPI interface driver
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 11/22/2000
// Revised		: 06/06/2002
// Version		: 0.6
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>

#include "include/Settings.h"
#include "include/spi.h"
#include <util/delay.h>


// Define the SPI_USEINT key if you want SPI bus operation to be
// interrupt-driven.  The primary reason for not using SPI in
// interrupt-driven mode is if the SPI send/transfer commands
// will be used from within some other interrupt service routine
// or if interrupts might be globally turned off due to of other
// aspects of your program
//
// Comment-out or uncomment this line as necessary
//#define SPI_USEINT

// global variables
volatile uint8_t spiTransferComplete;

// SPI interrupt service handler
#ifdef SPI_USEINT
ISR(SPI_STC_vect)
{
	spiTransferComplete = TRUE;
}
#endif

// access routines
void spiInit()
{

	// setup SPI I/O pins
	sbi(PORTB, 7);	// set SCK hi
	sbi(DDRB, 7);	// set SCK as output
	cbi(DDRB, 6);	// set MISO as input
	sbi(DDRB, 5);	// set MOSI as output
	sbi(DDRB, 4);	// SS must be output for Master mode to work

	// setup SPI interface :
	// master mode
	sbi(SPCR, MSTR);
	// clock = f/4
//	cbi(SPCR, SPR0);
//	cbi(SPCR, SPR1);
	// clock = f/16
	cbi(SPCR, SPR0);

	sbi(SPCR, SPR1);
	// select clock phase positive-going in middle of data
	sbi(SPCR, CPOL);
	// Data order MSB first
	cbi(SPCR,DORD);
	// enable SPI
	sbi(SPCR, SPE);


	// some other possible configs
	//outp((1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );
	//outp((1<<CPHA)|(1<<CPOL)|(1<<MSTR)|(1<<SPE)|(1<<SPR0)|(1<<SPR1), SPCR );
	//outp((1<<CPHA)|(1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );

	// clear status
	inb(SPSR);
	spiTransferComplete = TRUE;

	// enable SPI interrupt
	#ifdef SPI_USEINT
	sbi(SPCR, SPIE);
	#endif
}

void spiSendByte(uint8_t data)
{
	// send a byte over SPI and ignore reply
	#ifdef SPI_USEINT
		while(!spiTransferComplete);
		spiTransferComplete = FALSE;
	#else
		while(!(inb(SPSR) & (1<<SPIF)));
	#endif

	outb(SPDR, data);
}

uint8_t spiTransferByte(uint8_t data)
{
	#ifdef SPI_USEINT
	// send the given data
	spiTransferComplete = FALSE;
	outb(SPDR, data);
	// wait for transfer to complete
	while(!spiTransferComplete);
	#else
	// send the given data
	outb(SPDR, data);
	// wait for transfer to complete
	while(!(inb(SPSR) & (1<<SPIF)));
	#endif
	// return the received data
	return inb(SPDR);
}

uint16_t spiTransferWord(uint16_t data)
{
	uint16_t rxData = 0;

	// send MS byte of given data
	rxData = (spiTransferByte((data>>8) & 0x00FF))<<8;
	// send LS byte of given data
	rxData |= (spiTransferByte(data & 0x00FF));

	// return the received data
	return rxData;
}

void spi_master_send_char (unsigned char c)
{
  SPDR = c;                    // sende Zeichen
  while (!(SPSR & (1<<SPIF)))  // warten bis Senden moeglich
  {
  }
  _delay_us(100);
}

void spi_init2() {
    sbi(PORTB, 7);	// set SCK hi
	sbi(DDRB, 7);	// set SCK as output
	cbi(DDRB, 6);	// set MISO as input
	sbi(DDRB, 5);	// set MOSI as output
	sbi(DDRB, 4);	// SS must be output for Master mode to work
  SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(0<<SPR1)|(0<<SPR0);
  SPSR = (0<<SPI2X);
  _delay_ms(100);
}
