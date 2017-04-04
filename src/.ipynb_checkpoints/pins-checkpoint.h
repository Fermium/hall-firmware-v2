//example usage
// portwrite (0x0A, 2, true)  //write high on PORTA2
// portwrite (0x0B, 3, false) //write low on PORTB3
#include <stdbool.h>
#include <avr/io.h>


void portwrite (char port, char pincacca, bool outvalue)
{
  unsigned char set_pin = 1 << pincacca;
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