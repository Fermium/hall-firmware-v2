#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <src/pins.h>
#include <src/max5805.h>

void init_io(void)
{
    DDRB = 0xff;
    PORTB = 0x00;
		uint8_t b[2] = {0x00,0x00};
		//reset device
		b[0] = 0x00;
		b[1] = 0x00;
		i2c_writeReg(0x36, 0b00101101, b, 2);
		//set default value to middle scale
		b[0] = 0x00;
		b[1] = 0b01000000;
		i2c_writeReg(0x36, 0b01100000, b, 2);
		//set reference to 2.5v
		//modify if needed, the two lsb change the reference
		b[0] = 0x00;
		b[1] = 0x00;
		i2c_writeReg(0x36, 0b00101101, b, 2);
		//clear output, codeload to middlescale
		b[0] = 0b10000000;
		b[1] = 0b00000000;
		i2c_writeReg(0x36, 0b10100000, b, 2);
		//output normal
		b[0] = 0x00;
		b[1] = 0x00;
		i2c_writeReg(0x36, 0b01000000, b, 2);
		//clear output, codeload to ???
		b[0] = 0xff;
		b[1] = 0b00000000;
		i2c_writeReg(0x36, 0b10100000, b, 2);
		
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
