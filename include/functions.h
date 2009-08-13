
/**
 *  @defgroup Grundfunktionen des Asuros
 *  @code #include <functions.h> @endcode
 *
 *  @brief Eine Sammlung von Asuro Funktionen
 *
 *  Die Bibliothek bieten unter anderem:
 *  Motorsteuerung,
 *  Linienverfolgung,
 *  Ultraschall-Hindernisserkennung,
 *  Ansteuern der LEDs und
 *  Ausgabe auf der seriellen Schnittstelle
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */

/**@{*/

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


void StatusLED(uint8_t farbe);

void LinienLED(uint8_t Status);
void LinienLED(uint8_t Status);
char VariableZuChar(uint16_t Wert);
void RadencoderLEDS(uint8_t Status);
uint16_t max(uint16_t, uint16_t);
uint16_t min(uint16_t, uint16_t);
void SelbstTest(void);
void InitStatusLED(void);
uint16_t Taster(void);

void LinienFolger(uint8_t UmgebungL,uint8_t UmgebungR);
void InitLinienFolger(uint8_t *Links,uint8_t *Rechts);
void PrintWert(char s[50],uint16_t var);

unsigned char get_key_press( unsigned char key_mask );
#endif /* FUNCTIONS_H */
/*
 *  module global variables
 */
volatile unsigned char count72kHz;
volatile unsigned long timebase;

