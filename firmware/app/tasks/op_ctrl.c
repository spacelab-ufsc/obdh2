/*
 * op_ctrl.c
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Operation Control Task implementation
 * 
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2024/08/04
 * 
 * \addtogroup op_ctrl 
 * \{
 */

#include <stdint.h>
#include <stdbool.h>

#include <structs/satellite.h>
#include <system/sys_log/sys_log.h>
#include <FreeRTOS.h>
#include <task.h>

#include "op_ctrl.h"

#define POS_BRAZIL_LIM_N    (6L)
#define POS_BRAZIL_LIM_S    (-34L)
#define POS_BRAZIL_LIM_E    (-35L)
#define POS_BRAZIL_LIM_W    (-74L)

#define POS_LATITUDE        ((int16_t)sat_data_buf.obdh.data.position.latitude)
#define POS_LONGITUDE       ((int16_t)sat_data_buf.obdh.data.position.longitude)

static inline bool satellite_is_in_brazil(void);

static int activate_main_edc(void);
static int activate_px(void);

void vTaskOpCtrl(void)
{
    sys_time_t px_time_s = 0UL;

    sat_data_buf.state.current_edc = &sat_data_buf.edc_0;

    while (1) 
    {
        TickType_t last_cycle = xTaskGetTickCount();

        bool in_brazil = satellite_is_in_brazil();

        if (in_brazil && (sat_data_buf.obdh.data.mode != OBDH_MODE_NORMAL)) 
        {
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_OP_CTRL_NAME, "Changing mode to Nominal Mode...");
            sys_log_new_line();

            satellite_change_mode(OBDH_MODE_NORMAL);

            if (activate_main_edc() != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable main EDC");
                sys_log_new_line();
            }
        }

        if (!in_brazil && ((sat_data_buf.obdh.data.mode == OBDH_MODE_NORMAL) || (satellite_active_payload == PAYLOAD_X)))
        {
            if (px_time_s >= PAYLOAD_X_EXPERIMENT_PERIOD_S)
            {
                px_time_s = 0UL;

                if (payload_disable(PAYLOAD_X) != 0)
                {
                    sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to disable Payload X");
                    sys_log_new_line();
                }

                sat_data_buf.state.active_payload = PAYLOAD_NONE;
                satellite_change_mode(OBDH_MODE_STAND_BY);
            }
            else 
            {
                /* If Payload X was not active */
                if (px_time_s == 0UL)
                {
                    if (activate_px() != 0)
                    {
                        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_OP_CTRL_NAME, "Failed to enable Payload X");
                        sys_log_new_line();
                    }
                }
                else 
                {
                    ++px_time_s;
                }
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_OP_CTRL_PERIOD_MS));
    }
}

static int activate_main_edc(void)
{
    int err = -1;

    sat_data_buf.state.active_payload = sat_data_buf.state.main_edc;

    if (sat_data_buf.state.main_edc == PAYLOAD_EDC_0) 
    {
        if (payload_enable(PAYLOAD_EDC_0) == 0)
        {
            sat_data_buf.state.current_edc = &sat_data_buf.edc_0;
            err = 0;
        }
    }
    else 
    {
        if (payload_enable(PAYLOAD_EDC_1) == 0)
        {
            sat_data_buf.state.current_edc = &sat_data_buf.edc_1;
            err = 0;
        }

    }

    return err;
}

static int activate_px(void)
{
    int err = -1;

    sat_data_buf.state.active_payload = PAYLOAD_X;

    if (payload_enable(PAYLOAD_X) == 0)
    {
        err = 0;
    }

    return err;
}

static inline bool satellite_is_in_brazil(void)
{
    return ((POS_LATITUDE >= POS_BRAZIL_LIM_S) && (POS_LATITUDE <= POS_BRAZIL_LIM_N) && (POS_LONGITUDE >= POS_BRAZIL_LIM_W) && (POS_LONGITUDE <= POS_BRAZIL_LIM_E));
}

/** \} End of op_ctrl group */
