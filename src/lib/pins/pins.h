#ifndef PINS_H
#define PINS_H

  //example usage
  // portwrite (0x0A, 2, true)  //write high on PORTA2
  // portwrite (0x0B, 3, false) //write low on PORTB3
  #include <stdbool.h>
  #include <avr/io.h>

  #define DDR_OUTPUT 1
  #define DDR_INPUT 0

  void portwrite (char port, char pin, bool outvalue);
  void ddrwrite (char port, char pin, bool outvalue);
#endif
