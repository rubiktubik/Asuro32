/*************************************************************************
Title:    Allgemeine nützliche Funktionen
Author:   Michael Tissen und Alexander Berg
File:     $Id: functionen.c,v 1.6.2.1 2007/07/01 11:14:38 peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4.6 or higher
Hardware: Atmega32
License:  GNU General Public License

DESCRIPTION:
    Eine Sammlung von Funktionen für den Asuro.

USAGE:




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

#include "include\std.h"
#include "include\types.h"
/*
 =======================================================================================================================
 Gibt den größten der beiden Eingabewerte a und b aus
 =======================================================================================================================
 */
/**
  * @brief  Funktion max
  *
  *         Diese Funktion gibt den grössten der beiden Parameter aus
  *
  * @param	a   Erster Parameter als unint16
  * @param	b   Zweiter Parameter als unint16
  * @return    Gibt den grössten Wert aus, a oder b
  *
  */
uint16_t max(uint16_t a, uint16_t b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/*
 =======================================================================================================================
Gibt den kleinsten der beiden Eingabewerte a und b aus
 =======================================================================================================================
 */
/**
   * @brief Vergleicht a und b und gibt den kleineren der beiden aus
   * @param a ist der erste Wert zum Verleichen
   * @param b ist der zweite Wert zum Verleichen
   * @return Gibt den kleineren aus a oder b
*/
uint16_t min(uint16_t a, uint16_t b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

