#include <avr\io.h>
#include "include\Settings.h"
#include <util\delay.h>
#include "include\lcd.h"
#include "include\spi.h"

uint8_t disp_ram[1024];

void LcdBefehl(char Byte)
{
    cbi(PORTA,A0);

    cbi(PORTA,CS);

    _delay_us(10);

    spi_master_send_char(Byte);

    sbi(PORTA,CS);

    _delay_us(10);
}


void LcdData(char Byte)
{
    sbi (PORTB,A0);			// A0 auf HIGH

	cbi (PORTB,CS);		// CS auf LOW

	_delay_us(10);

	spi_master_send_char(Byte);

	sbi (PORTB,CS);		// CS auf HIGH

	_delay_us(10);
}

void LcdInit()
{
    cbi (PORTB,RESET);	// RESET auf LOW
	_delay_ms(100);
	sbi (PORTB,RESET);	// RESET  auf HIGH
	_delay_ms(100);

    // Start Line 0
	LcdBefehl(0x40);

	// ADC reverse
	LcdBefehl(0xA1);

	// Normal COM0-COM63
	LcdBefehl(0xC0);

	// Display normal A6 A7 --> invertiert
	LcdBefehl(0xA6);

	// SET BIAS
	LcdBefehl(0xA2);

	// BOOSTER
	LcdBefehl(0x2F);

	// Set Booster internal to 4x
	LcdBefehl(0xF8);

	//
	LcdBefehl(0x00);

	// CONTRAST
	LcdBefehl(0x27);

	// CONTRAST
	LcdBefehl(0x81);

	// CONTRAST 0x16
	LcdBefehl(0x16);

	// No indicator
	LcdBefehl(0xAC);

	//
	LcdBefehl(0x00);

	// Display on
	LcdBefehl(0xAF);
}

