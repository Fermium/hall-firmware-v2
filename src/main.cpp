#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>
#include "lib/adc/ads1115.h"
#include "lib/heater/heater.h"
#include "lib/scheduler/scheduler.h"

extern "C"{
	#include "lib/pins/pins.h"
	#include "lib/data-chan/Device/Bootstrap/main.h"
	#include "lib/data-chan/Device/Bootstrap/Custom.h"
	#include "lib/dac/max5805.h"
	#include "lib/timer/timer1.h"
}
ADS1115 adc1;
ADS1115 adc2;
HEATER heater(0x0C,6);
void io_setup()
{
		adc1.setaddress(ADS1115_ADDR_GND);
		adc2.setaddress(ADS1115_ADDR_VDD);
		heater.set_duty_cycle(200);
		heater.set_period(1020);//2000ms
		heater.enable();
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


		//initialize max5805
		max5805_init(0x36);
		max5805_setref(2.5);
		max5805_outenable(true);
}

//this routine is execute only when the device is connected
void MainRoutine(void) {
	if (datachan_output_enabled()) {
				start_task(&adc1,&adc2,&heater);
		}
		_delay_ms(3);

}
