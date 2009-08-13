#ifndef TIMER1_H
#define TIMER1_H

#endif /* TIMER1_H */

/**
 *  @defgroup Timer
 *  @code #include <uart.h> @endcode
 *
 *  @brief Benutzung des Hardware Timer z.z Timer1
 *          ToDo: -alle Timer einfügen
 *
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

extern void InitCounter(uint16_t);
void StopCounter(void);

volatile uint16_t counter;
