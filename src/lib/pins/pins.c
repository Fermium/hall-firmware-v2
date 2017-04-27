//example usage
// portwrite (0x0A, 2, true)  //write high on PORTA2
// portwrite (0x0B, 3, false) //write low on PORTB3
#include <stdbool.h>
#include <avr/io.h>
#include "pins.h"


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
