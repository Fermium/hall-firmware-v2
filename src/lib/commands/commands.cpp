#include "commands.h"


void set_current_output(float current){
  if(fabs(current)<0.0006){
    max5805_set_to_middlescale(0x36);
  }
  else{
    max5805_codeload(current/2.5+0.5*max5805_getref());
  }
}

void set_heater_state(HEATER* heater,uint8_t power){
  heater->set_duty_cycle(power);
}


void set_channel_gain(ADS1115* adc,uint8_t channel,uint8_t gain){
  adc->setrange(channel,gain);
}
