/*
 * isis_antenna_i2c.c
 * 
 * Copyright (C) 2021, SpaceLab.
 * 
 * This file is part of OBDH 2.0.
 * 
 * OBDH 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OBDH 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver I2C routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.8
 * 
 * \date 2021/10/10
 * 
 * \addtogroup isis_antenna
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/tca4311a/tca4311a.h>

#include "isis_antenna.h"

/* I2C port configuration */
#define ISIS_ANTENNA_I2C_PORT               I2C_PORT_2
#define ISIS_ANTENNA_I2C_CLOCK_HZ           100000UL
#define ISIS_ANTENNA_I2C_EN_PIN             GPIO_PIN_18
#define ISIS_ANTENNA_I2C_RDY_PIN            GPIO_PIN_21
#define ISIS_ANTENNA_I2C_SLAVE_ADDRESS      ISIS_ANTENNA_I2C_SLAVE_ADDRESS_UC_A

#define ISIS_ANTENNA_I2C_OP_ATTEMPTS        10U

static tca4311a_config_t ants_config = {0};

int isis_antenna_i2c_init(void)
{
    int err = -1;

    ants_config.i2c_port            = ISIS_ANTENNA_I2C_PORT;
    ants_config.i2c_config.speed_hz = ISIS_ANTENNA_I2C_CLOCK_HZ;
    ants_config.en_pin              = ISIS_ANTENNA_I2C_EN_PIN;
    ants_config.ready_pin           = ISIS_ANTENNA_I2C_RDY_PIN;

    if (tca4311a_init(ants_config, false) == TCA4311A_READY)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error initializing the I2C port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_i2c_write(uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t i = 0;
    for(i = 0; i < ISIS_ANTENNA_I2C_OP_ATTEMPTS; i++)
    {
        if (tca4311a_enable(ants_config) == TCA4311A_READY)
        {
            isis_antenna_delay_ms(1);

            if (tca4311a_write(ants_config, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, data, len) == TCA4311A_READY)
            {
                uint8_t j = 0;
                for(j = 0; j < ISIS_ANTENNA_I2C_OP_ATTEMPTS; j++)
                {
                    isis_antenna_delay_ms(1);

                    if (tca4311a_disable(ants_config) == TCA4311A_NOT_READY)
                    {
                        err = 0;

                        break;
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error disabling the I2C bus during a writing operation!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }
                }

                break;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing to the I2C bus!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error enabling the I2C bus during an writing operation!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

int isis_antenna_i2c_read(uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t i = 0;
    for(i = 0; i < ISIS_ANTENNA_I2C_OP_ATTEMPTS; i++)
    {
        if (tca4311a_enable(ants_config) == TCA4311A_READY)
        {
            isis_antenna_delay_ms(1);

            if (tca4311a_read(ants_config, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, data, len) == TCA4311A_READY)
            {
                uint8_t j = 0;
                for(j = 0; j < ISIS_ANTENNA_I2C_OP_ATTEMPTS; j++)
                {
                    isis_antenna_delay_ms(1);

                    if (tca4311a_disable(ants_config) == TCA4311A_NOT_READY)
                    {
                        err = 0;

                        break;
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error disabling the I2C bus during a reading operation!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }
                }

                break;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the I2C bus!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error enabling the I2C bus during an reading operation!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

/** \} End of isis_antenna group */
