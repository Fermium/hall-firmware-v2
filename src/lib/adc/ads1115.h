//ADS1115 library
//Heavly adapted to the pic architecture, less abstracted i2c library
//Partially ported from Adafruit arduino library https://github.com/adafruit/Adafruit_ADS1X15
#ifndef ADS1115_ROUTINES_H
#define	ADS1115_ROUTINES_H

//include configuration mask registers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include <math.h>
#include <avr/io.h>
#include <stdbool.h>
#include "../i2c/i2c.h"
#include <src/ads1115-config.h>

//Acquire voltage in single ended mode

float VoltageReadSingleEnded(uint8_t address, uint8_t channel, uint8_t range);


#endif	/* ADS1115_ROUTINES_H */
