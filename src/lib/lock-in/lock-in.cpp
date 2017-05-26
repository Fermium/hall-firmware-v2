#include "lock-in.h"

LOCKIN::LOCKIN(){
}

int LOCKIN::evaluate(){
  if((timer1_millis()/PERIOD) % FULL_SCALE>DUTY_CYCLE ){
    this-> sgn = -1;
  }
  else{
    this -> sgn = 1;
  }
  max5805_codeload((this->current/2.5+0.5*max5805_getref())*this->sgn);
  return 0;
}

int LOCKIN::time_to_transition(){
  int t = (timer1_millis()/PERIOD) % FULL_SCALE;
  return fmin(fabs(t-DUTY_CYCLE),fmin(t,FULL_SCALE-t))*PERIOD;
}
