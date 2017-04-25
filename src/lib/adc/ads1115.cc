#include "ads1115.h"

ADS115::ADS115(uint8_t address){
  i2c_init();
  this->address=address;
}

int ADS115::config(uint8_t startch,uint8_t endch){
  uint16_t config = 0x0000;
  config = ADS1115_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
       ADS1115_REG_CONFIG_CLAT_NONLAT | // Non-latching (default val)
       ADS1115_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
       ADS1115_REG_CONFIG_CMODE_TRAD | // Traditional comparator (default val)
       ADS1115_REG_CONFIG_DR_128SPS | // 128 samples per second
       ADS1115_REG_CONFIG_MODE_CONTIN; // continuous sampling
  switch (this->range) {
    case (ADS1115_RANGE_0_256V):
      config |= ADS1115_REG_CONFIG_PGA_0_256V;
    break;
    case (ADS1115_RANGE_0_512V):
      config |= ADS1115_REG_CONFIG_PGA_0_512V;
    break;
    case (ADS1115_RANGE_1_024V):
      config |= ADS1115_REG_CONFIG_PGA_1_024V;
    break;
    case (ADS1115_RANGE_2_048V):
      config |= ADS1115_REG_CONFIG_PGA_2_048V;
    break;
    case (ADS1115_RANGE_4_096V):
      config |= ADS1115_REG_CONFIG_PGA_4_096V;
    break;
    case (ADS1115_RANGE_6_144V):
      config |= ADS1115_REG_CONFIG_PGA_6_144V;
    break;
  }
  if (startch > endch)
  {
      uint8_t temp;
      temp = startch;
      startch = endch;
      endch = temp;
  }
  if (startch == endch)
  {
      //Set acquisition channel
      switch (startch) {
      case (0):
          config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
          break;
      case (1):
          config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
          break;
      case (2):
          config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
          break;
      case (3):
          config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
          break;
      default:
          return 1;
          break;
      }
  }
  else
  {
      if (endch == 1)
      {
          if (startch == 0)
          {
              config |= ADS1115_REG_CONFIG_MUX_DIFF_0_1;
          }
          else
          {
              return 1;
          }
      }
      else if (endch == 3)
      {
          switch (startch) {
          case (0):
              config |= ADS1115_REG_CONFIG_MUX_DIFF_0_3;
              break;
          case (1):
              config |= ADS1115_REG_CONFIG_MUX_DIFF_1_3;
              break;
          case (2):
              config |= ADS1115_REG_CONFIG_MUX_DIFF_2_3;
              break;
          default:
              return 1;
              break;
          }
      } else
      {
          return 1;
      }

  }
  uint8_t b[2];
  b[0] = (uint8_t) (config >> 8);
  b[1] = (uint8_t) (config & 0xFF);
  i2c_writeReg(address, ADS1115_REG_POINTER_CONFIG, b, 2);
}

float ADS1115::get_se_read(uint8_t startch){
  return this->get_diff_read(startch,startch)

}

float ADS1115::get_diff_read(uint8_t startch,uint8_t endch){
  if(startch!=this->statch || endch!=this->endch){
    this->config(startch,endch);
  }
  uint8_t b[2] = {0x00,0x00};
  i2c_readReg(this->address, ADS1115_REG_POINTER_CONVERT, b, 2);

  int16_t data = 0x0000;

  data = ((uint16_t)b[0] << 8) | b[1];

  float voltage;
  voltage = data;

  /*
     //Adjust value for voltage
     switch (ads1115_range) {
          //multiplier = range/2^16;
      case (ADS1115_RANGE_0_256V):
          voltage *=3.90625e-6;
          voltage *=2;
          break;
      case (ADS1115_RANGE_0_512V):
          voltage *= 2.5e-4;
          voltage *=2;
          break;
      case (ADS1115_RANGE_1_024V):
          voltage *= 7.8125e-6;
          voltage *=2;
          break;
      case (ADS1115_RANGE_2_048V):
          voltage *= 3.125e-5;
          voltage *=2;
          break;
      case (ADS1115_RANGE_4_096V):
          voltage *= 6.25e-5;
          voltage *=2;
          break;
      case (ADS1115_RANGE_6_144V):
          voltage *= 9.375e-5;
          voltage *=2;
          break;
     }
   */
  return (voltage);

}
