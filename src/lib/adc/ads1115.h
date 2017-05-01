//ADS1115 library
//Heavly adapted to the pic architecture, less abstracted i2c library
//Partially ported from Adafruit arduino library https://github.com/adafruit/Adafruit_ADS1X15
#ifndef ADS1115_ROUTINES_H
#define	ADS1115_ROUTINES_H
//Config register masks for ADS1115, from adafruit arduino library
// https://github.com/adafruit/Adafruit_ADS1X15

#define ADS1115_REG_CONFIG_OS_MASK      (0x8000)
#define ADS1115_REG_CONFIG_OS_SINGLE    (0x8000)  // Write: Set to start a single-conversion
#define ADS1115_REG_CONFIG_OS_BUSY      (0x0000)  // Read: Bit = 0 when conversion is in progress
#define ADS1115_REG_CONFIG_OS_NOTBUSY   (0x8000)  // Read: Bit = 1 when device is not performing a conversion

#define ADS1115_REG_POINTER_MASK        (0x03)
#define ADS1115_REG_POINTER_CONVERT     (0x00)
#define ADS1115_REG_POINTER_CONFIG      (0x01)
#define ADS1115_REG_POINTER_LOWTHRESH   (0x02)
#define ADS1115_REG_POINTER_HITHRESH    (0x03)

#define ADS1115_REG_CONFIG_MUX_MASK     (0x7000)
#define ADS1115_REG_CONFIG_MUX_DIFF_0_1 (0x0000)  // Differential P = AIN0, N = AIN1 (default)
#define ADS1115_REG_CONFIG_MUX_DIFF_0_3 (0x1000)  // Differential P = AIN0, N = AIN3
#define ADS1115_REG_CONFIG_MUX_DIFF_1_3 (0x2000)  // Differential P = AIN1, N = AIN3
#define ADS1115_REG_CONFIG_MUX_DIFF_2_3 (0x3000)  // Differential P = AIN2, N = AIN3
#define ADS1115_REG_CONFIG_MUX_SINGLE_0 (0x4000)  // Single-ended AIN0
#define ADS1115_REG_CONFIG_MUX_SINGLE_1 (0x5000)  // Single-ended AIN1
#define ADS1115_REG_CONFIG_MUX_SINGLE_2 (0x6000)  // Single-ended AIN2
#define ADS1115_REG_CONFIG_MUX_SINGLE_3 (0x7000)  // Single-ended AIN3

#define ADS1115_REG_CONFIG_PGA_MASK     (0x0E00)
#define ADS1115_REG_CONFIG_PGA_6_144V   (0x0000)  // +/-6.144V range = Gain 2/3
#define ADS1115_REG_CONFIG_PGA_4_096V   (0x0200)  // +/-4.096V range = Gain 1
#define ADS1115_REG_CONFIG_PGA_2_048V   (0x0400)  // +/-2.048V range = Gain 2 (default)
#define ADS1115_REG_CONFIG_PGA_1_024V   (0x0600)  // +/-1.024V range = Gain 4
#define ADS1115_REG_CONFIG_PGA_0_512V   (0x0800)  // +/-0.512V range = Gain 8
#define ADS1115_REG_CONFIG_PGA_0_256V   (0x0A00)  // +/-0.256V range = Gain 16



#define ADS1115_REG_CONFIG_MODE_MASK    (0x0100)
#define ADS1115_REG_CONFIG_MODE_CONTIN  (0x0000)  // Continuous conversion mode
#define ADS1115_REG_CONFIG_MODE_SINGLE  (0x0100)  // Power-down single-shot mode (default)

#define ADS1115_REG_CONFIG_DR_MASK      (0x00E0)
#define ADS1115_REG_CONFIG_DR_8SPS      (0x0000)
#define ADS1115_REG_CONFIG_DR_16SPS     (0x0020)
#define ADS1115_REG_CONFIG_DR_32SPS     (0x0040)
#define ADS1115_REG_CONFIG_DR_64SPS     (0x0060)
#define ADS1115_REG_CONFIG_DR_128SPS    (0x0080)
#define ADS1115_REG_CONFIG_DR_250SPS    (0x00A0)
#define ADS1115_REG_CONFIG_DR_475SPS    (0x00C0)
#define ADS1115_REG_CONFIG_DR_860SPS    (0x00E0)
const int sp[8]={8,16,32,64,128,250,475,860};
#define ADS1115_REG_CONFIG_CMODE_MASK   (0x0010)
#define ADS1115_REG_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1115_REG_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator

#define ADS1115_REG_CONFIG_CPOL_MASK    (0x0008)
#define ADS1115_REG_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1115_REG_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active

#define ADS1115_REG_CONFIG_CLAT_MASK    (0x0004)  // Determines if ALERT/RDY pin latches once asserted
#define ADS1115_REG_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1115_REG_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator

#define ADS1115_REG_CONFIG_CQUE_MASK    (0x0003)
#define ADS1115_REG_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1115_REG_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1115_REG_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1115_REG_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)

//added by me, custom values to be used to select range

#define ADS1115_RANGE_6_144V                    (6)  // +/-6.144V range = Gain 2/3
#define ADS1115_RANGE_4_096V                    (5)  // +/-4.096V range = Gain 1
#define ADS1115_RANGE_2_048V                    (4)  // +/-2.048V range = Gain 2 (default)
#define ADS1115_RANGE_1_024V                    (3)  // +/-1.024V range = Gain 4
#define ADS1115_RANGE_0_512V                    (2)  // +/-0.512V range = Gain 8
#define ADS1115_RANGE_0_256V                    (1)  // +/-0.256V range = Gain 16
const float ADS1115_RANGE_CORRECTION[6] = {7.8125e-06, 1.5625e-05, 3.125e-05, 6.25e-05, 0.000125, 0.0001875};

#define ADS1115_ADDR_GND 0b10010000
#define ADS1115_ADDR_VDD 0b10010010
#define ADS1115_ADDR_SDA 0b10010100
#define ADS1115_ADDR_SCL 0b10010110


//include configuration mask registers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include <math.h>
#include <avr/io.h>
#include <stdbool.h>

extern "C" {
  #include "../i2c/i2c.h"
}

  class ADS1115
  {
    private :
       uint8_t range[8];
       uint8_t address;
       uint8_t startch;
       uint8_t endch;
       uint16_t sps;
       int config(uint8_t startch,uint8_t endch);


    public  :
       ADS1115();
       float get_se_read(uint8_t startch);
       float get_diff_read(uint8_t startch,uint8_t endch);
       uint8_t getaddress(){ return this->address; }
       void setaddress(uint8_t address){ this->address=address; }
       uint8_t getrange(uint8_t channel){ return this->range[channel]; }
       void setrange(uint8_t channel,uint8_t range){ this->range[channel]=range;uint8_t* ch=get_channels(channel); this->config(ch[0],ch[1]); }
       uint16_t getsps(){ return sp[(this->sps>>4)/2]; }
       uint16_t getsp(){ return (float)1/sp[(this->sps>>4)/2]*1000; }
       void setsps(uint16_t sps){ this->sps=sps; }
       uint16_t get_channel_cfg(uint8_t startch,uint8_t endch);
       uint8_t* get_channels(uint16_t channel);

  };
  #endif
