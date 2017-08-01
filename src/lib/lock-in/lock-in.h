/*!
   \file lock-in.cpp
   \brief Lock-in current generation
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/

#ifndef LOCKIN_H
  #define LOCKIN_H
  #define FULL_SCALE 255
  #define DUTY_CYCLE 127
  #define PERIOD 1
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
      float  lower = 0.0; /*!< lower current bound, amperes */
      float  upper = 0.0; /*!< upper current bound, amperes */
      char state = 1; /*!< present state. 1 for high (upper bound), 0 for low (lower bound) */

    public :
      LOCKIN();
      /*!
         \brief Get lower current bound
         \return Lower current bound, in Amperes
      */
      float get_lower(){ return this->lower; }
      /*!
         \brief Set lower current bound
         \param lower Lower current bound, in Amperes
      */
      void set_lower(float lower){ this->lower=lower/2.5+0.5*max5805_getref(); }
      /*!
         \brief Get upper current bound
         \return Upper current bound, in Amperes
      */
      float get_upper(){ return this->upper; }
      /*!
         \brief Set upper current bound
         \param upper Upper current bound, in Amperes
      */
      void set_upper(float upper){ this->upper=upper/2.5+0.5*max5805_getref(); }
      int evaluate();
      int time_to_transition();
  };

#endif
