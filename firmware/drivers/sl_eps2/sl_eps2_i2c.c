/*
 * sl_eps2_i2c.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief SpaceLab EPS 2.0 driver I2C implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.12
 * 
 * \date 2021/10/13
 * 
 * \addtogroup sl_eps2
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include <drivers/tca4311a/tca4311a.h>

#include "sl_eps2.h"

#define SL_EPS2_I2C_OP_ATTEMPTS             10U

int sl_eps2_i2c_init(sl_eps2_config_t config)
{
    int err = -1;

    if (tca4311a_init(config, false) == TCA4311A_NOT_READY)
    {
        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error initializing the I2C port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int sl_eps2_i2c_write(sl_eps2_config_t config, uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t i = 0;
    for(i = 0; i < SL_EPS2_I2C_OP_ATTEMPTS; i++)
    {
        if (tca4311a_enable(config) == TCA4311A_READY)
        {
            sl_eps2_delay_ms(1);

            if (tca4311a_write(config, SL_EPS2_I2C_SLAVE_ADR, data, len) == TCA4311A_READY)
            {
                uint8_t j = 0;
                for(j = 0; j < SL_EPS2_I2C_OP_ATTEMPTS; j++)
                {
                    sl_eps2_delay_ms(1);

                    if (tca4311a_disable(config) == TCA4311A_NOT_READY)
                    {
                        err = 0;

                        break;
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error disabling the I2C bus during a writing operation!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }
                }

                break;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error writing to the I2C bus!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error enabling the I2C bus during an writing operation!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

int sl_eps2_i2c_read(sl_eps2_config_t config, uint8_t *data, uint16_t len)
{
    int err = -1;

    uint8_t i = 0;
    for(i = 0; i < SL_EPS2_I2C_OP_ATTEMPTS; i++)
    {
        if (tca4311a_enable(config) == TCA4311A_READY)
        {
            sl_eps2_delay_ms(1);

            if (tca4311a_read(config, SL_EPS2_I2C_SLAVE_ADR, data, len) == TCA4311A_READY)
            {
                uint8_t j = 0;
                for(j = 0; j < SL_EPS2_I2C_OP_ATTEMPTS; j++)
                {
                    sl_eps2_delay_ms(1);

                    if (tca4311a_disable(config) == TCA4311A_NOT_READY)
                    {
                        err = 0;

                        break;
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error disabling the I2C bus during a reading operation!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }
                }

                break;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error reading the I2C bus!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS2_MODULE_NAME, "Error enabling the I2C bus during an reading operation!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }

    return err;
}

/** \} End of sl_eps2 group */
