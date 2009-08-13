#ifndef ADC_H
#define ADC_H

#endif /* ADC_H */

/**
 *  @defgroup A/D Wandler
 *  @code #include <include/leds.h> @endcode
 *
 *  @brief Initialisierung und Benutzung der Analog/Digitalwandler des AVRs
 *
 *          ToDo: -
 *
 *
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */

/**@{*/

uint16_t ReadChannel (uint8_t mux);
