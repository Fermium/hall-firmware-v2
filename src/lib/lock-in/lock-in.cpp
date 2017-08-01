/*!
   \file lock-in.cpp
   \brief Lock-in current generation
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/
#include "lock-in.h"

LOCKIN::LOCKIN(){
}

/*!
   \brief Evaluate the lockin and change the DAC output
*/
int LOCKIN::evaluate(){
  if((timer1_millis()/PERIOD) % FULL_SCALE>DUTY_CYCLE ){

    if(!this->state){ //only if state needs to change
      max5805_codeload(this->lower);
      this->state=1;
    }

  }
  else{

    if(this->state){ //only if state needs to change
      max5805_codeload(this->upper);
      this->state=0;
    }
  }
  return 0;
}

/*!
   \brief compute time to transition (present or past, absolute)
   \return time to transition, in milliseconds
*/
int LOCKIN::time_to_transition(){
  int t = (timer1_millis()/PERIOD) % FULL_SCALE;
  return fmin(fabs(t-DUTY_CYCLE),fmin(t,FULL_SCALE-t))*PERIOD;
}
