#ifndef MOTOR_H
#define MOTOR_H




/**
 *  @defgroup Motor
 *  @code #include <include/Motor.h> @endcode
 *
 *  @brief Initialisierung und Benutzung der Asuro Motoren
 *
 *          ToDo: -
 *
 *
 *
 *  @note Getestet mit Atmega32
 *  @author Michael Tissen und Alexander Berg
 */




void Motor(uint8_t RichtungLinks, uint8_t RichtungRechts, ...);
void MotorSpeed(uint8_t L,uint8_t R);
void MotorAnAus(void);
void MotorRichtung(uint8_t L,uint8_t R);

#endif /* MOTOR_H */
