/*!
   \file cgen.cpp
   \brief Advanced asyncronoys costant-current + lock-in contro
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/
#include "cgen.h"

/*!
   \brief empty initializer
*/
CGEN::CGEN(){
  reset();
}

/*!
   \brief Evaluate the lockin and change the DAC output
   \details take no action if not enabled
   \details if lower and upper bounds of the current are identical, disables the lo
 */
int CGEN::evaluate(){

        if(enabled) {
                if(fabs(lockin_lower - lockin_upper) < F_IF_SIGMA) { // if the two bounds are equal: costant current scenario
                        float current;
                        current = (lockin_lower + lockin_upper) / 2; //get the average
                        if(fabs(current)<0.0006){
                          reset();
                        }
                        else{
                        max5805_codeload(current_to_voltage(current));
                      }
                }
                else {
                        if((timer1_millis()/PERIOD) % FULL_SCALE>DUTY_CYCLE ) { //lockin scenario
                                if(!this->lockin_state) { //only if state needs to change
                                        max5805_codeload(current_to_voltage(this->lockin_lower));
                                        this->lockin_state=1;
                                }
                        }
                        else{
                                if(this->lockin_state) { //only if state needs to change
                                        max5805_codeload(current_to_voltage(this->lockin_upper));
                                        this->lockin_state=0;
                                }
                        }
                }

        }
        return 0;
}

/*!
   \brief compute time to transition (present or past, absolute)
   \return time to transition, in milliseconds
 */
int CGEN::time_to_transition(){
        int t = (timer1_millis()/PERIOD) % FULL_SCALE;
        return fmin(fabs(t-DUTY_CYCLE),fmin(t,FULL_SCALE-t))*PERIOD;
}

/*!
   \brief Immediate reset, reset and disabling of the current generator
 */
void CGEN::reset(){
        this->enabled = false;
        this->lockin_lower = 0.0;
        this->lockin_upper = 0.0;
        max5805_set_to_middlescale();
}
