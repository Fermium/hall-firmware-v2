#ifndef LOCKIN_H
  #define LOCKIN_H
  #define FULL_SCALE 255
  #define DUTY_CYCLE 127
  #define PERIOD 200
  extern "C" {
    #include "../pins/pins.h"
    #include "../timer/timer1.h"
    #include "../dac/max5805.h"
    #include <math.h>
  }
  class LOCKIN {
    private:
      uint16_t period_start;
      int8_t  sgn;
      float current;

    public :
      LOCKIN();
      float get_current(){ return this->current; }
      void set_current(float current){ this->current=current; }
      int evaluate();
      int time_to_transition();
  };

#endif
