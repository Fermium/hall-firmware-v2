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

  void set_current_output(float);
  void set_heater_state(HEATER*,uint8_t);
  void set_channel_gain(ADS1115*,uint8_t,uint8_t);
  void set_current_output_raw(uint16_t);

#endif //COMMANDS_H
