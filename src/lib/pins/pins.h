/*!
   \file pins.h
   \brief simple abstraction library for basic I/O on AVR
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/
#ifndef PINS_H
#define PINS_H

  // example usage
  // portwrite (0x0A, 2, true)  //write high on PORTA2
  // portwrite (0x0B, 3, false) //write low on PORTB3
  #include <stdbool.h>
  #include <avr/io.h>

  #define DDR_OUTPUT 1 /*!< value of the DDR register for output direction */
  #define DDR_INPUT 0  /*!< value of the DDR register for input direction */

  #define HIGH 1 /*!< value of the PORT register for output state */
  #define LOW 0/*!< value of the PORT register for output state */

  void portwrite (char port, char pin, bool outvalue);
  void ddrwrite (char port, char pin, bool outvalue);
#endif
