#ifndef LEDS_H
#define LEDS_H

#endif /* LEDS_H */

/**
 *  @defgroup LEDs
 *  @code #include <include/leds.h> @endcode
 *
 *  @brief Initialisierung und Benutzung der Asuro LEDs
 *
 *          ToDo: -
 *
 *
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */

/**@{*/

void StatusLED(uint8_t farbe);

void LinienLED(uint8_t Status);

void RadencoderLEDS(uint8_t Status);

void InitStatusLED(void);
