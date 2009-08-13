#ifndef UART_H
#define UART_H

#endif /* TIMER1_H */

/**
 *  @defgroup UART
 *  @code #include <uart.h> @endcode
 *
 *  @brief Benutzung des Hardware UARTs und Ausgabe von Text
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

void InitUART(void);
void ZeichenAusgabe(unsigned char data);
void TextAusgabe(char *s);
void PrintWert(char s[50],uint16_t var);
