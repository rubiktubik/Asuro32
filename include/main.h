/************************************************************************/
/*                                                                      */
/*                      RC5 Remote Receiver                             */
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>


#define uchar unsigned char
#define uint unsigned int

#define	xRC5_IN		PIND
#define	xRC5		PD2			// IR input low active


volatile uint	rc5_data;
