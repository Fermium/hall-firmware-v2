/*!
   \file "max5805.h"
   \brief C library to use the MAX5805 DAC
   \note This library support ONLY ONE MAX5805 ic. For more, it should be converted to c++
   \author Davide Bortolami
   \copyright Fermium LABS srl
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <stdbool.h>
#include "../i2c/i2c.h"


int max5805_init(uint8_t address_in);
int max5805_outenable(bool enable);
int max5805_set_to_middlescale();
int  max5805_setref(float ref_in);
float  max5805_getref();
int max5805_codeload(float input);
int max5805_codeloadRaw(uint16_t input);
