#ifndef COMMANDS_H

  #define COMMANDS_H
  #include <datachan.h>
  #include <stdint.h>
  #include <stdbool.h>



  float get_current();
  bool set_current_output_state(bool);
  bool set_heater_state(uint8_t);
  bool set_channel_gain(uint8_t,float);

#endif //COMMANDS_H
