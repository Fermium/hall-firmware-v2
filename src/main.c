#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <src/pins.h>
#include <src/max5805.h>

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
           // _delay_ms(10);
            //PORTB = 0xff;
            //_delay_ms(10);
            //PORTB = 0x00;
			/*
            for (int i=0; i != 25; i++)
            {
            float cacca;
            cacca = i*0.1;
            max5805_codeload(cacca); 
            _delay_ms(100);            
            }
            */
            
            max5805_codeload(0.5);
        }
		return 0;
}
