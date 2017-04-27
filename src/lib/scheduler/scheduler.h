#ifndef SCHEDULER_H
  #define SCHEDULER_H

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdint.h>
  #include <math.h>

  extern "C"{
    #include "../timer/timer1.h"
	  #include "../data-chan/Device/Bootstrap/Custom.h"
  }
  #include "../adc/ads1115.h"
  #include "../heater/heater.h"

  #define NUMBER_OF_TASKS 4


  void start_task(ADS1115*,ADS1115*,HEATER*);

  //tasks
  int task0(unsigned long, unsigned long);
  int task1(unsigned long, unsigned long);
  int task2(unsigned long, unsigned long);
  int task3(unsigned long, unsigned long);
  int task4(unsigned long, unsigned long);
  int task5(unsigned long, unsigned long);
#endif
