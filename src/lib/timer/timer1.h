/*!
   \file timer1.h
   \brief Counting Milliseconds with Timer1
   \author adnbr
   \copyright adnbr
*/

#ifndef TIMER1_H
#define TIMER1_H

  // 16MHz Clock
  #ifndef F_CPU
    #define F_CPU 16000000UL /*!< CPU clock frequency */
  #endif

  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <util/atomic.h>

  unsigned long timer1_millis(void);
  int timer1_init(void);
#endif
