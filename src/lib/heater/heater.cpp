/*!
   \file "heater.cpp"
   \brief Asyncronous ultra-low-speed PWM for powerline loads
   \author Simone Tosato
   \author Davide Bortolami
   \copyright Fermium LABS srl
*/
#include "heater.h"

/*!
   \brief Initialize the class, shuts down the heater
   \details Initialize the class, set the pin to a low impedance and low value to shut down the heater
   \param port port register on the MCU, in exadecimal. Example PORTA = 0x0A
   \param pin pin of the port register from 0x00 to 0x07
   \param full_scale Max value of the duty cycle, to be considere an always-on state. Usually 100 or 255
*/
HEATER::HEATER(uint8_t port,uint8_t pin,uint16_t full_scale){
  this->duty_cycle =  0;
  this->pin = pin;
  this->port = port;
  this->state = false;
  this->full_scale=full_scale;
  portwrite(this->port,this->pin,false);
  ddrwrite(this->port,this->pin,DDR_OUTPUT);
}




/*!
   \brief Evaluate if it's time to transition, then do it
   \return 0 if the heater state was enabled, regardless if transition happened or not. 1 if the heater was disabled
*/
int HEATER::evaluate(){
  if(this->state){
    if((timer1_millis()/(this->period/this->full_scale)) % full_scale>this->duty_cycle ){
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

/*!
   \brief Enable the heater. evaluate() will start working for now on
   \note This function does not necessarily power on the heater
*/
void HEATER::enable(){
  this->state = true;
}

/*!
   \brief Disable and power off the heater
*/
void HEATER::disable(){
  this->state = false;
  portwrite(this->port,this->pin,false);
}

/*!
   \brief Get time to transition
   \note This function can be used to preemptively decide if it's a good idea to read something from the ADC of if the system should wait
   \return time to transition, in milliseconds
*/
int HEATER::time_to_transition(){
  int t = (timer1_millis()/(this->period/this->full_scale)) % full_scale;
  return fmin(fabs(t-this->duty_cycle), fmin(t,full_scale-t)) * (this->period/this->full_scale);
}
