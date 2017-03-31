#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>


void setup (void);
void loop (void);

int main (void) {
  setup();
  loop();
  return 0;
}

void setup () {

}

void loop () {
  while (1) {
    _delay_ms(100);
  }
}
