#include "lock-in.h"

LOCKIN::LOCKIN(){
}

int LOCKIN::evaluate(){
  if((timer1_millis()/PERIOD) % FULL_SCALE>DUTY_CYCLE ){
    max5805_codeload(this->lower);
  }
  else{
    max5805_codeload(this->upper);
  }
  return 0;
}

int LOCKIN::time_to_transition(){
  int t = (timer1_millis()/PERIOD) % FULL_SCALE;
  return fmin(fabs(t-DUTY_CYCLE),fmin(t,FULL_SCALE-t))*PERIOD;
}
