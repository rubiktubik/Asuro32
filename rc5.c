#include <avr/io.h>
#include <avr/interrupt.h>

// Für alte avr-gcc Versionen
#ifndef SIGNAL
#include <avr/signal.h>
#endif // SIGNAL

#include "include/rc5.h"

#ifndef RC5_INT
#define RC5_INT      RC5_INT0
#endif  /* RC5_INT */

#ifndef RC5_PRESCALE
#define RC5_PRESCALE 256
#endif  /* RC5_PRESCALE */
#include "include\uart.h"

/* ******************************************************************************** */

rc5_t rc5;

/* ******************************************************************************** */

#ifndef F_CPU
#error Please define F_CPU
#endif /* !F_CPU */

/* µs for a whole bit of RC5 (first & second part) */
#define RC5_BIT_US   (64*27)

#define RC5_TICKS \
        ((uint8_t) ((uint32_t) (F_CPU / 1000 * RC5_BIT_US / 1000 / RC5_PRESCALE)))

#define RC5_DELTA \
        (RC5_TICKS / 6)

typedef union
{
        uint16_t w;
        uint8_t  b[2];
} code_t;

static code_t code;
static uint8_t rc5_addr;

/* Number of Bits received so far */
/* Number of Interrupts occured so far */
static uint8_t nbits;
static uint8_t nint;

/* ******************************************************************************** */

void rc5_init (uint8_t addr)
{
        nint  = 0;
        nbits = 0;
        rc5.flip = -1;

        rc5_addr = addr;

#if (RC5_PRESCALE==1024)
        TCCR0 = (1 << CS02) | (1 << CS00);
#elif   (RC5_PRESCALE==256)
        TCCR0 = (1 << CS02);
#elif   (RC5_PRESCALE==64)
        TCCR0 = (1 << CS01) | (1 << CS00);
#else
#error This RC5_PRESCALE is not supported
#endif /* RC5_PRESCALE */

        /* INTx on falling edge */
        /* clear pending INTx */
        /* enable INTx interrupt */
#if (RC5_INT == RC5_INT0)
        MCUCR = (MCUCR | (1 << ISC01)) & ~ (1 << ISC00);
        GIFR = (1 << INTF0);
        GICR |= (1 << INT0);
#elif (RC5_INT == RC5_INT1)
        MCUCR = (MCUCR | (1 << ISC11)) & ~ (1 << ISC10);
        GIFR = (1 << INTF1);
        GICR |= (1 << INT1);
#else
#error please define RC5_INT
#endif /* RC5_INT */
}

/* ******************************************************************************** */

ISR(TIMER0_OVF_vect)
{
    TextAusgabe("ISR Timer0");
        TIMSK &= ~(1 << TOIE0);

        uint8_t _nbits = nbits;
        code_t _code = code;

        if (26 == _nbits)
        {
                _nbits++;
                _code.w <<= 1;
        }

        if (27 == _nbits
                && _code.b[1] >= 0x30 /* AGC == 3 */
                && 0 > rc5.flip)
        {
                uint8_t _rc5_code;
                uint8_t _rc5_addr;
                /* we do the bit manipulation stuff by hand, because of code size */
                _rc5_code = _code.b[0] & 0x3f; /* 0b00111111 : #0..#5 */
                _code.w <<= 2;
                _rc5_addr = _code.b[1] & 0x1f; /* 0b00011111 : #6..#10 */

                if (rc5_addr & 0x80
                        || rc5_addr == _rc5_addr)
                {
                        rc5.code = _rc5_code;
                        rc5.addr = _rc5_addr;
                        signed char flip = 0;
                        if (_code.b[1] & 0x20) /* 0b00100000 : #11 */
                                flip = 1;
                        rc5.flip = flip;
                }
        }

        nint = 0;
        nbits = 0;

        /* INTx on falling edge */
        /* clear pending INTx */
        /* enable INTx interrupt */
#if (RC5_INT == RC5_INT0)
        MCUCR = (MCUCR | (1 << ISC01)) & ~ (1 << ISC00);
        GIFR = (1 << INTF0);
        GICR |= (1 << INT0);
#elif (RC5_INT == RC5_INT1)
        MCUCR = (MCUCR | (1 << ISC11)) & ~ (1 << ISC10);
        GIFR = (1 << INTF1);
        GICR |= (1 << INT1);
#endif
}

/* ******************************************************************************** */

ISR (INT0_vect)
{
        TextAusgabe("ISR INT0");
        code_t _code = code;
        uint8_t _nint = nint;

        uint8_t tcnt0 = TCNT0;
        TCNT0 = 0;

        if (0 == _nint)
        {
                /* INTx on both edges */
#if (RC5_INT == RC5_INT0)
                MCUCR = (MCUCR | (1 << ISC00)) & ~ (1 << ISC01);
#elif (RC5_INT == RC5_INT1)
                MCUCR = (MCUCR | (1 << ISC10)) & ~ (1 << ISC11);
#endif /* RC5_INT */

                TIFR = (1 << TOV0);
                TIMSK |= (1 << TOIE0);
                _code.w = 0;
        }
        else
        {
                /* Number of bits of the just elapsed period */
                uint8_t n = 1;

                /* Bits received so far */
                uint8_t _nbits = nbits;

                /* is TCNT0 close to RC5_TICKS or RC5_TICKS/2 ? */
                if (tcnt0 > RC5_TICKS + RC5_DELTA)
                        goto invalid;
                else if (tcnt0 < RC5_TICKS/2 - RC5_DELTA)
                        goto invalid;
                else if (tcnt0 > RC5_TICKS - RC5_DELTA)
                        n = 2;
                else if (tcnt0 > RC5_TICKS/2 + RC5_DELTA)
                        goto invalid;

                /* store the just received 1 or 2 bits */
                do
                {
                        _nbits++;
                        if (_nbits & 1)
                        {
                                _code.w <<= 1;
                                _code.b[0] |= _nint & 1;
                        }
                }
                while (--n);

                if (0)
                {
                        invalid:

                        /* disable INTx, run into Overflow0 */
#if (RC5_INT == RC5_INT0)
                        GICR &= ~(1 << INT0);
#elif (RC5_INT == RC5_INT1)
                        GICR &= ~(1 << INT1);
#endif /* RC5_INT */

                        _nbits = 0;
                }

                nbits = _nbits;
        }

        code = _code;
        nint = 1+_nint;
}

