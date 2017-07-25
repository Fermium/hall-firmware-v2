/**
CHANNELS

ADC | CHANNEL | MEANING
 1  |   3-0   | VR
 1  |   3-1   | TEMP
 1  |   3-2   | I-MES
 1  |    3    | 2.5 V
 2  |   3-0   | GAUSS
 2  |   3-1   | VH1
 2  |   3-2   | VH2
 2  |    3    | 2.5 V
**/



#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>
#include "lib/adc/ads1115.h"
#include "lib/heater/heater.h"
#include "lib/lock-in/lock-in.h"
#include "lib/scheduler/scheduler.h"
#include "lib/led/led.h"
#include "lib/commands/commands.h"

extern "C"{
	#include "lib/pins/pins.h"
	#include "lib/data-chan/Device/Bootstrap/main.h"
	#include "lib/data-chan/Device/Bootstrap/Custom.h"
	#include "lib/dac/max5805.h"
	#include "lib/timer/timer1.h"
}
ADS1115 adc1;
ADS1115 adc2;
LOCKIN lock;
HEATER heater(0x0C,6,255);
LED led(0x0B,0,255);
void io_setup()
{
		//initialize max5805
		max5805_init(0x36);
		max5805_setref(2.5);
		max5805_outenable(true);
		adc1.setaddress(ADS1115_ADDR_GND);
		adc2.setaddress(ADS1115_ADDR_VDD);
		heater.set_duty_cycle(0);
		heater.set_period(1020);//2000ms
		heater.enable();
		led.enable();
		led.set_duty_cycle(0);
		led.set_period(1020);//2000ms
}
int main(void)
{

		//setup of data-chan
		main_setup();
		timer1_init();
		io_setup();
		while (1)
		{
				//perform usb step
				main_loop();
		}
		return 0;
}



void Process_Async(uint8_t* inData,uint8_t* outData) {
		/*
		   data is the content of the packet minus the CRC, the CMD_ASYNC_RESPONSE,
		   and the ID.... so data have (GENERIC_REPORT_SIZE - 1 - 4) size
			 il primo byte è il comando (il famoso 0-255 del comando, verificare perchè alcuni potrebbero essere usati, ma probabilmente solo gli ultimi)

			 tutti gli altri byte sono il payload

			 per l'host leggere qua https://github.com/NeroReflex/data-chan/blob/master/Host/commands.c
			 void datachan_send_async_command(datachan_device_t* dev, uint8_t cmdType, uint8_t *cmdBuf, uint8_t cmdBufLength)
			 cdmBuf è il payload da inviare, max (GENERIC_REPORT_SIZE - 1 - 4)
			 cmdType è numero che identifica il tipo di comando
			 cmdBufLength è la lunghezza del buffer per evitare casini



		 */
		 uint8_t* pointer = inData;
		 uint8_t command;
		 memcpy(&command,pointer,sizeof(uint8_t));
		 pointer++;

		 switch(command){

			 case 0x01:
			 	float lower;
				float upper;
				memcpy(&lower,pointer,sizeof(float));
				pointer+=4;
				memcpy(&upper,pointer,sizeof(float));
				set_current_output(&lock,lower,upper);
			 	break;

			 case 0x02:
			 	uint8_t power;
				memcpy(&power,pointer,sizeof(uint8_t));
				heater.set_duty_cycle(power);
				set_heater_state(&heater,power);
			 	break;

			 case 0x03:
				uint8_t channel,gain;
				memcpy(&channel,pointer,sizeof(uint8_t));
				pointer++;
				memcpy(&gain,pointer,sizeof(uint8_t));
				if(channel/4 == 0){
					set_channel_gain(&adc1,channel,gain);
				}
				else{
					set_channel_gain(&adc2,channel%4,gain);
				}
			 	break;


				case 0x04:
				uint16_t current;
				memcpy(&current,pointer,sizeof(uint16_t));
				set_current_output_raw(current);
				break;

		 }

}

void Event_Connected(void) {
	io_setup();
}

void Event_Disconnected(void) {
	// reset to avoid things burning up
	io_setup();
}

void Event_Init(void) {
}

//this routine is execute only when the device is connected
void MainRoutine(void) {
	if (datachan_output_enabled()) {
				start_task(&adc1,&adc2,&heater,&lock);
		}
}
