/*
 * sl_eps.c
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
 * \brief SpaceLab EPS driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.9
 * 
 * \date 2020/07/18
 * 
 * \addtogroup sl_eps
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "sl_eps.h"

int sl_eps_init(i2c_port_t port, uint32_t bitrate)
{
    static sl_eps_i2c_config_t i2c_conf = {0};

    i2c_conf.port = port;
    i2c_conf.bitrate = bitrate;
#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
    sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS_MODULE_NAME, "\"sl_eps_init\" not implemented!");
    sys_log_new_line();
#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    return -1;
}

int sl_eps_read(sl_eps_data_t *data)
{
    data->voltage = 0;
#if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
    sys_log_print_event_from_module(SYS_LOG_ERROR, SL_EPS_MODULE_NAME, "\"sl_eps_read\" not implemented!");
    sys_log_new_line();
#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    return -1;
}

/** \} End of sl_eps group */
