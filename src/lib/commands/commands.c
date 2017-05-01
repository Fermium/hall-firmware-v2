#include "commands.h"


float get_current(){

}

bool set_current_output_state(bool state){

}

bool set_heater_state(HEATER* heater,uint8_t power){

}


void set_channel_gain(ADS1115* adc,uint8_t channel,uint8_t gain){
  adc->setrange(channel,gain);
}
