/* Counting Milliseconds with Timer1
 * ---------------------------------
 * For more information see
 * http://www.adnbr.co.uk/articles/counting-milliseconds
 *
 * 620 bytes - ATmega168 - 16MHz
 */

#ifndef TIMER1_H
#define TIMER1_H

  // 16MHz Clock
  #ifndef F_CPU
    #define F_CPU 16000000UL
  #endif

  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <util/atomic.h>

  unsigned long timer1_millis(void);
  int timer1_init(void);
#endif
