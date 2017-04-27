#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>
#include "lib/adc/ads1115.h"

extern "C"{
	#include "pins.h"
	#include "lib/data-chan/Device/Bootstrap/main.h"
	#include "lib/data-chan/Device/Bootstrap/Custom.h"
	#include "lib/dac/max5805.h"
	#include "lib/timer/timer1.h"
}
ADS1115 adc;

void io_setup()
{
		adc.setaddress(ADS1115_ADDR_GND);
		adc.setrange(ADS1115_RANGE_4_096V);
}
int main(void)
{
		//setup of data-chan
		main_setup();
		io_setup();
		timer1_init();

		while (1)
		{
				//perform usb step
				main_loop();
		}
		return 0;
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

//this routine is execute only when the device is connected
void MainRoutine(void) {
		// An example of measure generation :)
		float mybeautifularray[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
		/*
		ads1115_config(0x90, 0,0, ADS1115_RANGE_4_096V);
		mybeautifularray[0] = ads1115_getread();

		ads1115_config(0x90, 1,1, ADS1115_RANGE_4_096V);
		mybeautifularray[1] = ads1115_getread();

		ads1115_config(0x90, 2,2, ADS1115_RANGE_4_096V);
		mybeautifularray[2] = ads1115_getread();

		ads1115_config(0x90, 3,3, ADS1115_RANGE_4_096V);
		mybeautifularray[3] = ads1115_getread();
*/
		//ads1115_config(0X92, 0,0, ADS1115_RANGE_4_096V);
		mybeautifularray[4] = adc.get_se_read(3);
		mybeautifularray[5] = adc.get_diff_read(3,1);
		mybeautifularray[6] = adc.get_diff_read(1,3);
		mybeautifularray[7] = timer1_millis();
/*
		ads1115_config(0X92, 1,1, ADS1115_RANGE_4_096V);


		ads1115_config(0X92, 2,2, ADS1115_RANGE_4_096V);
		mybeautifularray[6] = ads1115_getread();

		ads1115_config(0X92, 3,3, ADS1115_RANGE_4_096V);
		mybeautifularray[7] = ads1115_getread();
*/

		if (datachan_output_enabled()) {
				measure_t* test_measure = new_nonrealtime_measure(0xFF);

				add_measure(test_measure, 1, mybeautifularray[0]);
				add_measure(test_measure, 2, mybeautifularray[1]);
				add_measure(test_measure, 3, mybeautifularray[2]);
				add_measure(test_measure, 4, mybeautifularray[3]);
				add_measure(test_measure, 5, mybeautifularray[4]);
				add_measure(test_measure, 6, mybeautifularray[5]);
				add_measure(test_measure, 7, mybeautifularray[6]);
				add_measure(test_measure, 8, mybeautifularray[7]);


				datachan_register_measure(test_measure);
		}
		_delay_ms(3);

		//ads1115_getread();
}
