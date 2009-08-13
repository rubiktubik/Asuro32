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

#include "include/taster.h"
#include "include/Settings.h"
#include "include/adc.h"
/*
 =======================================================================================================================
 Abfrage der Kollisionstaster
 =======================================================================================================================
 */
/**
  * @brief  Funktion Taster
  *
  *         Diese Funktion prüft mit Hilfe des A/D-Wandlers welche
  *         Taste der Kollisionstaster gedrückt worden ist.
  *         Und gibt ihn als Rückgabe aus.
  *
  *         Keine Erkennung von mehreren Tastern gleichzeitig!!
  *
  * @return	            Gibt werte mit einer Zahl von 0-6 aus
  *
  */
uint16_t Taster(void)
{
    uint16_t		werte=0;
    uint8_t		i;
    uint16_t	adc;
    /*~~~~~~~~~~~~~~~~~*/

    for (i = 0; i < 4; i++)
    {
        adc = ReadChannel(4);
        werte += adc;
    }
    _delay_ms(1);
    werte = werte / 4;
    switch (werte)
    {
    case 63:
        werte = 1;
        break;
    case 31:
        werte = 2;
        break;
    case 15:
        werte = 3;
        break;
    case 7:
        werte = 4;
        break;
    case 2:
        werte = 5;
        break;
    case 0:
        werte = 6;
        break;
    default :
        werte = 0;
    }
    return werte;
}
