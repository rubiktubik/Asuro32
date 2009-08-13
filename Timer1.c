#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/Timer1.h"
#include "include/leds.h"
#include "include/Settings.h"
//Initialisiert Counter und aktiviert Interrupt bei Overflow
void InitCounter(uint16_t StartWert)
{
    SETZE_BIT(TCCR1B,WGM12); //ClearTimerOnCompare Modus --> Festlegung des Neustarts des Timers


    SETZE_BIT(TCCR1B,CS12); //Prescaler auf 256 --> CS10,CS11,CS12 auf 0 gleich Timer Stop

    SETZE_BIT(TIMSK,OCIE1A); // Aktiviere CompareMatch A Interrupt --> --> Ist der Interrupt der für das CTC zuständig/nötig ist

    //SetzeBit(TIMSK,TOIE1); //Interrupt beim Timer1 Überlauf das heisst von 0-65536 --> keine anpassung möglich

    OCR1A = StartWert; //Den Timer StartWert festlegen --> also nicht von 0-65536 sondern z.b. von 3000-65536 30000 --> ungefähr 1sec

    sei(); //Globale Interrupts aktivieren
}

void StopCounter(void)
{
    TCCR1B |= ~((1<<CS10) || (1<<CS11) || (1<<CS12));
}
//ISR(TIMER1_OVF_vect)
//{
//    counter++; //Interrupt für normalen Overflow
//}

ISR(TIMER1_COMPA_vect)
{
    PORTD ^= (1<<PD6);
    if (counter == 0)
    {
        counter = 1;
    }
    else
    {
        counter = 0;
    }
}




