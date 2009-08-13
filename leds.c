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
#include "include/leds.h"
#include "include/Settings.h"
/*
 =======================================================================================================================
 Macht die Linien LED an
 =======================================================================================================================
 */
/**
  * @brief  Funktion LinienLED
  *
  *         Linien Led ein oder ausschalten
  *
  * @param	Status   Zustand der LEDs --> AN oder AUS
  * @return	            Kein Rückgabewert(void)
  *
  */
void LinienLED(uint8_t Status)
{
    DDRD |= (1 << DDD6);
    if (Status == AN)
    {
        PORTD |= (1 << PD6);
    }
    else if (Status == AUS)
    {
        PORTD &= ~(1 << PD6);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
/**
  * @brief  Funktion InitStatusLED
  *
  *         Diese Funktion initialisiert die PORTS und Register für
  *         die Benutzung der Status LED.
  *
  * @return	            Kein Rückgabewert(void)
  *
  */
void InitStatusLED(void)
{
    DDRD |= (1<<DDD7); //IR-PORT als Ausgang
    PORTD |= (1<<PB7); //IR-PORT auf 1 um Strom zu Sparen
    RichtungRot |= (1 << RotPin);
    RichtungGruen |= (1 << GruenPin);
}

/*
 =======================================================================================================================
Macht die IR-LEDs am Radencoder an
 =======================================================================================================================
 */
/**
  * @brief  Funktion RadencoderLEDs
  *
  *         Diese Funktion Aktiviert oder Deaktiviert die RadencoderLEDs.
  *
  * @param	Status   Zustand der LEDs --> AN oder AUS
  * @return	            Kein Rückgabewert(void)
  *
  */
void RadencoderLEDS(uint8_t Status)
{
    DDRA |= (1 << DDA7);
    if (Status == AN)
    {
        PORTA |= (1 << PA7);
    }
    else if (Status == AUS)
    {
        PORTA &= ~(1 << PA7);
    }
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
/**
  * @brief  Funktion StatusLED
  *
  *         Diese Funktion macht die StatusLED Grün, Rot oder Aus.
  *
  * @param	farbe   Farbe und Zustand der Led --> Rot,Gruen oder AUS
  * @return	            Kein Rückgabewert(void)
  *
  */
void StatusLED(uint8_t farbe)
{
    if (farbe == Rot)
    {
        GruenPort &= ~(1 << GruenPin);
        RotPort |= (1 << RotPin);
    }
    else if (farbe == Gruen)
    {
        RotPort &= ~(1 << RotPin);
        GruenPort |= (1 << GruenPin);
    }
    else if (farbe == AUS)
    {
        RotPort &= ~(1 << RotPin);
        GruenPort &= ~(1 << GruenPin);
    }
}
