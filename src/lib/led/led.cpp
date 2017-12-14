/*!
   \file led.cpp
   \brief Asyncronous led blinking
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/
#include "led.h"

/*!
   \brief Initialize the class, shuts down the led
   \details Initialize the class, set the pin to a low impedance and low value to shut down the led
   \param port port register on the MCU, in exadecimal. Example PORTA = 0x0A
   \param pin pin of the port register from 0x00 to 0x07
   \param full_scale Max value of the duty cycle, to be considere an always-on state. Usually 100 or 255
*/
LED::LED(uint8_t port,uint8_t pin){
  this->duty_cycle =  0;
  this->pin = pin;
  this->port = port;
  this->state = false;
  this->duty_cycle = LED_DUTY_CYCLE;
  portwrite(this->port,this->pin,false);
  ddrwrite(this->port,this->pin,DDR_OUTPUT);
}

/*!
   \brief Evaluate if it's time to blink, then do it
   \return 0 if the led state was enabled, regardless if transition happened or not. 1 if the led was disabled
*/
int LED::evaluate(){
  this->last_evaluation = timer1_millis();

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
   \brief Enable the led. evaluate() will start working for now on
   \note This function does not necessarily power on the led
*/
void LED::enable(){
  this->state = true;
}

/*!
   \brief Disable and power off the led
*/
void LED::disable(){
  this->state = false;
  portwrite(this->port,this->pin,false);
}

/*!
   \brief Shortcut to turn on the led
*/
void LED::on(){
  this->state = false;
  this->duty_cycle =  full_scale;
  portwrite(this->port,this->pin,true);
}

/*!
\brief Shortcut to turn off the led
*/
void LED::off(){
  this->state = false;
  this->duty_cycle =  0;
  portwrite(this->port,this->pin,false);

}



/*!
\brief Turn off if it has not been running for 4 cycles
*/
void LED::watchdog(){
  if ((timer1_millis() - this->last_evaluation) > 10000UL)
  {
    this->disable();
  }
}
