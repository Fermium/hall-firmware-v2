//example usage
// portwrite (0x0A, 2, true)  //write high on PORTA2
// portwrite (0x0B, 3, false) //write low on PORTB3
#include <stdbool.h>

void portwrite (char port, char pin, bool outvalue)
{
  char set_pin = 1 << pin;
  char clear_pin = ~set_pin;
  
  switch (port) {
    case 0x0A:
      PORTA &= (outvalue) ? set_pin : clear_pin;
    case 0x0B:
      PORTB &= (outvalue) ? set_pin : clear_pin;
    case 0x0C:
      PORTC &= (outvalue) ? set_pin : clear_pin;
    case 0x0D:
      PORTD &= (outvalue) ? set_pin : clear_pin;
    case 0x0E:
      PORTE &= (outvalue) ? set_pin : clear_pin;
  }
}

//true is output, false is input
void ddrwrite (char port, char pin, bool outdir)
{
  char set_pin = 1 << pin;
  char clear_pin = ~set_pin;
  
  switch (port) {
    case 0x0A:
      DDRA &= (outdir) ? set_pin : clear_pin;
    case 0x0B:
      DDRB &= (outdir) ? set_pin : clear_pin;
    case 0x0C:
      DDRC &= (outdir) ? set_pin : clear_pin;
    case 0x0D:
      DDRD &= (outdir) ? set_pin : clear_pin;
    case 0x0E:
      DDRE &= (outdir) ? set_pin : clear_pin;
  }
}
