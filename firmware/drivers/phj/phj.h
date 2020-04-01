/*
 * brief PHJ driver definition.
 * 
 * Author: João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
 * 
 * Date: 30/03/2020
 * 
 * Issue #128
 *
 */

#ifndef PHJ_H_
#define PHJ_H_

#include <stdint.h>
#include <stdbool.h>
#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#define PHJ_SLAVE_ADDRESS           0x48

#define PHJ_FRAME_STATE_LEN         13

#define PHJ_FRAME_ID_STATE          0x20

typedef struct
{
    i2c_port_t port;
    uint32_t bitrate;
} phj_config_i2c;

typedef struct
{
    gpio_pin_t pin;
    uint8_t mode;
} phj_config_gpio;

int phj_init(phj_config_i2c config);

int phj_init_gpio(phj_config_gpio config);

int phj_read(uint8_t *data, uint16_t len);

int phj_check_converter();

int phj_check_message();

#endif // PHJ_H_
