/*!
   \file "commands.h"
   \brief "functions to execute the received data-chan commands"
   \author "Simone Tosato"
   \copyright "Fermium LABS srl"
*/

#ifndef COMMANDS_H

#define COMMANDS_H
  #include <stdint.h>
  #include <stdbool.h>
  #include <math.h>
  extern "C" {
    #include "../dac/max5805.h"
  }
  #include "../heater/heater.h"
  #include "../adc/ads1115.h"
  #include "../cgen/cgen.h"

  void set_current_lockin(CGEN* ,float,float);
  void set_current_fixed(CGEN*,float);
  void set_heater_state(HEATER*,uint8_t);
  void set_channel_gain(ADS1115*,uint8_t,uint8_t);
  void set_current_raw(uint16_t);

#endif //COMMANDS_H
