/*!
   \file "commands.cpp"
   \brief "functions to execute the received data-chan commands"
   \author "Simone Tosato"
   \copyright "Fermium LABS srl"
*/
#include "commands.h"

/*!
   \brief Set the out current
   \details if the current is I<0.0006 the current generator will be disabled.
   \note The functions sets the current with withouth any serious calibration value.
   \note It assumes a perfect 2.5A/V voltage-->current conversion and no offset voltage of any kind
   \note To obtain a decent precision, the values needs to be sent pre-calibrated
   \param current the input current, in A (Amperes)
*/
void set_current_lockin(CGEN* cgen,float lower,float upper){
    cgen->set_lockin_lower(lower);
    cgen->set_lockin_upper(upper);
    cgen->enable(true);
    //max5805_codeload(current/2.5+0.5*max5805_getref());
}

void set_current_fixed(CGEN* cgen,float current){
   cgen->set_current(current);
   cgen->enable(true);
   cgen->evaluate();
}
/*!
   \brief Set the heater power
   \param heater the pointer to the heater's class istance
   \param power Power from 0 (off) to the heater class full_scale
*/
void set_heater_state(HEATER* heater,uint8_t power){
  heater->set_duty_cycle(power);
}

/*!
   \brief Set channel gain
   \param adc The ADS115 class istance
   \param channel The channel in 0-7 notation
   \gain the gain from 0-7 where 0 is the highest gain and 7 the lowest gain
*/
void set_channel_gain(ADS1115* adc,uint8_t channel,uint8_t gain){
  adc->setrange(channel,gain);
}
/*!
   \brief Set the output with a raw DAC code
   \note The raw code is used mostly for calibration and debugging
   \param current The current raw code
*/
void set_current_raw(uint16_t current){
    max5805_codeloadRaw(current);
}
