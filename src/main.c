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
		_delay_ms(20);
		ads1115_config(ADS1115_ADDR_VDD, 1, ADS1115_RANGE_4_096V);
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
				add_measure(test_measure, 1, ads1115_getread());
				add_measure(test_measure, 2, ads1115_getread());
				add_measure(test_measure, 3, ads1115_getread());
				add_measure(test_measure, 4, ads1115_getread());
				add_measure(test_measure, 5, ads1115_getread());
				add_measure(test_measure, 6, ads1115_getread());
				add_measure(test_measure, 7, ads1115_getread());
				add_measure(test_measure, 8, ads1115_getread());
				datachan_register_measure(test_measure);

		}
}
