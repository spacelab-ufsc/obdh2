/*
 * payload_edc.c
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief Payload EDC device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.2.22
 * 
 * \date 18/02/2020
 * 
 * \addtogroup payload_edc
 * \{
 */

#include <devices/logger/logger.h>
#include <drivers/edc/edc.h>

#include "payload_edc.h"

int payload_edc_init()
{
    edc_config_t config;

    config.port = I2C_PORT_0;
    config.bitrate = 400000;

    if (edc_init(config) != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error initializing the device!");
        logger_new_line();

        return -1;
    }

    if (edc_pause_ptt_task() != 0)
    {
        logger_print_event_from_module(LOGGER_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error pausing the PTT task!");
        logger_new_line();

        return -1;
    }

    uint8_t edc_hk[EDC_FRAME_HK_LEN];

    if (edc_get_hk_pkg(edc_hk) != EDC_FRAME_HK_LEN)
    {
        logger_print_event_from_module(LOGGER_ERROR, PAYLOAD_EDC_MODULE_NAME, "Error reading housekeeping data!");
        logger_new_line();

        return -1;
    }

    logger_print_event_from_module(LOGGER_INFO, PAYLOAD_EDC_MODULE_NAME, "Initialization done! (");
    logger_print_dec((int8_t)edc_hk[13]-40);
    logger_print_msg(" oC, ");
    logger_print_dec(((uint16_t)edc_hk[12] << 8) | edc_hk[11]);
    logger_print_msg(" mV, ");
    logger_print_dec(((uint16_t)edc_hk[10] << 8) | edc_hk[9]);
    logger_print_msg(" mA)");
    logger_new_line();

    return 0;
}

int payload_edc_get_state()
{
    return -1;
}

int payload_edc_enable()
{
    return -1;
}

int payload_edc_disable()
{
    return -1;
}

/** \} End of leds group */
