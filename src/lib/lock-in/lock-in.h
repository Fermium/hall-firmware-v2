#ifndef LOCKIN_H
  #define LOCKIN_H
  #define FULL_SCALE 255
  #define DUTY_CYCLE 127
  #define PERIOD 50
  extern "C" {
    #include "../pins/pins.h"
    #include "../timer/timer1.h"
    #include "../dac/max5805.h"
    #include <math.h>
    #include <util/delay.h>

  }
  class LOCKIN {
    private:
      uint16_t period_start;
      float  lower;
      float  upper;

    public :
      LOCKIN();
      float get_lower(){ return this->lower; }
      void set_lower(float lower){ this->lower=lower/2.5+0.5*max5805_getref(); }
      float get_upper(){ return this->upper; }
      void set_upper(float upper){ this->upper=upper/2.5+0.5*max5805_getref(); }
      int evaluate();
      int time_to_transition();
  };

#endif
