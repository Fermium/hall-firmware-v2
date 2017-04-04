#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

// standard gnu libc library

#define DAC_ADDR(x) (0x18 | (x & 0x3))


#define MAX580X_CODE 0b10000000
#define MAX580X_LOAD 0b10010000
#define MAX580X_CODELOAD 0b10100000

#define MAX580X_RETURN 0b01110000


#define MAX580X_REF 0b00100000
#define MAX580X_REF_DRIVE 0b1000
#define MAX580X_REF_ALWAYSON 0b0100
#define MAX580X_REF_EXT 0b0000
#define MAX580X_REF_25V 0b0001
#define MAX580X_REF_20V 0b0010
#define MAX580X_REF_41V 0b0011

#define MAX580X_CMD 0b00110000
#define MAX580X_CMD_END 0b000
#define MAX580X_CMD_GATE 0b001
#define MAX580X_CMD_CLR 0b100
#define MAX580X_CMD_RST 0b101

/* power setting is top bits of the 2nd data byte */
#define MAX580X_POWER 0b01000000
#define MAX580X_POWER_NORMAL 0b00000000
#define MAX580X_POWER_PD_1K 0b01000000
#define MAX580X_POWER_PD_100K 0b10000000
#define MAX580X_POWER_PD_HIZ 0b11000000
/* config bits are 5-3 in 2nd data byte */
#define MAX580X_CONFIG 0b01010000
#define MAX580X_CONFIG_AUX_GATE 0b00011000
#define MAX580X_CONFIG_AUX_CLEAR 0b00110000
#define MAX580X_CONFIG_AUX_NONE 0b00111000
/* default bits are upper 3 bits of 2nd data byte */
#define MAX580X_DEFAULT 0b01100000
#define MAX580X_DEFAULT_POR 0b00000000
#define MAX580X_DEFAULT_ZERO 0b00100000
#define MAX580X_DEFAULT_MID 0b01000000
#define MAX580X_DEFAULT_FULL 0b01100000
#define MAX580X_DEFAULT_RETURN 0b10000000

/* magic numbers for ident */
#define MAX580X_ID_5803 0x8a
#define MAX580X_ID_5804 0x92
#define MAX580X_ID_5805 0x82

/* query and report on an attached DAC */
int max580x_init(i2c_portname_t port, uint8_t addr) {
    uint8_t buf[3], n = 0;

    buf[0] = 0x00; 
    buf[1] = 0; 
    buf[2] = 0;
//    i2c_write_nostop(port,DAC_ADDR(addr),buf,1);
//    i2c_read_restart(port,DAC_ADDR(addr),buf,2);

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    i2c_write(port,buf,1,i2c_more);
    i2c_start(port,DAC_ADDR(addr),i2c_mode_read);
    i2c_read(port,buf,2,i2c_stop);

    /* buf should contain some interesting bytes */

    switch (buf[1]) {
        case MAX580X_ID_5803:
            console_message(0,"max580x: Found MAX5803 8-bit DAC");
            break;
        case MAX580X_ID_5804:
            console_message(0,"max580x: Found MAX5804 10-bit DAC");
            break;
        case MAX580X_ID_5805:
            console_message(0,"max580x: Found MAX5805 12-bit DAC");
            break;
        default:
            console_message(0,"max580x: No DAC found");
            return -ENODEV;
            break;
    }
    
    
    buf[0] = MAX580X_CONFIG;
    buf[1] = 0;
    buf[2] = MAX580X_CONFIG_AUX_NONE;

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    i2c_write(port,buf,3,i2c_more);
    i2c_start(port,DAC_ADDR(addr),i2c_mode_read);
    i2c_read(port,buf,3,i2c_stop);


    /* end the gate */
    buf[0] = MAX580X_CMD | MAX580X_CMD_END;
    buf[1] = 0;
    buf[2] = 0;

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    i2c_write(port,buf,3,i2c_more);
    i2c_start(port,DAC_ADDR(addr),i2c_mode_read);
    i2c_read(port,buf,3,i2c_stop);

    //console_message(0,"max580x: config (ext ref, midscale, output en");
    /* ensure reference is set to external */
    buf[0] = MAX580X_REF | MAX580X_REF_EXT;
    buf[1] = 0; 
    buf[2] = 0;

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    i2c_write(port,buf,3,i2c_more);
    i2c_start(port,DAC_ADDR(addr),i2c_mode_read);
    i2c_read(port,buf,3,i2c_stop);

    /* set to mid-scale */
    //buf[0] = MAX580X_CODELOAD;
    //buf[1] = 0x80; buf[2] = 0;
    //i2c_write(port,DAC_ADDR(addr),buf,3);

    /* output enable */
    buf[0] = MAX580X_POWER;
    buf[1] = 0; 
    buf[2] = MAX580X_POWER_NORMAL;

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    i2c_write(port,buf,3,i2c_more);
    i2c_start(port,DAC_ADDR(addr),i2c_mode_read);
    i2c_read(port,buf,3,i2c_stop);

    buf[0] = 0xf0; /* made up value for command */

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    i2c_write(port,buf,1,i2c_more);
    i2c_start(port,DAC_ADDR(addr),i2c_mode_read);
    i2c_read(port,buf,2,i2c_stop);

    //printf("max580x: status %02x %02x\r\n",buf[0],buf[1]);

    return 0;
}

/* set the DAC value to the given 16-bit value */
/* note: the chip will ignore bits it has no support for, so we always
 * accept and write the full 16-bit value */
int max580x_set(i2c_portname_t port, uint8_t addr, uint16_t value) {
    uint8_t buf[3];
    int ret;

    buf[0] = MAX580X_CODE;
    buf[1] = ((value & 0xff00) >> 8);
    buf[2] = (value & 0x00ff);

    i2c_start(port,DAC_ADDR(addr),i2c_mode_write);
    ret = i2c_write(port,buf,3,i2c_more);

    if (ret) {
        //console_message(0,"max580x: failed DAC update");
        return -EINVAL;
    }

    return 0;
}
