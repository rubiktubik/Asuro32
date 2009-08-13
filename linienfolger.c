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
#include <stdlib.h>
#include <stdio.h>


#include "include/Settings.h"
#include "include/adc.h"
#include "include/std.h"
#include "include/Motor.h"
#include "include/linienfolger.h"
#include "include/uart.h"

/*
 =======================================================================================================================
        LinienFolger
 =======================================================================================================================
 */
/**
  * @brief  Funktion LinienFolger
  *
  *         Der Asuro folgt einer Linie!
  *         Lichtdifferenz kann man in Settings.h --> Differenz ändern
  *
  *         Vor Benutzung muss InitLinieFolger() und stehen außerdem muss die Linien LED durch
  *         LinienLED(AN) aktiviert sein! Getestet mit schwarzem Klebeband als Linie
  *
  * @param	UmgebungL   Vorher gemessene Daten vom linken Fototransistor als Umgebungslicht
  * @param	UmgebungR   Vorher gemessene Daten vom rechten Fototransistor als Umgebungslicht
  * @return	            Kein Rückgabewert(void)
  *
  */
void LinienFolger(uint8_t UmgebungL,uint8_t UmgebungR)
{
    /*~~~~~~~~~~~~~~~~~*/
    uint8_t WerteRechts=0,WerteLinks=0;
    uint16_t DiffRechts=0,DiffLinks=0;
    /*~~~~~~~~~~~~~~~~~*/

    WerteLinks = ReadChannel(LDRLinks); //Messe Linken Wert
    _delay_ms(1);
    WerteRechts = ReadChannel(LDRRechts); //Messe Rechten Wert
    _delay_ms(1);

    DiffRechts = max(UmgebungR,WerteRechts) - min(UmgebungR,WerteRechts); //Differenz zwischen Umgebung und gemessener Wert Rechts
    DiffLinks = max(UmgebungL,WerteLinks) - min(UmgebungL,WerteLinks); //Differenz zwischen Umgebung und gemessener Wert Links
//    PrintWert("DiffLinks %u",DiffLinks);
    //if (DiffLinks > Differenz && !(DiffRechts > Differenz))
    if (DiffLinks > Differenz) //Wenn die Differenz zwischen Umgebung und Sensor(Links) größer
    {
//        TextAusgabe("Differenz groesser");
        while (DiffLinks > Differenz )
        {
            WerteLinks = ReadChannel(LDRLinks);
            DiffLinks = max(UmgebungL,WerteLinks) - min(UmgebungL,WerteLinks);
            Motor(FWD,BREAK,100,0);
            _delay_ms(10);
        }
        Motor(BREAK,BREAK,0,0);
    }
    //else if (DiffRechts > Differenz && !(DiffLinks > Differenz))
    else if (DiffRechts > Differenz)
    {
        while (DiffRechts > Differenz)
        {
            WerteRechts = ReadChannel(LDRRechts);
            DiffRechts = max(UmgebungR,WerteRechts) - min(UmgebungR,WerteRechts);
            Motor(BREAK,FWD,0,100);

            _delay_ms(10);
        }
        Motor(BREAK,BREAK,0,0);

    }
    else
    {
        Motor(FWD,FWD,80,80);

        _delay_ms(10);
    }
}
void InitLinienFolger(uint8_t *Links,uint8_t *Rechts)
{
    *Links = ReadChannel(LDRLinks);
    *Rechts = ReadChannel(LDRRechts);
}
