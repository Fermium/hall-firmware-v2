/*!
   \file scheduler.h
   \brief Simple task scheduler and execution
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/
#ifndef SCHEDULER_H
  #define SCHEDULER_H

  #if DUMMY == true
  	#warning "The firmware is in dummy mode. Useful just for testing on dev boards"
  #endif
  #ifndef DUMMY
  	#define DUMMY false
  #endif

  #define DUMMY true

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
  #include "../cgen/cgen.h"
  #include "../led/led.h"
  #define NUMBER_OF_TASKS 5


  void run_tasks(ADS1115* ,ADS1115* ,HEATER* ,CGEN* ,LED* );
  int task0(unsigned long);
  int task1(unsigned long);
  int task2(unsigned long);
  int task3(unsigned long);
  int task4(unsigned long);
  int task5(unsigned long);
  bool check_measure(float,uint8_t);
#endif
