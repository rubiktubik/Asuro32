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
#include "include/touch.h"
#include "include/adc.h"

void GetTouch(int *X,int *Y)
{
    double Ux,Uy;
        int x,y;
        //x
        DDRA |= (1<<PA1); //PA0: 5V
        DDRA |= (1<<PA0); //PA1: 0V
        PORTA |= (1<<PA0);
        _delay_ms(16);

        //adc(PORT) liefert gemesse Spanung in V
        Uy = ReadChannel(2);
        Uy = ((Uy*5.0)/1024.0);
        y = ((Uy-Uy1) / (Uy2-Uy1)) * WIDTH;

        PORTA &= ~(1<<PA0); //PA0 und
        DDRA &= ~(1<<PA0); //PA1 wiedr
        DDRA &= ~(1<<PA1); //Eingang

        //y
        DDRA |= (1<<PA3); //PA3: 5V
        DDRA |= (1<<PA2); //PA2: 0V
        PORTA |= (1<<PA2);
        _delay_ms(16);

        //adc(PORT) liefert gemesse Spanung in V
        Ux = ReadChannel(0);
        Ux = ((Ux*5.0)/1024.0);
        x = (((Ux-Ux1) / (Ux2-Ux1))*HEIGHT);
        PORTA &= ~(1<<PA2); //PA2 und
        DDRA &= ~(1<<PA2); //PA1 wiedr
        DDRA &= ~(1<<PA3); //Eingang
        _delay_ms(100);

    *X = x;
    *Y = y;
}
