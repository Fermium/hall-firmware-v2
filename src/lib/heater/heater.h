#ifndef HEATER_H
  #define HEATER_H
  extern "C" {
    #include "../pins/pins.h"
    #include "../timer/timer1.h"
    #include <math.h>
  }
  class HEATER {
    private:
      uint8_t port;
      uint8_t pin;
      uint8_t duty_cycle;
      bool state;
      uint16_t period_start;
      uint16_t full_cycle_ms;
      uint16_t expected_transition;

    public :
      HEATER(uint8_t port,uint8_t pin);
      void set_pin(uint8_t pin){ this->pin = pin; }
      uint8_t get_pin(){ return this->pin; }
      void set_port(uint8_t port){ this->port = port; }
      uint8_t get_port(){ return this->port; }
      void set_period(uint16_t period){ this->full_cycle_ms = period/255.0; }
      uint16_t get_period(){ return this->full_cycle_ms*255.0; }
      uint16_t get_frequency(){ return 1/(this->full_cycle_ms*255.0); }
      void set_duty_cycle(uint8_t duty_cycle){ this->duty_cycle = duty_cycle; }
      uint8_t get_duty_cycle(){ return this->duty_cycle; }
      bool get_state(){ return this->state; }
      void start_transition();
      int evaluate();
      void enable();
      void disable();
      int time_to_transition();
  };

#endif
