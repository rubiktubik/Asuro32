#ifndef Settings_H
#define Settings_H


/*Globale Variablen */
uint16_t count72kHz;

/*
 *  constants and macros
 */
#define FR_CPU 14745600UL //Mein Board
//#define FR_CPU 8000000UL //Asuro

#define BAUD 2400UL          // Baudrate
#define UBRR_VAL ((FR_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (FR_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif
//Controllerspezifische Einstellungen

//Status Leds:
#define RichtungRot DDRC
#define RichtungGruen DDRB
#define RotPort PORTD //PORT an dem die Rote LED ist
#define GruenPort PORTB
#define RotPin PC1
#define GruenPin PB3

#define AUSGANG 0xFF

//Hilfsdefinitionen
#define Rot 1
#define Gruen 2
#define AUS 3
#define FWD 1
#define BWD 2
#define BREAK 3
#define AN 1
#define LDRLinks 3
#define LDRRechts 2
#define Differenz 40UL /*Differenz für die Linienfolger;
    15:00 --> 90
    10:00 aufgemalte Linie --> 40
*/
#define EINGABEPIN PIND //Eingabepin für Debounce

#define ToString utoa
#define CR "\r\n"
#define NEWLINE TextAusgabe(CR);

#define SETZE_BIT(reg,bit)  (reg |= (1<<bit))

#define Uy1 0.977
#define Ux1 0.833
#define Uy2 4.063
#define Ux2 4.048

#define HEIGHT 38
#define WIDTH 66

#define FALSE	0
#define TRUE	-1

//Defines für SPI
#ifndef outb
	#define	outb(addr, data)	addr = (data)
#endif
#ifndef inb
	#define	inb(addr)			(addr)
#endif
#ifndef outw
	#define	outw(addr, data)	addr = (data)
#endif
#ifndef inw
	#define	inw(addr)			(addr)
#endif
#ifndef BV
	#define BV(bit)			(1<<(bit))
#endif
#ifndef cbi
	#define cbi(reg,bit)	reg &= ~(BV(bit))
#endif

#define sbi(reg,bit)	reg |= (BV(bit))

#endif
