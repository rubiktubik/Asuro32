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
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/uart.h"
#include "include\Settings.h"
/*
 =======================================================================================================================
    Seriellen Port initiallisieren
 =======================================================================================================================
 */
/**
  * @brief  Funktion InitUART
  *
  *         Diese Funktion initialisiert die nötigen Register
  *         für die Nutzung der seriellen Schnittstelle.
  *
  * @return	            Kein Rückgabewert(void)
  *
  */
void InitUART(void)
{
    cli();
    UBRRH = UBRR_VAL >> 8;
    UBRRL = UBRR_VAL & 0xFF;

    UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);	/* Asynchron 8N1 */

    UCSRB |= (1 << TXEN);	/* UART TX einschalten */
}

/*
 =======================================================================================================================
    Zeichen auf Seriellem Port ausgeben
 =======================================================================================================================
 */
/**
  * @brief  Funktion ZeichenAusgabe
  *
  *         Diese Funktion gibt EIN Zeichen
  *         auf der seriellen Schnittstelle aus.
  *
  * @param	data   Ein Zeichen --> Beispiel: 'a'
  * @return	            Kein Rückgabewert(void)
  *
  */
void ZeichenAusgabe(unsigned char data)
{

    /* Wait for empty transmit buffer */
    while (!(UCSRA & (1 << UDRE)))	/* warten bis Senden moeglich */
        { }

    UDR = data;
}

/*
 =======================================================================================================================
    Puts gibt String auf dem Seriellen Port aus
 =======================================================================================================================
 */
/**
  * @brief  Funktion TextAusgabe
  *
  *         Diese Funktion gibt Text
  *         auf der seriellen Schnittstelle aus.
  *
  * @param	*s   Zeiger auf s --> Beispiel: "Hallo"
  * @return	            Kein Rückgabewert(void)
  *
  */
void TextAusgabe(char *s)
{
    while (*s)
    {
        /* so lange *s != '\0' also ungleich dem "String-Endezeichen" */
        ZeichenAusgabe(*s);
        s++;
    }
}

/*
=======================================================================================================================
	Speicherintensive aber bequeme TextAusgabe
=======================================================================================================================
 */
/**
  * @brief  Funktion PrintWert
  *
  *         Diese Funktion gibt einen Formatierten Text(printf Format) mit einer als
  *         Parameter übergebenen Variable aus
  *         auf dem seriellen Port aus.
  *         Speicherintensiv!! Zum schnellen Ausgeben von variablen mit formatierten Text
  *
  * @param	s   String im Printf Format
  * @param	var   Auszugebende Variable als unsigned integer
  * @return	            Kein Rückgabewert(void)
  *
  */
void PrintWert(char s[50],uint16_t var)
{
    char Buffer[50];
    sprintf(Buffer,s,var);
    TextAusgabe(Buffer);
}

