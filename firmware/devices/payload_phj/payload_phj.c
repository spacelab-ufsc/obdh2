/*
 * Brief PHJ device implementation.
 * 
 * Author: João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
 * 
 * Date: 30/03/2020
 * 
 * Issue #129
 *
 */

#include <devices/logger/logger.h>
#include <drivers/phj/phj.h>

#include "payload_phj.h"

int payload_phj_init()
{
    phj_config_i2c config_i2c;
    phj_config_gpio config_gpio;

    config_i2c.port = I2C_PORT_0;
    config_i2c.bitrate = 400000;

    if (phj_init_i2c(config_i2c) != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, PAYLOAD_PHJ_MODULE_NAME, "Error initializing the i2c configuration!");
        logger_new_line();

        return -1;
    }

    config_gpio.pin = GPIO_PIN_0;
    config_gpio.mode = GPIO_MODE_INPUT;


    if (phj_init_gpio(config_gpio) != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, PAYLOAD_PHJ_MODULE_NAME, "Error initializing the gpio configuration!");
        logger_new_line();

        return -1;
    }

    return 0;
}
