/*!
   \file lock-in.h
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

  #define F_IF_SIGMA 0.00001


  extern "C" {
    #include "../pins/pins.h"
    #include "../timer/timer1.h"
    #include "../dac/max5805.h"
    #include <math.h>
    #include <util/delay.h>

  }
  /*!
     \brief Lock-in class
  */
  class LOCKIN {
    private:

      uint16_t period_start;

      float  lockin_lower = 0.0; /*!< lower current bound, amperes */
      float  lockin_upper = 0.0; /*!< upper current bound, amperes */

      bool lockin_state = 1; /*!< present state. 1 for high (upper bound), 0 for low (lower bound) */
      bool enabled = 0; /*!< if the current generator is enabled (can take action) or disabled (can't) */


      /*!
         \brief Calculate DAC voltage given expected current
         \param current Input current, Amperes
         \return DAC voltage, Volts
      */
      float current_to_voltage(float current){return (current/2.5+0.5*max5805_getref());}

    public :
      LOCKIN();

      /*!
         \brief set the curent (costant current mode)
         \param current input current (amperes)
      */
      void set_current(float current){ this->lockin_upper = current; this->lockin_lower = current;}
      /*!
         \brief Get lower current bound
         \return Lower current bound, in Amperes
      */
      float get_lockin_lower(){ return this->lockin_lower; }
      /*!
         \brief Set lower current bound
         \note if you make the lower and upper bound differ, you'll trigger lock-in mode
         \param lower Lower current bound, in Amperes
      */
      void set_lockin_lower(float lower){ this->lockin_lower=lower; }
      /*!
         \brief Get upper current bound
         \return Upper current bound, in Amperes
      */
      float get_lockin_upper(){ return this->lockin_upper; }
      /*!
         \brief Set upper current bound
         \note if you make the lower and upper bound differ, you'll trigger lock-in mode
         \param upper Upper current bound, in Amperes
      */
      void set_lockin_upper(float upper){ this->lockin_upper=upper; }
      /*!
         \brief Enable or disable the current generator
         \details A disabled current generator will attempt no action in changing the state of the D
         \note Disabling the current generator does not shut it down! use shutdown() !
         \param enabled true or false
      */
      void enable( unsigned char enabled){this->enabled = enabled;}

      void shutdown();
      int evaluate();
      int time_to_transition();


  };

#endif
