/*
 * (c) Anil Kumar Pugalia, 2010. Email: email@sarika-pugs.com
 *
 * ATmega48/88/168, ATmega16/32
 * 
 * Example Blink. Toggles all IO pins at 1Hz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <src/pins.h>

void init_io(void)
{
    DDRB = 0xff;
    PORTB = 0x00;
}

int main(void)
{
	init_io();

	while (1)
	{
        portwrite(0x0b, 0, true);
		_delay_ms(500);
        portwrite(0x0b, 0, false);
		_delay_ms(500);
	}

	return 0;
}
