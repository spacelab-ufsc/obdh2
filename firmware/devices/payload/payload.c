/*
 * payload.c
 * 
 * Copyright The OBDH 2.0 Contributors.
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
 * \brief Payload device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
 * 
 * \version 0.9.1
 * 
 * \date 2021/08/15
 * 
 * \addtogroup payload
 * \{
 */

#include <system/system.h>
#include <system/sys_log/sys_log.h>

#include <drivers/gpio/gpio.h>
#include <drivers/edc/edc.h>
#include <drivers/phj/phj.h>

#include "payload.h"

#define PAYLOAD_UNIX_TO_J2000_EPOCH(x)      ((x) - 946684800)   /* Unix to J2000 epoch conversion */

static edc_config_t edc_0_conf = {0};
static edc_config_t edc_1_conf = {0};

int payload_init(payload_t pl)
{
    int err = -1;

    switch(pl)
    {
        case PAYLOAD_EDC_0:
        {
            edc_0_conf.port = I2C_PORT_0;
            edc_0_conf.bitrate = 400000UL;
            edc_0_conf.en_pin = GPIO_PIN_29;

            if (edc_init(edc_0_conf) == 0)
            {
                if (edc_set_rtc_time(edc_0_conf, PAYLOAD_UNIX_TO_J2000_EPOCH(system_get_time())) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error configuring the RTC time!");
                    sys_log_new_line();
                }

                edc_hk_t hk_data = {0};

                if (edc_get_hk(edc_0_conf, &hk_data) == 0)
                {
                    err = 0;

                    sys_log_print_event_from_module(SYS_LOG_INFO, PAYLOAD_MODULE_NAME, "EDC 0: Initialization done! (");
                    sys_log_print_uint(hk_data.temp);
                    sys_log_print_msg(" oC, ");
                    sys_log_print_uint(hk_data.voltage_supply);
                    sys_log_print_msg(" mV, ");
                    sys_log_print_uint(hk_data.current_supply);
                    sys_log_print_msg(" mA)");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error reading the housekeeping data!");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error during the initialization!");
                sys_log_new_line();
            }

            break;
        }
        case PAYLOAD_EDC_1:
        {
            edc_1_conf.port = I2C_PORT_0;
            edc_1_conf.bitrate = 400000UL;
            edc_1_conf.en_pin = GPIO_PIN_30;

            if (edc_init(edc_1_conf) == 0)
            {
                if (edc_set_rtc_time(edc_1_conf, PAYLOAD_UNIX_TO_J2000_EPOCH(system_get_time())) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error configuring the RTC time!");
                    sys_log_new_line();
                }

                edc_hk_t hk_data = {0};

                if (edc_get_hk(edc_1_conf, &hk_data) == 0)
                {
                    err = 0;

                    sys_log_print_event_from_module(SYS_LOG_INFO, PAYLOAD_MODULE_NAME, "EDC 1: Initialization done! (");
                    sys_log_print_uint(hk_data.temp);
                    sys_log_print_msg(" oC, ");
                    sys_log_print_uint(hk_data.voltage_supply);
                    sys_log_print_msg(" mV, ");
                    sys_log_print_uint(hk_data.current_supply);
                    sys_log_print_msg(" mA)");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error reading the housekeeping data!");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error during the initialization!");
                sys_log_new_line();
            }

            break;
        }
        case PAYLOAD_X:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Payload-X: init() not implemented yet");
            sys_log_new_line();

            break;
        case PAYLOAD_PHJ:
        {
            phj_config_i2c_t config_i2c;
            phj_config_gpio_t config_gpio;

            config_i2c.port = I2C_PORT_0;
            config_i2c.bitrate = 400000UL;

            if (phj_init_i2c(config_i2c) == 0)
            {
                config_gpio.pin = GPIO_PIN_0;
                config_gpio.mode = GPIO_MODE_INPUT;

                if (phj_init_gpio(config_gpio) == 0)
                {
                    err = 0;
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "PHJ: Error initializing the GPIO configuration!");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "PHJ: Error initializing the I2C configuration!");
                sys_log_new_line();
            }

            break;
        }
        case PAYLOAD_HARSH:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Harsh: init() not implemented yet!");
            sys_log_new_line();

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to initialize!");
            sys_log_new_line();

            break;
    }

    return err;
}

