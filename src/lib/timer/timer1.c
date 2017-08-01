/*!
   \file timer1.h
   \brief Counting Milliseconds with Timer1
   \author adnbr
   \copyright adnbr
*/

#include "timer1.h"

#define CTC_MATCH_OVERFLOW  (( F_CPU / 1000) / 8) /*!< Calculate the value needed for the CTC match value in OCR1A. */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static unsigned long timer1_millis_counter = 0;

/*!
   \brief Interrupt Request Subroutine
*/
ISR (TIMER1_COMPA_vect)
{
    timer1_millis_counter++;
}

/*!
   \brief Get timer1 in milliseconds
   \return timer1 in milliseconds
*/
unsigned long timer1_millis ()
{
  unsigned long millis_return;

    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis_return = timer1_millis_counter;
    }

    return millis_return;
}

/*!
   \brief Initialize timer1
   \return always returns 0
*/
int timer1_init(void)
{
  // CTC mode, Clock/8
  TCCR1B |= (1 << WGM12) | (1 << CS11);

  // Load the high byte, then the low byte
  // into the output compare
  OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
  OCR1AL = CTC_MATCH_OVERFLOW;

  // Enable the compare match interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Now enable global interrupts
  sei();
  return 0;
}
