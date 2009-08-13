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
#include "include/Settings.h"
#include "include/functions.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/sleep.h>

#include "Timer1.h"
#include "help.h"
/*
 =======================================================================================================================
 =======================================================================================================================
 */
//
//SIGNAL(SIG_OUTPUT_COMPARE2) /* Interrupt Compare OC2 */
//{
//    count72kHz++;
//}
/*
 =======================================================================================================================
 =======================================================================================================================
 */
ISR(TIMER2_COMP_vect)
{
    count72kHz++;
}
/*
 =======================================================================================================================
 =======================================================================================================================
 */
ISR(TIMER2_OVF_vect)
{
    TCNT2 += 0x25;
    count72kHz ++;
    if (!count72kHz) timebase ++;
}
/*
 =======================================================================================================================
 =======================================================================================================================
 */


/*
 =======================================================================================================================
 =======================================================================================================================
 */
ISR(INT0_vect)
{
    sleep_disable();
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
inline void Motor(uint8_t RichtungLinks, uint8_t RichtungRechts, ...)
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
void UltraschallInit(void)
{
    /*
     * Change Oscillator-frequency of Timer 2 ;
     * to 40kHz, no toggling of IO-pin:
     */
    cli();
    TCCR2 |= (1 << WGM21) | (1 << CS20);
    OCR2 = 0x64;			/* 40kHz @8MHz crystal */
    ADCSRA = 0x00;			/* ADC off */

    /* Analog comparator: */
    ACSR = 0x02;			/* Generate interrupt on falling edge */
    ADMUX = 0x06;			/* Multiplexer for comparator to */

    /* ADC pin 6 */
    SFIOR |= (1 << ACME);	/* Enable muliplexing of comparator */
    DDRB &= ~(1 << DDB2);	/* Port B Pin 2 is input! */
    sei();
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
    count72kHz = 0;

    TCCR2 = (1 << WGM21) | (1 << COM20) | (1 << CS20);

    /*
     * Toggling of IO-Pin on ;
     * generate the Chirp
     */
    while (count72kHz < length)
    {
        OCR2 = 0x64 + length / 2 - count72kHz;
    }

    TCCR2 = (1 << WGM21) | (1 << CS20); /* Toggling of IO-Pin off */
    OCR2 = 0x64;
    ACSR |= _BV(ACI);

    /* set frequency to 40kHz */
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
    TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM20) | (1 << COM21) | (1 << CS20);
    OCR2  = 0x91;                         // duty cycle fuer 36kHz
    TIMSK |= (1 << TOIE2);
    DDRD |= (1 << DDD7);	/* PORTD.7 als Eingang f�r OC2 Funktion */
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
    /*~~~~~~~~~~*/
    int pos, i;
    int posmarker;
    /*~~~~~~~~~~*/

    posmarker = 0;
    Ping(20);
    for (pos = 0; pos < 1000; pos++)
    {
        /* 1000 = die maximale Entfernung */
        _delay_us(10);
        if ((ACSR & (1 << ACI)) != 0)
        {
            /* Wenn Register ACI von ACSR nicht gesetzt ist dann.. */
            if (posmarker == 0)
            {
                posmarker = pos;
            }	/* Wenn posmarker = 0 ist dann pos und posmarker gleichsetzen */
        }

        ACSR |= (1 << ACI); /* Das Bit ACI setzen. Damit es durch einen Interrupt gel�scht werden kann */
    }

    if (posmarker < 200)
    {
        /* Wenn ein Hinderniss im Bereich von 200 ist */
        StatusLED(Rot); /* StatusLed leuchtet Rot */
        Motor(BREAK, BREAK, 0, 0);		/* Motor kurz anhalten */
        Motor(FWD, BWD, 0, 255);		/* Linkes Rad vorw�rts und Rechtes R�ckw�rts(Ausweichen) */
    }
    else
    {
        StatusLED(Gruen);				/* LED Gr�n */
        Motor(FWD, FWD, Speed, Speed);	/* Motoren f�hrt geradeaus */

        for (i = 0; i < 100; i++)
        {
            _delay_ms(1);
        }	/* kurze Pause */
    }
}

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

/*
 =======================================================================================================================
 Kurzer Selbstest der die Motoren und die Status-LED testet
 =======================================================================================================================
 */
/**
  * @brief  Funktion SelbstTest
  *
  *         Diese Funktion überprüft die Hardware des Asuros:
  *         1. beide Motoren geradeaus
  *         2. Status Led leuchtet Gruen
  *         3. beide Motoren stoppen
  *         4. beide Motoren rückwerts
  *         5. Status Led leuchtet Rot
  *         6. beide Motoren stoppen
  *
  *
  */
void SelbstTest(void)
{
    Motor(FWD, FWD, 100, 100);
    StatusLED(Gruen);
    _delay_ms(600);
    Motor(BREAK, BREAK, 0, 0);
    _delay_ms(600);
    Motor(BWD, BWD, 100, 100);
    StatusLED(Rot);
    _delay_ms(600);
    Motor(BREAK, BREAK, 0, 0);
}

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
/*

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
  *         LinienLED(AN) aktiviert sein!
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

    //if (DiffLinks > Differenz && !(DiffRechts > Differenz))
    if (DiffLinks > Differenz) //Wenn die Differenz zwischen Umgebung und Sensor(Links) größer
    {
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
