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
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "include/Settings.h"
#include "include/adc.h"
/*
 =======================================================================================================================
    Adc Wert lesen;
    mux = ADC Kanal
 =======================================================================================================================
 */
/**
  * @brief  Funktion ReadChannel
  *
  *         Diese Funktion liest den A/D Wert vom jeweiligen Pin.
  *
  * @param	mux   AD Portnummer
  * @return	  gibt result als den A/D-Wert aus
  *
  */
uint16_t ReadChannel(uint8_t mux)
{
    /*~~~~~~~~~~~~~~~*/
    uint8_t		i;
    uint16_t	result;
    /*~~~~~~~~~~~~~~~*/

    ADMUX = mux;			/* Kanal waehlen */
    ADMUX |= (1 << REFS0);	/* AVCC Referenzspannung nutzen */

    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0); /* Frequenzvorteiler */

    /*
     * setzen auf 8 (1) und ADC aktivieren (1) ;
     * nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest also
     * einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen"
     */
    ADCSRA |= (1 << ADSC);	/* eine ADC-Wandlung */
    while (ADCSRA & (1 << ADSC))
    {
        ;			/* auf Abschluss der Konvertierung warten */
    }

    result = ADCW;	/* ADCW muss einmal gelesen werden, */

    /*
     * sonst wird Ergebnis der n�chsten Wandlung ;
     * nicht �bernommen. ;
     * Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen
     */
    result = 0;
    for (i = 0; i < 4; i++)
    {
        ADCSRA |= (1 << ADSC);	/* eine Wandlung "single conversion" */
        while (ADCSRA & (1 << ADSC))
        {
            ;			/* auf Abschluss der Konvertierung warten */
        }

        result += ADCW; /* Wandlungsergebnisse aufaddieren */
    }

    ADCSRA &= ~(1 << ADEN); /* ADC deaktivieren (2) */

    result /= 4;			/* Summe durch vier teilen = arithm. Mittelwert */

    return result;
}
