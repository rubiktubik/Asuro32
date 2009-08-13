#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#endif /* ULTRASONIC_H */

/**
 *  @defgroup Ultraschall
 *  @code #include <ultrasonic.h> @endcode
 *
 *  @brief Initialisierung und Benutzung des Ultraschallsensor
 *          inklusive Abstandsmessung
 *          ToDo: Nochmal auf Funktion überprüfen
 *
 *
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */

/**@{*/

//void UltraschallInit(void);
void Ping(unsigned char length);
void InitUltraTimer(void);
void Ausweichen(uint16_t Speed);
void InitUltrasonics(void);
