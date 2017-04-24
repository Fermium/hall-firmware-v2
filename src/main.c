#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>

#include "pins.h"
#include "lib/data-chan/Device/Bootstrap/main.h"
#include "lib/data-chan/Device/Bootstrap/Custom.h"
#include "lib/adc/ads1115.h"
#include "lib/dac/max5805.h"

int main(void)
{
		//setup of data-chan
		main_setup();
		io_setup();

		while (1)
		{
				//perform usb step
				main_loop();
		}
		return 0;
}

void io_setup()
{
		i2c_init();
		//ads1115_getread();
}

void Process_Async(uint8_t* data) {
		/*
		   data is the content of the packet minus the CRC, the CMD_ASYNC_RESPONSE,
		   and the ID.... so data have (GENERIC_REPORT_SIZE - 1 - 4) size
		 */
}

void Event_Connected(void) {

}

void Event_Disconnected(void) {

}

void Event_Init(void) {
		DDRB = 0xff;
		PORTB = 0x00;

		//initialize max5805
		max5805_init(0x36);
		max5805_setref(2.5);
		max5805_outenable(true);
}

void MainRoutine(void) {
		//_delay_ms(100);
		//max5805_codeload(voltage);


		// An example of measure generation :)
		if (datachan_output_enabled()) {
				measure_t* test_measure = new_nonrealtime_measure(0xFF);
				add_measure(test_measure, 1, 1.1);
				add_measure(test_measure, 2, 1.2);
				add_measure(test_measure, 3, 1.3);
				add_measure(test_measure, 4, 1.4);

				datachan_register_measure(test_measure);

		}
}
