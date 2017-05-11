#ifndef LED_H
  #define LED_H
  extern "C" {
    #include "../pins/pins.h"
    #include "../timer/timer1.h"
    #include <math.h>
  }
  class LED {
    private:
      uint8_t port;
      uint8_t pin;
      uint8_t duty_cycle;
      bool state;
      uint16_t period_start;
      uint16_t full_cycle_ms;
      uint16_t expected_transition;
      uint16_t full_scale;

    public :
      LED(uint8_t port,uint8_t pin,uint16_t full_scale);
      void set_pin(uint8_t pin){ this->pin = pin; }
      uint8_t get_pin(){ return this->pin; }
      void set_port(uint8_t port){ this->port = port; }
      uint8_t get_port(){ return this->port; }
      void set_period(uint16_t period){ this->full_cycle_ms = period/(float)full_scale; }
      uint16_t get_period(){ return this->full_cycle_ms*(float)full_scale; }
      uint16_t get_frequency(){ return 1/(this->full_cycle_ms*(float)full_scale); }
      void set_duty_cycle(uint8_t duty_cycle){ this->duty_cycle = duty_cycle; }
      void set_status(bool status){ this->duty_cycle = status*0xFF; }
      uint8_t get_duty_cycle(){ return this->duty_cycle; }
      bool get_state(){ return this->state; }
      int blink();
      void enable();
      void disable();
  };

#endif
