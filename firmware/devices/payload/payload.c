/*
 * payload.c
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Payload device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.4
 * 
 * \date 2021/08/15
 * 
 * \addtogroup payload
 * \{
 */

#include <system/sys_log/sys_log.h>

#include <drivers/edc/edc.h>

#include "payload.h"

int payload_init(payload_t pl)
{
    switch(pl)
    {
        case PAYLOAD_EDC:
        {
            edc_config_t config = {0};

            config.port = I2C_PORT_0;
            config.bitrate = 400000;

            if (edc_init(config) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Error during the initialization!");
                sys_log_new_line();

                return -1;
            }

            if (payload_disable(PAYLOAD_EDC) != 0)
            {
                return -1;
            }

            edc_hk_t hk_data = {0};

            if (edc_get_hk(&hk_data) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Error pausing the PTT task!");
                sys_log_new_line();

                return -1;
            }

            sys_log_print_event_from_module(SYS_LOG_INFO, PAYLOAD_MODULE_NAME, "EDC: Initialization done! (");
            sys_log_print_uint(hk_data.temp);
            sys_log_print_msg(" oC, ");
            sys_log_print_uint(hk_data.voltage_supply);
            sys_log_print_msg(" mV, ");
            sys_log_print_uint(hk_data.current_supply);
            sys_log_print_msg(" mA)");
            sys_log_new_line();

            return 0;
        }
        case PAYLOAD_X:
            break;
        case PAYLOAD_PHJ:
            break;
        case PAYLOAD_HARSH:
            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to initialize!");
            sys_log_new_line();

            return -1;
    }
}

int payload_enable(payload_t pl)
{
    switch(pl)
    {
        case PAYLOAD_EDC:
            if (edc_resume_ptt_task() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Error enabling!");
                sys_log_new_line();

                return -1;
            }

            return 0;
        case PAYLOAD_X:
            break;
        case PAYLOAD_PHJ:
            break;
        case PAYLOAD_HARSH:
            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Invalid payload to enable!");
            sys_log_new_line();

            return -1;
    }
}

int payload_disable(payload_t pl)
{
    switch(pl)
    {
        case PAYLOAD_EDC:
            if (edc_pause_ptt_task() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Error disabling!");
                sys_log_new_line();

                return -1;
            }

            return 0;
        case PAYLOAD_X:
            break;
        case PAYLOAD_PHJ:
            break;
        case PAYLOAD_HARSH:
            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to disable!");
            sys_log_new_line();

            return -1;
    }
}

int payload_write_cmd(payload_t pl, payload_cmd_t cmd)
{
    switch(pl)
    {
        case PAYLOAD_EDC:
            break;
        case PAYLOAD_X:
            break;
        case PAYLOAD_PHJ:
            break;
        case PAYLOAD_HARSH:
            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to write command!");
            sys_log_new_line();

            return -1;
    }
}

int payload_get_data(payload_t pl, payload_data_id_t id, uint8_t *data, uint32_t *len)
{
    switch(pl)
    {
        case PAYLOAD_EDC:
        {
            switch(id)
            {
                case PAYLOAD_EDC_RAW_STATE:
                {
                    int bytes = edc_get_state_pkg(data);

                    if (bytes < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Error reading state!");
                        sys_log_new_line();
                    }

                    *len = bytes;

                    return 0;
                }
                case PAYLOAD_EDC_RAW_HK:
                {
                    int bytes = edc_get_hk_pkg(data);

                    if (bytes < 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "EDC: Error reading housekeeping data!");
                        sys_log_new_line();
                    }

                    *len = bytes;

                    return 0;
                }
                default:
            }
        }
        case PAYLOAD_X:
            break;
        case PAYLOAD_PHJ:
            break;
        case PAYLOAD_HARSH:
            break;
        default:
            sys_log_print_event_from_module(SYS_LOG_ERROR, PAYLOAD_MODULE_NAME, "Invalid payload to get data!");
            sys_log_new_line();

            return -1;
    }
}

/** \} End of payload group */
