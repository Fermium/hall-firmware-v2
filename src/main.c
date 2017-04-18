#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <src/pins.h>
#include <src/max5805.h>
#include <math.h>
#include <src/ads1115.h>

void init_io(void)
{
		DDRB = 0xff;
		PORTB = 0x00;
    
        //initialize max5805
		max5805_init(0x36);
		max5805_setref(2.5);
		max5805_outenable(true);
}

int main(void)
{
		init_io();
		while (1)
		{
						__delay_ms(100);
						float voltage;
						voltage = VoltageReadSingleEnded(ADS1115_ADDR_GND, 0, ADS1115_RANGE_4_096V);
						max5805_codeload(voltage);		
							 
        }
		return 0;
}
