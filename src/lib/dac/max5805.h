#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <stdbool.h>
#include "../i2c/i2c.h"


int max5805_init(uint8_t address_t);

//enable and disable output
int max5805_outenable(bool enable);
int max5805_set_to_middlescale(uint8_t address_t);


//input ref in volt, floats. The value you input will be assigned as the real reference voltage to do calculations.
//nearest value will be choosen by 100mV margin
//external reference is not supported here
int  max5805_setref(float ref_t);
float  max5805_getref();


int max5805_codeload(float input);

int max5805_codeloadRaw(uint16_t input);
