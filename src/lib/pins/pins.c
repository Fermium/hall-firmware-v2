/*!
   \file pins.c
   \brief simple abstraction library for basic I/O on AVR
   \author Davide Bortolami
   \copyright Fermium LABS srl
*/
#include <stdbool.h>
#include <avr/io.h>
#include "pins.h"



/*!
   \brief Write PORT register to set I/O
   \param port The register to write to, in exadecimal. Example 0x0A for PORTA, 0x0C for PORTC
   \param pin The pin of the PORT register to write to, from 0x00 to 0x07
   \param outvalue The output value, true=1=HIGH or false=0=LOW
*/
void portwrite (char port, char pin, bool outvalue)
{
  unsigned char set_pin = 1 << pin;
  unsigned char clear_pin = ~set_pin;

  switch (port) {
    case 0x0B:
      outvalue  ? (PORTB |= set_pin) : (PORTB &= clear_pin);
      break;
    case 0x0C:
      outvalue  ? (PORTC |= set_pin) : (PORTC &= clear_pin);
      break;
    case 0x0D:
      outvalue  ? (PORTC |= set_pin) : (PORTD &= clear_pin);
      break;
    case 0x0E:
      outvalue  ? (PORTE |= set_pin) : (PORTE &= clear_pin);
      break;
  }
}

/*!
   \brief Write DDR register to set I/O direction
   \param port The register to write to, in exadecimal. Example 0x0A for PORTA, 0x0C for PORTC
   \param pin The pin of the PORT register to write to, from 0x00 to 0x07
   \param outvalue The direction of the I/O pin, DDR_INPUT or DDR_OUTPUT
*/
void ddrwrite (char port, char pin, bool outvalue)
{
  unsigned char set_pin = 1 << pin;
  unsigned char clear_pin = ~set_pin;

  switch (port) {
    case 0x0B:
      outvalue  ? (DDRB |= set_pin) : (DDRB &= clear_pin);
      break;
    case 0x0C:
      outvalue  ? (DDRC |= set_pin) : (DDRC &= clear_pin);
      break;
    case 0x0D:
      outvalue  ? (DDRC |= set_pin) : (DDRD &= clear_pin);
      break;
    case 0x0E:
      outvalue  ? (DDRE |= set_pin) : (DDRE &= clear_pin);
      break;
  }
}
