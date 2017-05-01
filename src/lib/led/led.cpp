#include "led.h"

LED::LED(uint8_t port,uint8_t pin,uint16_t full_scale){
  this->duty_cycle = this->period_start = this-> full_cycle_ms = this->expected_transition =  0;
  this->pin = pin;
  this->port = port;
  this->state = false;
  this->full_scale=full_scale;
  portwrite(this->port,this->pin,false);
  ddrwrite(this->port,this->pin,DDR_OUTPUT);
}

int LED::blink(){
  if(this->state){
    if((timer1_millis()/this->full_cycle_ms) % full_scale>this->duty_cycle ){
      portwrite(this->port,this->pin,false);

    }

    else{
      portwrite(this->port,this->pin,true);
      this->state=true;
    }
    return 0;
  }
  return 1;
}

void LED::enable(){
  this->state = true;
}
void LED::disable(){
  this->state = false;
}
