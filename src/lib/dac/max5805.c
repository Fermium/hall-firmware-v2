/*!
   \file "max5805.c"
   \brief C library to use the MAX5805 DAC
   \note This library support ONLY ONE MAX5805 ic. For more, it should be converted to c++
   \author Davide Bortolami
   \copyright Fermium LABS srl
*/
#include "max5805.h"

static uint8_t address = 0x36; /*!< device i2c address */

static uint8_t b[2] = {0x00,0x00}; /*!< buffer register for i2c transmission */

static float ref = 0.0; /*!< reference voltage of the MAX5805: 2.048V, 2.500V, or 4.096V */

/*!
   \brief "Initialize and configures the max5805"
   \param address_in The I2C address of the device
   \return 0 if everything is fine, 1 if comunication error
*/
int max5805_init(uint8_t address_in)
{
  int returncode = 0;

    i2c_init();

		address = address_in;
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

		max5805_setref(2.5);
}

/*!
    \brief Set the MAX5805 to middlescale
	\description This function is used to set the DAC to the exact LSB-precise middlescale, to shut down the current generator as much as possible
	\return 0 if everything is fine, 1 if comunication error
*/
int max5805_set_to_middlescale()
{
  int returncode = 0;

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

/*!
    \brief Enable and disable the MAX5805 output
	\param enable true=1 for low impedance (enabled), false=0 for high impedance (disabled)
	\note Disabling the output while connected to a circuit that expect a low impedance source can cause many issues
	\note For example, while connected to a voltage to current converter (current source) it may make it oscillate
	\return 0 if everything is fine, 1 if comunication error
*/
int max5805_outenable(bool enable){
  int returncode = 0;

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

/*!
   \brief Set the reference voltage
   \param ref_in The reference voltage, in Volts
   \note You can specify any voltage within +-0.1V voltage from one of the default reference voltages 2.048V, 2.500V, or 4.096V.
   \note The reference voltage used for output calculation will be the one you provide. This allows for calibration value
	 \return 0 if everything is fine, 1 if comunication error

*/
int  max5805_setref(float ref_in)
{
  int returncode = 0;

		float acceptedDiff = 0.1; /*!< the maximun accepted difference from the datasheet's ref values */
		uint8_t reg_t = 0x00;

		if(fabsf(ref_in-2.5) < acceptedDiff)
				reg_t = 0x01;
		else if(fabsf(ref_in-2.048) < acceptedDiff)
				reg_t = 0x02;
		else if(fabsf(ref_in-4.096) < acceptedDiff)
				reg_t = 0x03;
		else
				return 1;

		//useless
		b[0] = 0x00;
		b[1] = 0x00;

		//REF command, always on, drive pin
		reg_t |= 0b00101100;

		returncode = i2c_writeReg(address, reg_t, b, 2);
		if (returncode != 0)
				return 1;

		//if i2c_writeReg, set global reference voltage
		ref = ref_in;

		return 0;
}

/*!
   \brief Get the reference voltage
   \return The reference voltage
*/
float max5805_getref(){
  return ref;
}

/*!
   \brief Load a voltage output into the DAC
   \param input The input voltage, in Volts
	 \return 0 if everything is fine, 1 if comunication error
*/
int max5805_codeload(float input)
 {
   int returncode = 0;

 	 unsigned int d; ///output code

     //d=(Vout*2^n)/vref
  d = (input * (4096.0/ref));

     //max code to 2^12-1 to avoid overflow (and a 0v output!)
  d = d < 4096 ? d : 4095;

  d = d << 4;

 	//split into two bytes
  b[0] = d >> 8;
 	b[1] = d & 0xFF;

 	returncode = i2c_writeReg(address, 0b10100000, b, 2);
     	if (returncode != 0)
 				return 1;
     return 0;
 }

 /*!
    \brief Load a raw code into the output register of the DAC
		\note Avoid using this if possible, use max5805_codeload(). This is for calibration and testing
    \param input The input voltage, from 0 to 4095
 	 \return 0 if everything is fine, 1 if comunication error
 */
 int max5805_codeloadRaw(uint16_t input)
 {
   int returncode = 0;

   uint16_t _input = input << 4;

		//split into two bytes
    b[0] = _input >> 8;
 		b[1] = _input & 0xFF;

 		returncode = i2c_writeReg(address, 0b10100000, b, 2);
     	if (returncode != 0)
 				return 1;
    return returncode;
 }
