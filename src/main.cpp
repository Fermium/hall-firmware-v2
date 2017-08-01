/*!
   \file main.cpp
   \brief Main entry point of the firmware
   \author Davide Bortolami
   \author Simone Tosato
   \author Denis Benato
   \copyright (c) 2017 - Fermium LABS srl
 */

#define F_CPU 16000000UL /*!< CPU clock frequency */

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>
#include "lib/adc/ads1115.h"
#include "lib/heater/heater.h"
#include "lib/cgen/cgen.h"
#include "lib/scheduler/scheduler.h"
#include "lib/led/led.h"

extern "C" {
	#include "lib/pins/pins.h"
	#include "lib/data-chan/Device/Bootstrap/main.h"
	#include "lib/data-chan/Device/Bootstrap/Custom.h"
	#include "lib/dac/max5805.h"
	#include "lib/timer/timer1.h"
}

ADS1115 adc1;
ADS1115 adc2;
CGEN cgen;
HEATER heater(0x0C,6);
LED led(0x0B,0);

/*!
   \brief Initialize I/O
   \note IMPORTANT: the instrument must be in a shutdown and safe state after this function is executed
 */

void io_setup()
{
				//DAC
				max5805_init(0x36);
				max5805_setref(2.5);
				max5805_set_to_middlescale();
				max5805_outenable(true);

				//ADC
				adc1.setaddress(ADS1115_ADDR_GND);
				adc2.setaddress(ADS1115_ADDR_VDD);

				//HEATER
				heater.set_duty_cycle(0);
				heater.set_period_ms(1020); //2000ms
				heater.enable();

				cgen.reset();
				cgen.enable(false);
				//LED
				led.set_duty_cycle(200);
			  led.set_period_ms(1020);//2000ms
				led.enable();
}

/*!
   \brief main function
   \details contains the setup of data-chan, the initialization of the timer to be done after data-chan, the initialization of the I/O and the main event loop
 */
int main(void)
{

				main_setup(); //data-chan setup
				timer1_init();
				io_setup();

				while (1)
				{
								main_loop(); //data-chan event processing
				}
				return 0;
}


/*!
   \brief Asyncronous processing of data-chan data
   \param inData inData is the content of the data-chan packet minus the CRC, the CMD_ASYNC_RESPONSE, and the ID.
          so inDdata has a (GENERIC_REPORT_SIZE - 1 - 4) size.
          The first byte is the data-chan command (0-255, last ones are reserved)
          All other bytes are the payload.

   \param outData output payload for data-chan
   \note to understand how to implement the Host see https://github.com/NeroReflex/data-chan/blob/master/Host/commands.c
 */
void Process_Async(uint8_t* inData,uint8_t* outData) {

				uint8_t* pointer = inData;
				uint8_t command;
				memcpy(&command,pointer,sizeof(uint8_t));
				pointer++;

				switch(command) {

				case 0x01: //set lockin current
								float lower;
								float upper;
								memcpy(&lower,pointer,sizeof(float));
								pointer+=4;
								memcpy(&upper,pointer,sizeof(float));
								cgen.set_lockin_lower(lower);
						    cgen.set_lockin_upper(upper);
						    cgen.enable(true);
								break;

				case 0x02:  //set costant current
								float current;
								memcpy(&current,pointer,sizeof(float));
								cgen.set_current(current);
						    cgen.enable(true);
						    cgen.evaluate();
								break;

				case 0x03:   //set raw current
								uint16_t current_raw;
								memcpy(&current_raw,pointer,sizeof(uint16_t));
								max5805_codeloadRaw(current_raw);
								break;

				case 0x04: //set_heater_state
								uint8_t power;
								memcpy(&power,pointer,sizeof(uint8_t));
								heater.set_duty_cycle(power);
								break;

				case 0x05: //set channel gain
								uint8_t channel,gain;
								memcpy(&channel,pointer,sizeof(uint8_t));
								pointer++;
								memcpy(&gain,pointer,sizeof(uint8_t));
								if(channel/4 == 0) {
												adc1.setrange(channel,gain);
								}
								else{
												adc2.setrange(channel%4,gain);
								}
								break;

				case 0x06:  //reset
								io_setup();
								break;

				}

}

/*!
   \brief IRS on usb connection
 */
void Event_Connected(void) {
				//io_setup();
}

/*!
   \brief IRS on usbdisconnection
 */
void Event_Disconnected(void) {
				// reset to avoid things burning up
				io_setup();
}

/*!
   \brief IRS on data-chan initialize
   \note not sure, should be checked
 */
void Event_Init(void) {
}

/*!
   \brief Data-chan main routine
   \details This routine is to be considered an event-loop, executed only during usb-connection and data-acquisition from the Host
   \details When this is running, the instrument is acquiring data
   \note do not mess with data-chan outside of this, it may be not sending data and it's buffer can overflow quite easly
 */
void MainRoutine(void) {
				if (datachan_output_enabled()) {
								start_task(&adc1,&adc2,&heater,&cgen,&led);
				}
}
