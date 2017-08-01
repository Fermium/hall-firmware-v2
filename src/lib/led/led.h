/*!
   \file led.h
   \brief Asyncronous led blinking
   \author Simone Tosato
   \author Davide Bortolami
   \copyright (c) 2017 - Fermium LABS srl
*/

#ifndef LED_H
  #define LED_H
  extern "C" {
    #include "../pins/pins.h"
    #include "../timer/timer1.h"
    #include <math.h>
  }

#define LED_DUTY_CYCLE 127 /*!< duty cycle of the led blinking, from 0 to fullscale */

  class LED {
    private:
      uint8_t port; /*!< port register on the MCU, in exadecimal. Example PORTA = 0x0A */
      uint8_t pin; /*!< pin of the port register from 0x00 to 0x07 */
      uint8_t duty_cycle; /*!< Duty cycle from 0 to full_scale */
      uint16_t period; /*!< Time in milliseconds it takes to complete a full cycle */
      const static uint16_t full_scale = 255; /*!< Max value of the duty cycle, to be considere an always-on state. Usually 100 or 255 */
      bool state; /*!< Present state of the Heater, true for ON, false for OFF */


    public :
      LED(uint8_t port,uint8_t pin);
      /*!
         \brief Set the Pin
         \param pin pin of the port register from 0x00 to 0x07
      */
      void set_pin(uint8_t pin){ this->pin = pin; }
      /*!
         \brief Get the Pin
         \return pin of the port register from 0x00 to 0x07
      */
      uint8_t get_pin(){ return this->pin; }
      /*!
         \brief Set the port
         \param port port register on the MCU, in exadecimal. Example PORTA = 0x0A"
      */
      void set_port(uint8_t port){ this->port = port; }
      /*!
         \brief Gtt the port
         \return port register on the MCU, in exadecimal. Example PORTA = 0x0A"
      */
      uint8_t get_port(){ return this->port; }
      /*!
        \brief Set the period on which full_scale is applied
        \param period The period of a full transition cycle, in milliseconds
      */
      void set_period_ms(uint16_t period){ this->period = period; }
      /*!
      \brief Get the period on which full_scale is applied
      \return The period of a full transition cycle, in milliseconds
	  */
      uint16_t get_period_ms(){ return this->period; }
      /*!
      \brief Get the frequency of a full cycle
      \return the frequency of a full cycle, in Hz
	  */
      uint16_t get_frequency(){ return 1/((float)this->period/1000); }
      /*!
         \brief Set the duty cycle
         \note The duty cycle is the moment, between 0 and full_scale at which the transition happens
         \param duty_cycle the duty cycle from 0 to full_scale (max 255)
      */
      void set_duty_cycle(uint8_t duty_cycle){ this->duty_cycle = duty_cycle; }
	  /*!
		 \brief Get the duty cycle
		 \note The duty cycle is the moment, between 0 and full_scale at which the transition happens
		 \return the duty cycle from 0 to full_scale (max 255)
	  */
      uint8_t get_duty_cycle(){ return this->duty_cycle; }

      /*!
      \brief Get the state of the led control
      \note Not necessarily the same as the actual output state of the led
      \note if false, the led is shutdown
      \return true if enable, false if disable
    */
      bool get_state(){ return this->state; }

      int evaluate();
      void enable();
      void disable();
  };

#endif
