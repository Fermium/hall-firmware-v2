#include "ads1115.h"

static uint8_t ads1115_range;
static uint8_t ads1115_address;

int ads1115_config(uint8_t address, uint8_t channel, uint8_t range)
{
    uint8_t b[2] = {0x00,0x00};
    //int returncode = 0;


    //    possible settings:
    //
    //    channel from 0 to 4
    //    ADS1115_RANGE_6_144V   +/-6.144V range = Gain 2/3
    //    ADS1115_RANGE_4_096V   +/-4.096V range = Gain 1
    //    ADS1115_RANGE_2_048V   +/-2.048V range = Gain 2 (default)
    //    ADS1115_RANGE_1_024V   +/-1.024V range = Gain 4
    //    ADS1115_RANGE_0_512V   +/-0.512V range = Gain 8
    //    ADS1115_RANGE_0_256V   +/-0.256V range = Gain 16

    uint16_t config = 0x00;

    //Calculate ADS1115 configuration register
    config = ADS1115_REG_CONFIG_CQUE_NONE | // Disable the comparator (default val)
            ADS1115_REG_CONFIG_CLAT_NONLAT | // Non-latching (default val)
            ADS1115_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
            ADS1115_REG_CONFIG_CMODE_TRAD | // Traditional comparator (default val)
            ADS1115_REG_CONFIG_DR_128SPS | // 128 samples per second
            ADS1115_REG_CONFIG_MODE_CONTIN; // continuous sampling

    //Set gain
    switch (range) {
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
    
    ads1115_range = range;
    ads1115_address = address;

    //Set acquisition channel
    switch (channel) {
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
    }

    // Set 'start single-conversion' bit
    //config |= ADS1115_REG_CONFIG_OS_SINGLE;

    //b[0] = (uint8_t) (config >> 8);
    //b[1] = (uint8_t) (config & 0xFF);
    i2c_writeReg(address, ADS1115_REG_POINTER_CONFIG, config, 2);
}


float ads1115_getread() {
    
    uint8_t b[2] = {0x00,0x00};
    uint16_t data = 0x00;


    i2c_readReg(ads1115_address, ADS1115_REG_POINTER_CONVERT, data, 2);

    //data = b[0] << 8;
    //data |= b[1];

    /*
    data = ((uint16_t)I2CRead() << 8);
    I2CAck();
    data |=I2CRead();
    I2CAck();
    I2CStop();

    data = data >> 4; //From 12 bit to 16 bit

*/
    float voltage = 0.0;
    int16_t data2 = data;
    voltage =  data2;

    /*
    //Adjust value for voltage
    switch (ads1115_range) {
            //multiplier = range/4096;
        case (ADS1115_RANGE_0_256V):
            voltage *=3.90625e-6;
            break;
        case (ADS1115_RANGE_0_512V):
            voltage *= 2.5e-4;
            break;
        case (ADS1115_RANGE_1_024V):
            voltage *= 7.8125e-6;
            break;
        case (ADS1115_RANGE_2_048V):
            voltage *= 3.125e-5;
            break;
        case (ADS1115_RANGE_4_096V):
            voltage *= 6.25e-5;
            break;
        case (ADS1115_RANGE_6_144V):
            voltage *= 9.375e-5;
            break;
    }
*/

    return (voltage);

}
