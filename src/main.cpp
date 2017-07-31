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
#include "lib/scheduler/scheduler.h"
#include "lib/led/led.h"
#include "lib/commands/commands.h"

#define USB_VID 0x16D0
#define USB_PID 0x0C9B
#define USB_MANUFACTURER L"FermiumLABS"
#define USB_NAME L"Hall Effect"

extern "C"{
	#include "lib/pins/pins.h"
	#include "lib/data-chan/Device/Bootstrap/main.h"
	#include "lib/data-chan/Device/Bootstrap/Custom.h"
	#include "lib/dac/max5805.h"
	#include "lib/timer/timer1.h"
}

ADS1115 adc1;
ADS1115 adc2;
HEATER heater(0x0C,6,255);
LED led(0x0B,0,255);

/*!
   \brief Initialize I/O
	 \note IMPORTANT: the instrument must be in a shutdown and safe state after this function is executed
*/

void io_setup()
{
	  //DAC
		max5805_init(0x36);
		max5805_setref(2.5);
		max5805_outenable(true);

    //ADC
		adc1.setaddress(ADS1115_ADDR_GND);
		adc2.setaddress(ADS1115_ADDR_VDD);

    //HEATER
		heater.set_duty_cycle(200);
		heater.set_period(1020);//2000ms
		heater.enable();
		/*led.enable();
		led.set_duty_cycle(200);
		led.set_period(1020);//2000ms*/
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

		 switch(command){

			 case 0x01: //set_current_output
			 		float state;
					memcpy(&state,pointer,sizeof(float));
					set_current_output(state);
					_delay_ms(50);
			 		break;

			 case 0x02: //set_heater_state
			 		uint8_t power;
					memcpy(&power,pointer,sizeof(uint8_t));
					set_heater_state(&heater,power);
			 		break;

			 case 0x03: //set_channel_gain
					uint8_t channel,gain;
					memcpy(&channel,pointer,sizeof(uint8_t));
					pointer++;
					memcpy(&gain,pointer,sizeof(uint8_t));
					//we consider the channel from 0 to 15, where 0-7 is the first adc, 7-15 the last
					if(channel/8 == 0){
					set_channel_gain(&adc1,channel%8,gain);
					}
					else{
					set_channel_gain(&adc2,channel%8,gain);
					}

			 		break;

				case 0x04:
 			 		uint16_t current;
 					memcpy(&current,pointer,sizeof(uint16_t));
					set_current_output_raw(current);
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
				task(&adc1,&adc2,&heater);
		}
}
