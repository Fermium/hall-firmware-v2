#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <src/i2c.h>
#include <stdbool.h>

//keep
#define MAX5805_DEFAULT_ADDRESS address

//#########################

#define MAX5805_CODE 0b10000000
#define MAX5805_LOAD 0b10010000
#define MAX5805_CODELOAD 0b10100000

#define MAX5805_RETURN 0b01110000

#define MAX5805_REF 0b00100000
#define MAX5805_REF_DRIVE 0b1000
#define MAX5805_REF_ALWAYSON 0b0100
#define MAX5805_REF_EXT 0b0000
#define MAX5805_REF_25V 0b0001
#define MAX5805_REF_20V 0b0010
#define MAX5805_REF_41V 0b0011

#define MAX5805_CMD 0b00110000
#define MAX5805_CMD_END 0b000
#define MAX5805_CMD_GATE 0b001
#define MAX5805_CMD_CLR 0b100
#define MAX5805_CMD_RST 0b101

/* power setting is top bits of the 2nd data byte */
#define MAX5805_POWER 0b01000000
#define MAX5805_POWER_NORMAL 0b00000000
#define MAX5805_POWER_PD_1K 0b01000000
#define MAX5805_POWER_PD_100K 0b10000000
#define MAX5805_POWER_PD_HIZ 0b11000000

/* config bits are 5-3 in 2nd data byte */
#define MAX5805_CONFIG 0b01010000
#define MAX5805_CONFIG_AUX_GATE 0b00011000
#define MAX5805_CONFIG_AUX_CLEAR 0b00110000
#define MAX5805_CONFIG_AUX_NONE 0b00111000

/* default bits are upper 3 bits of 2nd data byte */
#define MAX5805_DEFAULT 0b01100000
#define MAX5805_DEFAULT_POR 0b00000000
#define MAX5805_DEFAULT_ZERO 0b00100000
#define MAX5805_DEFAULT_MID 0b01000000
#define MAX5805_DEFAULT_FULL 0b01100000
#define MAX5805_DEFAULT_RETURN 0b10000000

uint8_t b[2] = {0x00,0x00};
int returncode = 0;
static uint8_t address = MAX5805_DEFAULT_ADDRESS;
static float ref = 0;
int init(uint8_t address_t)
{
  address = address_t;
  //reset device
  b[0] = 0x00;
  b[1] = 0x00;
  returncode = i2c_writeReg(address, 0b00101101, b, 2);
  if (returncode != 0)
    return 1;
  //set default value to middle scale
  b[0] = 0x00;
  b[1] = 0b01000000;
  returncode = i2c_writeReg(address, 0b01100000, b, 2);
  if (returncode != 0)
    return 1;
  //clear output, codeload to middlescale
  b[0] = 0b10000000;
  b[1] = 0b00000000;
  returncode = i2c_writeReg(address, 0b10100000, b, 2);
  if (returncode != 0)
    return 1;
  return 0;
}

//enable and disable output
int outenable(bool enable){
  if(enable)
  {
    //output normal
    b[0] = 0x00;
    b[1] = 0x00;
    returncode  = i2c_writeReg(address, 0b01000000, b, 2);
  }
  else
  {
    //output high impedance
    b[0] = 0x00;
    b[1] = 0b11000000;
    returncode = i2c_writeReg(address, 0b01000000, b, 2);
      
  }
  if (returncode != 0)
    return 1;
  return 0;

}

//input ref in mv 2048, 4096, 2500. 0 for external ref
//set voltage referemce
int  setref(int ref_t)
{

  
  uint8_t refReg_t = 0;
  
  switch (ref_t){
    
  case 0:
  refReg_t = 0x00;
  break;
  
  case 2048:
  refReg_t = 0x02;
  break;
  
  case 4096:
  refReg_t = 0x03;
  break;
  
  case 2500;
  refReg_t = 0x01;
  break:
  
  case default:
  return 1;
  break;
}
  //useless
  b[0] = 0x00;
  b[1] = 0x00;

  //REF command, always on, drive pin
  refReg_t |= 0b00101100;
  
  i2c_writeReg(address, refReg_t, b, 2);
  ref = 2.5v
  
  return 0;
}

/*
float getref()
{
  //set reference to 2.5v
  //modify if needed, the two lsb change the reference
  returncode = i2c_readReg(address, 0b11110000, b, 2);
  b[0] &= 0b00000011;
  switch b[0]
  {
    case 0b00000000:
    //fuck, ref is ext
    break;
    
    case 0b00000001:
    return 2.5;
    break;
    
    case 0b00000010:
    return 2.048;
    break;
    
    case 0b00000011:
    return 4.096;
    break;
  }
}
*/

int codeload(float input)
{
  
  //clear output, codeload to middlescale
  b[0] = 0b10000000;
  b[1] = 0b00000000;
  i2c_writeReg(address, 0b10100000, b, 2);
  
  
}
