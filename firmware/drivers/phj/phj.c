/*
 * Brief PHJ driver implementation.
 * 
 * Author: João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
 * 
 * Date: 30/03/2020
 * 
 * Issue #128
 *
 */

#include <drivers/phj/phj.h>
#include <math.h>
#include <string.h>


i2c_port_t phj_i2c_port;
gpio_pin_t phj_gpio_pin;

// I2C configuration
int phj_init_i2c(phj_config_i2c config)
{
    phj_i2c_port = config.port;

    if (i2c_init(config.port, (i2c_config_t){.speed_hz=config.bitrate}) != 0)
    {
        return -1;  // Error initializing the I2C configuration
    }

    return 0;
}

//GPIO configuration
int phj_init_gpio(phj_config_gpio config)
{
    phj_gpio_pin = config.pin;

    if (gpio_init(config.pin, (gpio_config_t){.mode=config.mode}) != 0)
    {
        return -1;  // Error initializing the GPIO configuration
    }

    return 0;
}

//Read data from Payload PHJ
int phj_read(uint8_t *data, uint16_t len)
{
    return i2c_read(phj_i2c_port, PHJ_SLAVE_ADDRESS, data, len);
}

//Check the converter's health
int phj_check_converter()
{
    return gpio_get_state(phj_gpio_pin);
}

//Check for comunication errors
int phj_check_message()
{
    uint8_t status[PHJ_FRAME_STATE_LEN];

    if (phj_read(status, PHJ_FRAME_STATE_LEN) != 0)
    {
        return -1;
    }

    if (status[0] != PHJ_FRAME_ID_STATE)
    {
        return -2;
    }

    return 0;
}
