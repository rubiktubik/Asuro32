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
#include <stdarg.h>
#include <stdint.h>
#include "include/taster.h"
#include "include/Motor.h"
#include "include/Settings.h"
/*
 =======================================================================================================================
 Motor ansteuern
 =======================================================================================================================
 */
/**
  * @brief  Funktion Motor
  *
  *         Diese Funktion aktiviert die Asuromotoren
  *         Eingabe von der Geschwindigkeit ist optional!
  *
  * @param	RichtungLinks   Gibt die Richtung des linken Motors an
  * @param	RichtungRechts  Gibt die Richtung des rechten Motors an
  * @param	SpeedLinks      Gibt die Geschwindigkeit des linken Motors an.(Optionaler Parameter Standart=100)
  * @param	SpeedRechts     Gibt die Geschwindigkeit des rechten Motors an.(Optionaler Parameter Standart=100)
  * @return	            Kein Rückgabewert(void)
  *
  */
void Motor(uint8_t RichtungLinks, uint8_t RichtungRechts, ...)
{
    DDRC |= (1 << DDC7) | (1 << DDC6);
    DDRB |= (1 << DDB0) | (1 << DDB1);
    DDRD |= (1 << DDD4) | (1 << DDD5);


    /*~~~~~~~~~~~~~~~~~~*/
    float	Faktor = 1;
    uint8_t RechtsKorrekt;
    int SpeedLinks=100;
    int SpeedRechts=100;
    /*~~~~~~~~~~~~~~~~~~*/

    va_list argptr; //Anfang

    va_start(argptr, RichtungRechts); //Wert gleich letzter Festparameter
    SpeedLinks = va_arg(argptr,int); //Parameter umwandeln zu z.B. int und rein in eine Variable
    SpeedRechts = va_arg(argptr,int); //Zweites Argument in der Liste
    va_end(argptr); //Ende

    RechtsKorrekt = (int) (((float) SpeedRechts) * Faktor); /* Typumwandlung: */

    /*
     * SpeedRechts(int) wird in float umgewandelt und mal Faktor(float) genommen und ;
     * in RechtsKorrekt eingetragen
     */
    if (RichtungLinks == FWD)
    {
        PORTB |= (1 << PB1);	/* Vorw�hrts */
        PORTB &= ~(1 << PB0);
    }
    else if (RichtungLinks == BWD)
    {
        PORTB &= ~(1 << PB1);	/* R�ckw�rts */
        PORTB |= (1 << PB0);
    }
    else if (RichtungLinks == BREAK)
    {
        PORTB &= ~(1 << PB1);	/* Stop */
        PORTB &= ~(1 << PB0);
        SpeedLinks = 0;
    }

    if (RichtungRechts == FWD)
    {
        PORTC |= (1 << PC7);	/* Vorw�hrts Rechts */
        PORTC &= ~(1 << PC6);
    }
    else if (RichtungRechts == BWD)
    {
        PORTC &= ~(1 << PC7);	/* R�ckw�rts Rechts */
        PORTC |= (1 << PC6);
    }
    else if (RichtungRechts == BREAK)
    {
        PORTC &= ~(1 << PC7);	/* Stop Rechts */
        PORTC &= ~(1 << PC6);
        SpeedRechts = 0;
    }

    /*
     * OCR1A=SpeedLinks;
     */
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);	/* PWM, phase correct, 8 bit.WGM10 --> PWM10 | COM1A1-->
															 * OC1A */
    TCCR1B = (1 << CS11) | (1 << CS10); /* set clock/prescaler 1/64 -> enable counter */

    OCR1B = SpeedRechts;
    OCR1A = SpeedLinks;
}

/*
=======================================================================================================================
	MotorRichtung
=======================================================================================================================
 */
/**
* @brief  Funktion Motorrichtung
*
*         Diese Funktion ändert NUR die Motorrichtung
*         Ist nur für die Motorsound ausgabe nötig!
*
* @param	L   Verhalten vom linken Motor
* @param  R   Verhalten vom rechten Motor
* @return	Gibt nichts zurück
*
*/

void MotorRichtung(uint8_t L,uint8_t R)
{
    DDRC |= (1 << DDC7) | (1 << DDC6);
    DDRB |= (1 << DDB0) | (1 << DDB1);
    DDRD |= (1 << DDD4) | (1 << DDD5);

    if (L == FWD)
    {
        PORTB |= (1 << PB1);	/* Vorw�hrts */
        PORTB &= ~(1 << PB0);
    }
    else if (L == BWD)
    {
        PORTB &= ~(1 << PB1);	/* R�ckw�rts */
        PORTB |= (1 << PB0);
    }
    else if (L == BREAK)
    {
        PORTB &= ~(1 << PB1);	/* Stop */
        PORTB &= ~(1 << PB0);

    }

    if (R == FWD)
    {
        PORTC |= (1 << PC7);	/* Vorw�hrts Rechts */
        PORTC &= ~(1 << PC6);
    }
    else if (R == BWD)
    {
        PORTC &= ~(1 << PC7);	/* R�ckw�rts Rechts */
        PORTC |= (1 << PC6);
    }
    else if (R == BREAK)
    {
        PORTC &= ~(1 << PC7);	/* Stop Rechts */
        PORTC &= ~(1 << PC6);
    }
}
/*
=======================================================================================================================
	- MotorSpeed
=======================================================================================================================
 */
/**
  * @brief  Diese Funktion stellt die Motorgeschwindigkeit ein
  *
  *         Der PWM Wert wird den OCR1A/B Registern zugewiesen
  *         somit wird bestimmt wie lang ein AN-Impuls beim Motor ist.
  *         Dadurch kann man die Geschwindigkeit der Motoren einstellen.
  *
  * @param	L   Ein Wert für den linken Motor
  * @param	R   Ein Wert für den Rechten Motor
  * @return	            Kein Rückgabewert
  *
  */
void MotorSpeed(uint8_t L,uint8_t R)
{
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);	/* PWM, phase correct, 8 bit.WGM10 --> PWM10 | COM1A1-->
															 * OC1A */
    TCCR1B = (1 << CS11) | (1 << CS10); /* set clock/prescaler 1/64 -> enable counter */

    OCR1B = R;
    OCR1A = L;
}
 /*
 =======================================================================================================================
 Motor per Tastendruck starten
 =======================================================================================================================
 */
/**
  * @brief  Funktion MotorAnAus
  *
  *         Diese Funktion aktiviert den Motor beim Drücken
  *         irgendeines Kollisionstasters
  *
  * @return	            Kein Rückgabewert(void)
  *
  */
void MotorAnAus(void)
{
    uint8_t Ausgabe = 0;
    Ausgabe = Taster();
    while (Ausgabe > 0)
    {
        Ausgabe = Taster();

        if (Ausgabe == 1) //Wenn Taster 1 gedrückt
        {
            Motor(FWD,BREAK,100,0); //MotorRechts an
            _delay_ms(20);
        }
    }
    Motor(BREAK,BREAK,0,0); //Sonst Stop
}
