#include "ads1115.h"

ADS1115::ADS1115(){
  i2c_init();
  for(uint8_t i=0;i!=8;i++){
    this->range[i]=ADS1115_RANGE_4_096V;
  }
  this->startch=this->endch=0;
  this->sps=ADS1115_REG_CONFIG_DR_860SPS;
}

int ADS1115::config(uint8_t startch,uint8_t endch){
  uint16_t config = 0x0000;
  config = ADS1115_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
       ADS1115_REG_CONFIG_CLAT_NONLAT | // Non-latching (default val)
       ADS1115_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
       ADS1115_REG_CONFIG_CMODE_TRAD | // Traditional comparator (default val)
       ADS1115_REG_CONFIG_DR_860SPS | // 128 samples per second
       ADS1115_REG_CONFIG_MODE_CONTIN; // continuous sampling

  uint16_t channel_cfg = this->get_channel_cfg(startch,endch);
  config |= channel_cfg;

  switch (this->range[channel_cfg>>12]) {
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


  uint8_t b[2];
  b[0] = (uint8_t) (config >> 8);
  b[1] = (uint8_t) (config & 0xFF);

  i2c_writeReg(address, ADS1115_REG_POINTER_CONFIG, b, 2);
  return 0;
}

float ADS1115::get_se_read(uint8_t startch){
  return this->get_diff_read(startch,startch);
}

float ADS1115::get_diff_read(uint8_t startch,uint8_t endch){
  this->config(startch,endch);
  int8_t sgn = (startch>endch) ? -1 : 1;

  _delay_ms(15);

  uint8_t b[2] = {0x00,0x00};
  i2c_readReg(this->address, ADS1115_REG_POINTER_CONVERT, b, 2);

  int16_t data = 0x0000;

  data = ((uint16_t)b[0] << 8) | b[1];

  float voltage;
  voltage = data;

  voltage *= ADS1115_RANGE_CORRECTION[this->range[this->get_channel_cfg(startch,endch)>>12]-1];
  voltage *= sgn;

  return (voltage);

}

uint16_t ADS1115::get_channel_cfg(uint8_t startch,uint8_t endch){
  uint16_t ch = 0x0000;
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
          ch |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
          break;
      case (1):
          ch |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
          break;
      case (2):
          ch |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
          break;
      case (3):
          ch |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
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
              ch |= ADS1115_REG_CONFIG_MUX_DIFF_0_1;
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
              ch |= ADS1115_REG_CONFIG_MUX_DIFF_0_3;
              break;
          case (1):
              ch |= ADS1115_REG_CONFIG_MUX_DIFF_1_3;
              break;
          case (2):
              ch |= ADS1115_REG_CONFIG_MUX_DIFF_2_3;
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
  return ch;
}


uint8_t* ADS1115::get_channels(uint16_t channel){
  uint8_t ch[2]={0};
  switch (channel) {
    case 0:
      ch[0]=0;
      ch[1]=1;
    break;
    case 1:
      ch[0]=0;
      ch[1]=3;
    break;
    case 2:
      ch[0]=1;
      ch[1]=3;
    break;
    case 3:
      ch[0]=2;
      ch[1]=3;
    break;
    case 4:
      ch[0]=0;
      ch[1]=0;
    break;
    case 5:
      ch[0]=1;
      ch[1]=1;
    break;
    case 6:
      ch[0]=2;
      ch[1]=2;
    break;
    case 7:
      ch[0]=3;
      ch[1]=3;
    break;
  }
  return ch;
}