int payload_enable(payload_t pl)
{
    int err = -1;

    switch(pl)
    {
        case PAYLOAD_EDC_0:
            if (edc_enable(edc_0_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error enabling!");
                sys_log_new_line();
            }

            break;
        case PAYLOAD_EDC_1:
            if (edc_enable(edc_1_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error enabling!");
                sys_log_new_line();
            }

            break;
        case PAYLOAD_X:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Payload-X: enable() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_PHJ:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "PHJ: enable() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_HARSH:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Harsh: enable() routine not implemented yet!");
            sys_log_new_line();

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Invalid payload to enable!");
            sys_log_new_line();

            break;
    }

    return err;
}

int payload_disable(payload_t pl)
{
    int err = -1;

    switch(pl)
    {
        case PAYLOAD_EDC_0:
            if (edc_disable(edc_0_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error disabling!");
                sys_log_new_line();
            }

            break;
        case PAYLOAD_EDC_1:
            if (edc_disable(edc_1_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error disabling!");
                sys_log_new_line();
            }

            break;
        case PAYLOAD_X:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Payload-X: disable() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_PHJ:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "PHJ: disable() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_HARSH:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Harsh: disable() routine not implemented yet!");
            sys_log_new_line();

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to disable!");
            sys_log_new_line();

            break;
    }

    return err;
}

int payload_write_cmd(payload_t pl, payload_cmd_t cmd)
{
    int err = -1;

    switch(pl)
    {
        case PAYLOAD_EDC_0:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: ");
            sys_log_print_hex(cmd);
            sys_log_print_msg(" command received!");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: write_cmd() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_EDC_1:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: ");
            sys_log_print_hex(cmd);
            sys_log_print_msg(" command received!");
            sys_log_new_line();

            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: write_cmd() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_X:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Payload-X: write_cmd() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_PHJ:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "PHJ: write_cmd() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_HARSH:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Harsh: write_cmd() routine not implemented yet!");
            sys_log_new_line();

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to write command!");
            sys_log_new_line();

            break;
    }

    return err;
}

int payload_get_data(payload_t pl, payload_data_id_t id, uint8_t *data, uint32_t *len)
{
    int err = -1;

    switch(pl)
    {
        case PAYLOAD_EDC_0:
        {
            switch(id)
            {
                case PAYLOAD_EDC_RAW_STATE:
                {
                    int bytes = edc_get_state_pkg(edc_0_conf, data);

                    if (bytes < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error reading state!");
                        sys_log_new_line();
                    }

                    *len = bytes;

                    err = 0;

                    break;
                }
                case PAYLOAD_EDC_RAW_HK:
                {
                    int bytes = edc_get_hk_pkg(edc_0_conf, data);

                    if (bytes < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Error reading housekeeping data!");
                        sys_log_new_line();
                    }

                    *len = bytes;

                    err = 0;

                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 0: Invalid data ID!");
                    sys_log_new_line();

                    break;
            }

            break;
        }
        case PAYLOAD_EDC_1:
        {
            switch(id)
            {
                case PAYLOAD_EDC_RAW_STATE:
                {
                    int bytes = edc_get_state_pkg(edc_1_conf, data);

                    if (bytes < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error reading state!");
                        sys_log_new_line();
                    }

                    *len = bytes;

                    err = 0;

                    break;
                }
                case PAYLOAD_EDC_RAW_HK:
                {
                    int bytes = edc_get_hk_pkg(edc_1_conf, data);

                    if (bytes < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Error reading housekeeping data!");
                        sys_log_new_line();
                    }

                    *len = bytes;

                    err = 0;

                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC 1: Invalid data ID!");
                    sys_log_new_line();

                    break;
            }

            break;
        }
        case PAYLOAD_X:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Payload-X: get_data() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_PHJ:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "PHJ: get_data() routine not implemented yet!");
            sys_log_new_line();

            break;
        case PAYLOAD_HARSH:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Harsh: get_data() routine not implemented yet!");
            sys_log_new_line();

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to get data!");
            sys_log_new_line();

            break;
    }

    return err;
}

/** \} End of payload group */
