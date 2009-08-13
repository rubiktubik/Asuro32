/*************************************************************************
Title:    Asuro Bibliothek
Author:   Michael Tissen und Alexander Berg
File:     $Id: functionen.c,v 1.6.2.1 2007/07/01 11:14:38 peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4.6 or higher
Hardware: Atmega32
License:  GNU General Public License

DESCRIPTION:
    Eine Sammlung von Funktionen für den Asuro.

USAGE:
    Zur Nutzung funktionen.h und Settings.h einbinden



LICENSE:
    Copyright (C) --

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/


#include <avr/io.h>

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "include/ultrasonic.h"
#include "include/Settings.h"
#include "include/leds.h"
#include "include/Motor.h"
#include "include/uart.h"

volatile uint8_t count40kHz;

ISR(TIMER2_COMP_vect)
{
	count40kHz++;
}

/*
=======================================================================================================================
	-
 =======================================================================================================================
 */
/**
  * @brief  Funktion InitUltraTimer
  *
  *         Diese Funktion initialisiert den Ultraschall
  *
  * @return	            Kein Rückgabewert(void)
  *
  */
void InitUltraTimer(void)
{
//    TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS20);
//    OCR2  = 0x91;                         // duty cycle fuer 36kHz
//    TIMSK |= (1 << OCIE2);
//    DDRD |= (1 << DDD7);	/* PORTD.7 als Ausgang fuer OC2 Funktion */
        DDRD = 0x04;
        DDRB = 0x08;
        PORTB = 0x08;
        PINB = 0x08;
        TCCR2 = 0x19;
        OCR2 = 0x6E;
        TIMSK = 0x80;
        DDRD = 0x84;
        PIND = 0x80;
        TCCR2 |= (1<<CS20);

}

/*
 =======================================================================================================================
 Asuro fährt und sendet einen Ultraschall bei einem Hinterniss weicht er durch eine Linksdrehung aus
 =======================================================================================================================
 */
/**
  * @brief  Funktion Ausweichen
  *
  *         Der Asuro benutzt den Ultraschall um die Umgebung zu scannen und weicht
  *         jedem Hinderniss mit einer Rechtsdrehung aus und fährt weiter geradeaus.
  *
  * @param	Speed   Die Fahrgeschwindigkeit
  * @return	            Kein Rückgabewert(void)
  *
  */
void Ausweichen(uint16_t Speed)
{
    sei();
    /*~~~~~~~~~~*/
    int pos, i;
    int posmarker;
    /*~~~~~~~~~~*/
    TextAusgabe("Vor Ping");
    TextAusgabe(CR);
    posmarker = 0;
    Ping(20);
    TextAusgabe("Nach PING");
    TextAusgabe(CR);
    for (pos = 0; pos < 1000; pos++)
    {
        TextAusgabe("In der ersten For-schleife");
        TextAusgabe(CR);
        /* 1000 = die maximale Entfernung */
        _delay_us(10);
        if ((ACSR & (1 << ACI)) != 0)
        {
            TextAusgabe("Im ersten if");
            TextAusgabe(CR);
            /* Wenn Register ACI von ACSR nicht gesetzt ist dann.. */
            if (posmarker == 0)
            {
                TextAusgabe("Im zweiten if");
                TextAusgabe(CR);
                posmarker = pos;
            }	/* Wenn posmarker = 0 ist dann pos und posmarker gleichsetzen */
        }
        TextAusgabe("Vor ACSR");
        ACSR |= (1 << ACI); /* Das Bit ACI setzen. Damit es durch einen Interrupt gel�scht werden kann */
    }

    if (posmarker < 200)
    {
        TextAusgabe("Im dritten if");
        TextAusgabe(CR);
        /* Wenn ein Hinderniss im Bereich von 200 ist */
        StatusLED(Rot); /* StatusLed leuchtet Rot */
        Motor(BREAK, BREAK, 0, 0);		/* Motor kurz anhalten */
        Motor(FWD, BWD, 0, 255);		/* Linkes Rad vorw�rts und Rechtes R�ckw�rts(Ausweichen) */
    }
    else
    {
        TextAusgabe("Im else");
        TextAusgabe(CR);
        StatusLED(Gruen);				/* LED Gr�n */
        Motor(FWD, FWD, Speed, Speed);	/* Motoren f�hrt geradeaus */

        for (i = 0; i < 100; i++)
        {
            TextAusgabe("Im zweiten for");
            TextAusgabe(CR);
            _delay_ms(1);
        }	/* kurze Pause */
    }
}
/*
 =======================================================================================================================
 Sendet einen Ultraschallimpuls(nötig für Funktion Ausweichen!)
 =======================================================================================================================
 */
