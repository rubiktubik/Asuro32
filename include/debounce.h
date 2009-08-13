#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#define CLK 14745600
#define KEY_DDR         DDRD ////Hier den Richtungs PORT angeben wo die Taster angeschlossen sind z.B. DDRD
#define KEY_PORT        PORTD //Hier den Ausgangs-PORT angeben wo die Taster angeschlossen sind z.B. PORTD
#define KEY_PIN         PIND //Hier den Eingangs-PORT angeben wo die Taster angeschlossen sind z.B. PIND
//wo welche Tasten angeschlossen sind
#define KEY0            3
#define KEY1            1
#define KEY2            2
//Alle Tasten
#define ALL_KEYS        (1<<KEY0)//(1<<KEY0 | 1<<KEY1 | 1<<KEY2)

#define REPEAT_MASK     (1<<KEY0)       // repeat: Hier alle Tasten eintragen(wichtig für die short long und repeat funktion!!)
#define REPEAT_START    50                        // after 500ms
#define REPEAT_NEXT     20                        // every 200ms

uint8_t get_key_press( uint8_t key_mask );
uint8_t get_key_rpt( uint8_t key_mask );
uint8_t get_key_short( uint8_t key_mask );
uint8_t get_key_long( uint8_t key_mask );
void InitEntprellTimer(void);
#endif /* TIMER1_H */
