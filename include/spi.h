/*! \file spi.h \brief SPI interface driver. */
//*****************************************************************************
//
// File Name	: 'spi.h'
// Title		: SPI interface driver
// Author		: Pascal Stang - Copyright (C) 2000-2002
// Created		: 11/22/2000
// Revised		: 06/06/2002
// Version		: 0.6
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup driver_avr
/// \defgroup spi SPI (Serial Peripheral Interface) Function Library (spi.c)
/// \code #include "spi.h" \endcode
/// \par Overview
///		Provides basic byte and word transmitting and receiving via the AVR
///	SPI interface.  Due to the nature of SPI, every SPI communication operation
/// is both a transmit and simultaneous receive.
///
///	\note Currently, only MASTER mode is supported.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef SPI_H
#define SPI_H



// function prototypes

void InitSpi();
void spi_master_send_char (unsigned char c);
#endif