/**
  * @brief  Funktion Ping
  *
  *         Diese Funktion sendet einen Ping aus dem Ultraschall
  *         nötig für die Funktion Ausweichen
  *
  * @param	lenght   Die Länge des Impulses
  * @return	            Kein Rückgabewert(void)
  *
  */
void Ping(unsigned char length)
{
    count40kHz = 0;
    TextAusgabe("Vor TCCR2");
    TextAusgabe(CR);
    TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS20);

    /*
     * Toggling of IO-Pin on ;
     * generate the Chirp
     */
    TextAusgabe("Vor while");
    TextAusgabe(CR);
    while (count40kHz < length)
    {
        TextAusgabe("In der Ping while");
        TextAusgabe(CR);
        PrintWert("counter %u",count40kHz);
        TextAusgabe(CR);
        OCR2 = 0x64 + length / 2 - count40kHz;
    }
    TextAusgabe("Aus der Ping-while");
    TextAusgabe(CR);
    TCCR2 = (1 << WGM21) | (1 << CS20); /* Toggling of IO-Pin off */
    OCR2 = 0x64;
    ACSR |= _BV(ACI);
    TextAusgabe("Ende von Ping");
    TextAusgabe(CR);
    /* set frequency to 40kHz */
}

/*
 =======================================================================================================================
    Nach InitTimer starten
 =======================================================================================================================
 */
/**
  * @brief  Funktion UltraschallInit
  *
  *         Diese Funktion stellt die Register für die Benutzung des
  *         Ultraschalls ein.
  *         Aufruf muss nach InitTimer erfolgen!
  *
  * @return	            Kein Rückgabewert(void)
  *
  */
//void UltraschallInit(void)
//{
//    /*
//     * Change Oscillator-frequency of Timer 2 ;
//     * to 40kHz, no toggling of IO-pin:
//     */
//    cli();
//    TCCR2 |= (1 << WGM21) | (1 << CS20);
//    OCR2 = 0x64;			/* 40kHz @8MHz crystal */
//    ADCSRA = 0x00;			/* ADC off */
//
//    /* Analog comparator: */
//    ACSR = 0x02;			/* Generate interrupt on falling edge */
//    ADMUX = 0x06;			/* Multiplexer for comparator to */
//
//    /* ADC pin 6 */
//    SFIOR |= (1 << ACME);	/* Enable muliplexing of comparator */
//    DDRB &= ~(1 << DDB2);	/* Port B Pin 2 is input! */
//    sei();
//}

void InitUltrasonics(void)
{
	// Change Oscillator-frequency of Timer 2
	// to 40kHz, no toggling of IO-pin:
  //cli();
  TCCR2  = (1 << WGM21) | (1 << CS20);
	OCR2   = 0x64;              // 40kHz @8MHz crystal
	TIMSK  = (1 << OCIE2);     // OCIE2:  Timer/Counter2 Output Compare Match Interrupt Enable

	ADCSRA = 0x00;      // deactivate ADC
	ACSR  = 0x02;    // Comparator Interrupt on Falling Output Edge

	ADMUX  = 0x06;             // connect ADC3-input with comparator
	SFIOR |= (1 << ACME);      // connect ADC multiplexer to comparator
	//DDRB  &= ~(1 << DDB2);        // use Port D Pin 6 as input (AIN0)
  //sei();
}
