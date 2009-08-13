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

#include "include/Motor.h"
#include "include/sound.h"
#include "include/Settings.h"

/*
=======================================================================================================================
	-
 =======================================================================================================================
 */
/**
  * @brief  Funktion Sound
  *
  *         Diese Funktion gibt Sound auf EINEM Asuro Motor aus.
  *
  *
  * @param	freq   Frequenz
  * @param	duration Dauer
  * @param	amplitude Amplitude
  * @return	            Kein Rückgabewert(void)
  *
  */
void Sound (uint16_t freq,uint16_t duration_msec,uint8_t  amplitude)
{
    uint16_t wait_tics;
    uint32_t n,k,period_usec,dauer_usec;

    period_usec = 1000000L / freq;
    dauer_usec = 1000 * (uint32_t) duration_msec;
    k = dauer_usec / period_usec;

    //IR Interuptfreq=36KHz
    //Wavefreq=18KHz

    wait_tics = 18000 / freq;

    Motor(BREAK,BREAK,amplitude,amplitude);

    for (n = 0; n < k; n++)
    {
        Motor (FWD, FWD,amplitude,amplitude);
        Sleep (wait_tics);
        Motor (BWD, BWD),amplitude,amplitude;
        Sleep (wait_tics);
    }
    Motor(BREAK,BREAK,0,0);
}
/*
=======================================================================================================================
	Sleep
 =======================================================================================================================
 */
/**
  * @brief  Funktion Sleep
  *
  *         Erzeugt eine Wartezeit mit Hilfe vom Timer
  *
  * @param	time36kHz   Zählerwert vom Timerinterrupt
  * @return	            Kein Rückgabewert
  *
  */
void Sleep (unsigned char time36kHz)
{
    unsigned char ziel = (time36kHz + count72kHz) & 0x00FF;

    while (count72kHz != ziel)
        ;
}
/*
=======================================================================================================================
	Stereo Sound
 =======================================================================================================================
 */
/**
  * @brief  Funktion StereoSound
  *
  *         Diese Funktion kann zwei verschiedene Töne an den Motoren ausgeben
  *         je nachdem welche Noten rein kommen.
  *
  * @param	*noten1   Note für den linken Motor
  * @param	*noten2   Note für den rechten Motor
  * @return	            Kein Rückgabewert
  *
  */
void StereoSound(uint16_t *noten1,uint16_t *noten2)
{
    uint16_t index1=0,index2=0;
    uint16_t timer1,timer2;
    uint16_t phase1=0,phase2=0,angle1,angle2;
    uint8_t dir1=FWD,dir2=FWD;
    uint8_t speed1,speed2;

    angle1=(uint32_t)noten1[index1++]*65536/FS;
    timer1=noten1[index1++]*(FS/1000);
    if (angle1==0)speed1=0;
    else speed1=255;

    angle2=(uint32_t)noten2[index2++]*65536/FS;
    timer2=noten2[index2++]*(FS/1000);
    if (angle2==0)speed2=0;
    else speed2=255;
    MotorSpeed(speed1,speed2);

    cli(); // stop all interrupts
    while (timer1!=0)
    {
        timer1--;
        timer2--;

        if (timer1==0)
        {
            angle1=(uint32_t)noten1[index1++]*65536/FS;
            timer1=noten1[index1++]*(FS/1000);
            if (angle1==0)speed1=0;
            else speed1=255;

            MotorSpeed(speed1,speed2);
        }

        if (timer2==0)
        {
            angle2=(uint32_t)noten2[index2++]*65536/FS;
            timer2=noten2[index2++]*(FS/1000);
            if (angle2==0)speed2=0;
            else speed2=255;

            MotorSpeed(speed1,speed2);
        }
        phase1+=angle1;

        if (phase1&0x8000) dir1=FWD;
        else dir1=BWD;

        phase2+=angle2;

        if (phase2&0x8000) dir2=FWD;
        else dir2=BWD;
        MotorRichtung(dir1,dir2);

        // sync with Timer2 ( 31250Hz )
        while (!(TIFR&(1<<TOV2)));
        TIFR|=TIFR&(1<<TOV2);
    }
    MotorSpeed(0,0);
    sei(); // enable all interrupts
}

/*
 =======================================================================================================================
    Muss vor Nutzung des Sounds initialisiert werden!
 =======================================================================================================================
 */
/**
  * @brief  Funktion InitSoundTimer
  *
  *         Diese Funktion initialisiert dir Register um
  *         die Stereosound Funktion nutzen zu können
  *
  * @return	            Kein Rückgabewert(void)
  *
  */
void InitSoundTimer(void)
{
    TCCR2 |= (1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21) | (1 << CS20);
    OCR2  = 0x91; // duty cycle for 36kHz
    TIMSK |= (1 << TOIE2); // 36kHz counter for sleep
}
