#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <src/pins.h>
#include <src/max5805.h>

void init_io(void)
{
		DDRB = 0xff;
		PORTB = 0x00;
		max5805_init(0x36);
		max5805_setref(2.5);
		max5805_codeload(0);
		max5805_outenable(true);
}

int main(void)
{
		init_io();
		while (1)
		{
			
            for (int i=0; i != 2500; i++)
            {
            float cacca;
            cacca = i*0.01;
            max5805_codeload(cacca); 
            _delay_ms(10);            
            }
            
        

        }
		return 0;
}
