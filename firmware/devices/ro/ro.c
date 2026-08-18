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
 * \author Renato Augusto Schenkel Meneghin Marchiori
 * 
 * \version 0.0.1
 * 
 * \date 2026/08/15
 * 
 * \addtogroup ro
 * \{
 */

#include <config/config.h>
#include <system/system.h>
#include <system/sys_log/sys_log.h>

#include <drivers/gpio/gpio.h>
#include <drivers/ro/ro.h>

#include "ro.h"

#define RO_UNIX_TO_J2000_EPOCH(x)      ((x) - 946684800UL)   /* Unix to J2000 epoch conversion */

static ro_config_t ro_conf;

int ro_payload_init(ro_type_t ro)
{
    int err = -1;

    switch(ro)
    {
        case RO_0:
        {
            ro_conf.interface = RO_IF_I2C;
            ro_conf.uart_port = I2C_PORT_1;
            ro_conf.en_pin = GPIO_PIN_31; // Modificar

            if (ro_init(ro_conf) == 0)
            {
                if (ro_set_rtc_time(ro_conf, RO_UNIX_TO_J2000_EPOCH(system_get_time())) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error configuring the RTC time!");
                    sys_log_new_line();
                }

                ro_hk_t hk_data = {0};

                if (ro_get_hk(ro_conf, &hk_data) == 0)
                {
                    err = 0;

                    sys_log_print_event_from_module(SYS_LOG_INFO, RO_MODULE_NAME, "RO: Initialization done! (");
                    sys_log_print_int(hk_data.fpga_temperature);
                    sys_log_print_msg(" oC, ");
                    sys_log_print_uint(hk_data.voltage);
                    sys_log_print_msg(" mV, ");
                    sys_log_print_uint(hk_data.current);
                    sys_log_print_msg(" mA)");
                    sys_log_new_line();
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading the housekeeping data!");
                    sys_log_new_line();
                }

                if (ro_start_tracking(ro_conf) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error starting tracking!");
                    sys_log_new_line();
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error during the initialization!");
                sys_log_new_line();
            }

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid payload to initialize!");
            sys_log_new_line();

            break;
    }

    return err;
}

int ro_payload_enable(ro_type_t ro)
{
    int err = -1;

    switch(ro)
    {
        case RO_0:
            if (ro_enable(ro_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error enabling!");
                sys_log_new_line();
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid payload to enable!");
            sys_log_new_line();

            break;
    }

    return err;
}

int ro_payload_disable(ro_type_t ro)
{
    int err = -1;

    switch(ro)
    {
        case RO_0:
            if (ro_disable(ro_conf) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error disabling!");
                sys_log_new_line();
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid payload to disable!");
            sys_log_new_line();

            break;
    }

    return err;
}

int ro_payload_write_cmd(ro_type_t ro, ro_cmd_t cmd)
{
    int err = -1;

    switch(ro)
    {
        case RO_0:
            if (ro_write_cmd(ro_conf, cmd) == 0)
            {
                err = 0;
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error writing command!");
                sys_log_new_line();
            }

            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid payload to write command!");
            sys_log_new_line();

            break;
    }
}

int ro_get_data(ro_type_t ro, ro_data_id_t id, uint8_t *data, int32_t *len)
{
    int err = -1;

    switch(ro)
    {
        case RO_0:
        {
            switch(id)
            {
                case PAYLOAD_RO_RAW_STATE:
                {
                    *len = ro_get_state_pkg(ro_conf, data);

                    if (*len < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading state!");
                        sys_log_new_line();
                    }
                    else
                    {
                        err = 0;
                    }

                    break;
                }
                case PAYLOAD_RO_STATE:
                {
                    ro_state_t state = {0};

                    if (ro_get_state(ro_conf, &state) == 0)
                    {
                        uint8_t i = 0;
                        for(i = 0; i < sizeof(ro_state_t); i++)
                        {
                            data[i] = ((uint8_t*)&state)[i];
                        }

                        *len = sizeof(ro_state_t);

                        err = 0;
                    }
                    else
                    {
                        *len = 0U;

                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading state!");
                        sys_log_new_line();
                    }

                    break;
                }
                case PAYLOAD_RO_RAW_EVENT:
                {
                    *len = ro_get_event_pkg(ro_conf, data);

                    if (*len < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading event!");
                        sys_log_new_line();
                    }
                    else
                    {
                        err = 0;
                    }

                    break;
                }
                case PAYLOAD_RO_EVENT:
                {
                    ro_event_t event = {0};

                    if (ro_get_event(ro_conf, &event) == 0)
                    {
                        uint8_t i = 0;
                        for(i = 0; i < sizeof(ro_event_t); i++)
                        {
                            data[i] = ((uint8_t*)&event)[i];
                        }

                        *len = sizeof(ro_event_t);

                        err = 0;
                    }
                    else
                    {
                        *len = 0U;

                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading event!");
                        sys_log_new_line();
                    }

                    break;
                }
                case PAYLOAD_RO_RAW_NAVIGATION:
                {
                    *len = ro_get_navigation_pkg(ro_conf, data);

                    if (*len < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading navigation data!");
                        sys_log_new_line();
                    }
                    else
                    {
                        err = 0;
                    }

                    break;
                }
                case PAYLOAD_RO_NAVIGATION:
                {
                    ro_navigation_t navigation = {0};

                    if (ro_get_navigation(ro_conf, &navigation) == 0)
                    {
                        uint8_t i = 0;
                        for(i = 0; i < sizeof(ro_navigation_t); i++)
                        {
                            data[i] = ((uint8_t*)&navigation)[i];
                        }

                        *len = sizeof(ro_navigation_t);

                        err = 0;
                    }
                    else
                    {
                        *len = 0U;

                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading navigation data!");
                        sys_log_new_line();
                    }

                    break;
                }
                case PAYLOAD_RO_RAW_OBSERVATION:
                {
                    *len = ro_get_observation_pkg(ro_conf, data);

                    if (*len < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading observation data!");
                        sys_log_new_line();
                    }
                    else
                    {
                        err = 0;
                    }

                    break;
                }
                case PAYLOAD_RO_OBSERVATION:
                {
                    ro_observation_t observation = {0};

                    if (ro_get_observation(ro_conf, &observation) == 0)
                    {
                        uint8_t i = 0;
                        for(i = 0; i < sizeof(ro_observation_t); i++)
                        {
                            data[i] = ((uint8_t*)&observation)[i];
                        }

                        *len = sizeof(ro_observation_t);

                        err = 0;
                    }
                    else
                    {
                        *len = 0U;

                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading observation data!");
                        sys_log_new_line();
                    }

                    break;
                }
                case PAYLOAD_RO_RAW_IQ_HEADER: // Verificar implementação com o Lucas K.
                {
                    *len = ro_get_iq_pkg(ro_conf, data);

                    if (*len < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading IQ data!");
                        sys_log_new_line();
                    }
                    else
                    {
                        err = 0;
                    }

                    break;
                }
                case PAYLOAD_RO_IQ_HEADER: // Verificar implementação com o Lucas K.
                {
                    ro_iq_header_t observation = {0};

                    if (ro_get_iq(ro_conf, &observation) == 0)
                    {
                        uint8_t i = 0;
                        for(i = 0; i < sizeof(ro_iq_header_t); i++)
                        {
                            data[i] = ((uint8_t*)&observation)[i];
                        }

                        *len = sizeof(ro_iq_header_t);

                        err = 0;
                    }
                    else
                    {
                        *len = 0U;

                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading IQ header data!");
                        sys_log_new_line();
                    }

                    break;
                }
                case PAYLOAD_RO_RAW_HK:
                {
                    *len = ro_get_iq_pkg(ro_conf, data);

                    if (*len < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading IQ data!");
                        sys_log_new_line();
                    }
                    else
                    {
                        err = 0;
                    }

                    break;
                }
                case PAYLOAD_RO_HK:
                {
                    ro_observation_t observation = {0};

                    if (ro_get_observation(ro_conf, &observation) == 0)
                    {
                        uint8_t i = 0;
                        for(i = 0; i < sizeof(ro_observation_t); i++)
                        {
                            data[i] = ((uint8_t*)&observation)[i];
                        }

                        *len = sizeof(ro_observation_t);

                        err = 0;
                    }
                    else
                    {
                        *len = 0U;

                        sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Error reading observation data!");
                        sys_log_new_line();
                    }

                    break;
                }
                default:
                    sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid data ID!");
                    sys_log_new_line();

                    break;
            }

            break;
        }
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid payload to get data!");
            sys_log_new_line();

            break;
    }

    return err;
}

int ro_set_clock(const ro_type_t ro, const uint32_t time)
{
    int err = 0;

    switch(ro)
    {
        case RO_0:
            sys_log_print_event_from_module(SYS_LOG_INFO, RO_MODULE_NAME, "RO 0: Setting Clock/RTC time...");
            sys_log_new_line();

            if (ro_set_rtc_time(ro_conf, RO_UNIX_TO_J2000_EPOCH(time)) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO 0: Error setting the Clock/RTC time!");
                sys_log_new_line();
                err = -1;
            }
            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, RO_MODULE_NAME, "RO: Invalid payload to set clock!");
            sys_log_new_line();
            err = -1;
            break;
    }

    return err;
}

/** \} End of payload group */
