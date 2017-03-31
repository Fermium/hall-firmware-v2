/*
 * (c) Anil Kumar Pugalia, 2010. Email: email@sarika-pugs.com
 *
 * ATmega48/88/168, ATmega16/32
 * 
 * Example Blink. Toggles all IO pins at 1Hz
 */

#include <avr/io.h>
#include <util/delay.h>

void init_io(void)
{

}

int main(void)
{
	init_io();

	while (1)
	{
		_delay_ms(500);
	}

	return 0;
}
