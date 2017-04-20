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

void MainRoutine(void) {
	//_delay_ms(100);
	float voltage;
	/*voltage = VoltageReadSingleEnded(ADS1115_ADDR_GND, 0, ADS1115_RANGE_4_096V);
	max5805_codeload(voltage);*/

  // An example of measure generation :)
  if (datachan_output_enabled()) {
    measure_t* test_measure = new_nonrealtime_measure(0xFF);

    add_measure(test_measure, 0, 173.345);
    add_measure(test_measure, 1, 45.5);
    add_measure(test_measure, 2, 56.12);

    datachan_register_measure(test_measure);

  }
}
