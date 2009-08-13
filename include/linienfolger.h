#ifndef LINIENFOLGER_H
#define LINIENFOLGER_H

#endif /* LINIENFOLGER_H */

/**
 *  @defgroup Linienfolger
 *  @code #include <include/linienfolger.h> @endcode
 *
 *  @brief Initialisierung und Benutzung der Fototransistoren
 *          inklusive der Linienfolgefunktion
 *          ToDo:
 *
 *
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */

/**@{*/
void LinienFolger(uint8_t UmgebungL,uint8_t UmgebungR);
void InitLinienFolger(uint8_t *Links,uint8_t *Rechts);
