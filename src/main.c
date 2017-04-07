#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <src/pins.h>
#include <src/max5805.h>
#include <math.h>

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
           for (int i = 0; i !=360; i++)
            
               max5805_codeload(sin(i*(3.14159265 / 180.0))+1.0);
            
    
        }
		return 0;
}
