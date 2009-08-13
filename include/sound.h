#ifndef SOUND_H
#define SOUND_H

#endif /* SOUND_H */
/**
 *  @defgroup Sound
 *  @code #include <sound.h> @endcode
 *
 *  @brief Abspielen von Musik mit Hilfe der Asuro Motoren
 *          ToDo: -Empfangen von Zeichen
 *                -Benutzung der Interrupts
 *
 *
 *
 *
 *
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */

/**@{*/

void Sound (uint16_t freq,uint16_t duration_msec,uint8_t  amplitude);
void Sleep (unsigned char time36kHz);
void StereoSound(uint16_t *note1,uint16_t *note2);
void InitSoundTimer(void);

#define SetzeBit(byte,pin) ((byte) |= (1<<pin))
#define LoescheBit(var,bitno)	((var) &= ~(1 << (bitno))
#define TimerStartWert 34285


// tone table: thanks to IVO

//Töne definieren - tiefere Töne brummen und scheppern
#define A4		440
#define B4		466
#define H4		493
#define C5		523
#define DES5	554
#define D5		587
#define ES5		622
#define E5		659
#define F5		698
#define GES5	740
#define G5		783
#define AS5		830
#define A5		880
#define B5		932
#define H5		987
#define C6		1046
#define DES6	1108
#define D6		1174
#define ES6 	1244
#define E6		1318
#define F6		1396
#define GES6	1479
#define G6		1567
#define AS6		1661
#define A6		1760
#define B6		1864
#define H6		1975
#define C7		2093
#define DES7	2217
#define D7		2349
#define ES7		2489
#define E7		2637
#define F7		2793
#define GES7	2959
#define G7		3135
#define AS7		3322
#define A7		3520
#define H7		3729
#define B7		3951

//#define HALF 300
//#define FULL 600
#define _OFF 0
#define STOP 0

#define EIGHTH 300
#define QUARTER 600
#define HALF 1200
#define FULL 2400


#define FS 31250 // controlled by timer2 8Mhz/256

//static uint16_t duck_melody[]={
//	C5,HALF,
//	D5,HALF,
//	E5,HALF,
//	F5,HALF,
//	G5,FULL,
//	_OFF,HALF,
//	G5,FULL,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	G5,FULL,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	A5,HALF,
//	_OFF,HALF,
//	G5,FULL,
//	_OFF,HALF,
//
//	F5,HALF,
//	_OFF,HALF,
//	F5,HALF,
//	_OFF,HALF,
//	F5,HALF,
//	_OFF,HALF,
//	F5,HALF,
//	_OFF,HALF,
//	E5,FULL,
//	_OFF,HALF,
//	E5,FULL,
//	_OFF,HALF,
//
//	D5,HALF,
//	_OFF,HALF,
//	D5,HALF,
//	_OFF,HALF,
//	D5,HALF,
//	_OFF,HALF,
//	D5,HALF,
//	_OFF,HALF,
//
//	C5,FULL,
//	_OFF,HALF,
//
//	STOP,STOP
//};
//
//uint16_t unknown_melody[]={
//	G5,FULL,
//	G5,FULL,
//	A5,HALF,
//	G5,HALF,
//	C6,HALF,
//	H5,HALF/2,
//	G5,FULL,
//	G5,FULL,
//	A5,HALF,
//	G5,HALF,
//	D6,HALF,
//	C6,HALF/2,
//	G5,FULL,
//	G5,FULL,
//	G6,HALF,
//	E6,HALF,
//	C6,FULL,
//	C6,FULL,
//	H5,HALF,
//	A5,HALF,
//	F6,FULL,
//	F6,FULL,
//	E6,HALF,
//	C6,HALF,
//	D6,HALF,
//	C6,HALF/2,
//
//	STOP,STOP
//};
//
//uint16_t schwebung1[]={
//	C5,1000,
//	C5,1000,
//	C5,1000,
//	C5,2000,
//	STOP,STOP
//};
//uint16_t schwebung2[]={
//	D5,1000,
//	E5,1000,
//	F5,1000,
//	G5,2000,
//	STOP,STOP
//};
//
///*Für Elise*/
//uint16_t hs_lied[]={
//    E6,EIGHTH,
//    ES6,EIGHTH,
//    E6,EIGHTH,
//    ES6,EIGHTH,
//    E6,EIGHTH,
//    H5,EIGHTH,
//    D6,EIGHTH,
//    C6,EIGHTH,
//    A5,EIGHTH,
//    _OFF,EIGHTH,
//    C5,EIGHTH,
//    E5,EIGHTH,
//    A5,EIGHTH,
//    H5,QUARTER,
//    _OFF,EIGHTH,
//    E5,EIGHTH,
//    AS5,EIGHTH,
//    H5,EIGHTH,
//    C6,QUARTER,
//    _OFF,QUARTER,
//    E6,EIGHTH,
//    ES6,EIGHTH,
//    E6,EIGHTH,
//    ES6,EIGHTH,
//    E6,EIGHTH,
//    H5,EIGHTH,
//    D6,EIGHTH,
//    C6,EIGHTH,
//    A5,EIGHTH,
//    _OFF,EIGHTH,
//    C5,EIGHTH,
//    E5,EIGHTH,
//    A5,EIGHTH,
//    H5,QUARTER,
//    _OFF,EIGHTH,
//    E5,EIGHTH,
//    C6,EIGHTH,
//    H5,EIGHTH,
//    A5,HALF,
//
//   STOP,STOP
//};
//
///*Oh when the sains*/
//uint16_t sains[]={
//    F5,QUARTER,
//    A5,QUARTER,
//    B5,QUARTER,
//    C6,FULL,
//    F5,QUARTER,
//    A5,QUARTER,
//    B5,QUARTER,
//    C6,FULL,
//    F5,QUARTER,
//    A5,QUARTER,
//    B5,QUARTER,
//    C6,HALF,
//    A5,HALF,
//    F5,HALF,
//    A5,HALF,
//    G5,FULL,
//    A5,QUARTER,
//    A5,QUARTER,
//    G5,QUARTER,
//    F5,HALF,
//    F5,QUARTER,
//    A5,HALF,
//    C6,HALF,
//    C6,QUARTER,
//    B5,FULL,
//    B5,QUARTER,
//    A5,QUARTER,
//    B5,QUARTER,
//    C6,HALF,
//    A5,HALF,
//    F5,HALF,
//    G5,HALF,
//    F5,FULL,
//
//   STOP,STOP
//};
