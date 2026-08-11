/*
 * mission_manager.c
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
 * \brief Mission Manager Task implementation
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 1.0.0
 *
 * \date 2024/08/04
 *
 * \addtogroup mission_man
 * \{
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <structs/satellite.h>
#include <structs/obdh_data.h>
#include <devices/payload/payload.h>
#include <devices/ttc/ttc.h>
#include <devices/eps/eps.h>
#include <drivers/sl_eps2/sl_eps2.h>
#include <conops/conops.h>
#include <system/sys_log/sys_log.h>
#include <system/system.h>
#include <config/config.h>

#include "mission_manager.h"
#include "read_px.h"
#include "system_reset.h"
#include "process_tc.h"
#include "startup.h"
#include "pos_det.h"

static inline int32_t handle_deployment_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    (void)ctx;

    int32_t transition_to = OBDH_MODE_DEPLOYMENT;

    switch (ev->ev_id)
    {
        case EV_OUT_OF_BRAZIL:
            break;
        case EV_IN_BRAZIL:
            break;
        case EV_TC_GOTO_NORMAL_MODE:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            transition_to = OBDH_MODE_EXPERIMENT;
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        case EV_TC_GOTO_COMMISSION_MODE:
            transition_to = OBDH_MODE_COMMISSION;
            break;
        case EV_DEPLOYMENT_COMPLETE:
            transition_to = OBDH_MODE_COMMISSION;
            break;
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static inline int32_t handle_commission_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    sat_data_t *sat = ctx->user_data;
    int transition_to = OBDH_MODE_COMMISSION;

    switch (ev->ev_id)
    {
        /* In commission there's simple no reason to react to position related
         * events since the TLEs are still not updated */
        case EV_OUT_OF_BRAZIL:
            break;
        case EV_IN_BRAZIL:
            break;
        case EV_TC_GOTO_NORMAL_MODE:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            transition_to = OBDH_MODE_EXPERIMENT;
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        case EV_TC_ENABLE_PAYLOAD:
        {
            payload_t pl = (payload_t)ev->src;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Enabling Payload (");
            sys_log_print_hex((uint32_t)ev->src);
            sys_log_print_msg(") manually...");
            sys_log_new_line();

            if (payload_enable(pl) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable Payload!");
                sys_log_new_line();
            }

            break;
        }
        case EV_TC_DISABLE_PAYLOAD:
        {
            payload_t pl = (payload_t)ev->src;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Disabling Payload (");
            sys_log_print_hex((uint32_t)ev->src);
            sys_log_print_msg(") manually...");
            sys_log_new_line();

            if (payload_disable(pl) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Payload!");
                sys_log_new_line();
            }

            break;
        }
        case EV_COMISSION_TIMEOUT:
            if (is_satellite_in_brazil((float)sat->obdh.data.position.latitude, (float)sat->obdh.data.position.longitude))
            {
                transition_to = OBDH_MODE_NORMAL;
            }
            else
            {
                transition_to = OBDH_MODE_STAND_BY;
            }

            break;
        case EV_BATTERY_LEVEL_CRITICAL:
            (void)payload_disable(PAYLOAD_X);
            (void)payload_disable(PAYLOAD_EDC_0);
            (void)payload_disable(PAYLOAD_EDC_1);
            break;
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static inline int32_t handle_nominal_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    sat_data_t *sat = ctx->user_data;
    int32_t transition_to = OBDH_MODE_NORMAL;

    switch (ev->ev_id)
    {
        case EV_OUT_OF_BRAZIL:
            if (sat->obdh.data.manual_experiments)
            {
                transition_to = OBDH_MODE_STAND_BY;
            }
            else
            {
                transition_to = OBDH_MODE_EXPERIMENT;
            }
            break;
        case EV_IN_BRAZIL:
            break;
        case EV_TC_GOTO_NORMAL_MODE:
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            transition_to = OBDH_MODE_EXPERIMENT;
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_BATTERY_LEVEL_CRITICAL:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static inline int32_t handle_standby_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    (void)ctx;
    int32_t transition_to = OBDH_MODE_STAND_BY;

    switch (ev->ev_id)
    {
        case EV_OUT_OF_BRAZIL:
            break;
        case EV_IN_BRAZIL:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_NORMAL_MODE:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            transition_to = OBDH_MODE_EXPERIMENT;
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_BATTERY_LEVEL_CRITICAL:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static inline int32_t handle_experiment_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    (void)ctx;
    int32_t transition_to = OBDH_MODE_EXPERIMENT;

    switch (ev->ev_id)
    {
        case EV_OUT_OF_BRAZIL:
            break;
        case EV_IN_BRAZIL:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_NORMAL_MODE:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_BATTERY_LEVEL_CRITICAL:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        case EV_PX_FINISHED:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static inline int32_t handle_fdir_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    sat_data_t *sat = ctx->user_data;
    int32_t transition_to = OBDH_MODE_FDIR;

    switch (ev->ev_id)
    {
        case EV_OUT_OF_BRAZIL:
            break;
        case EV_IN_BRAZIL:
            break;
        case EV_TC_GOTO_NORMAL_MODE:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            transition_to = OBDH_MODE_EXPERIMENT;
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_BATTERY_LEVEL_CRITICAL:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        case EV_FDIR_RESOLVED:
            if (is_satellite_in_brazil((float)sat->obdh.data.position.latitude, (float)sat->obdh.data.position.longitude))
            {
                transition_to = OBDH_MODE_NORMAL;
            }
            else
            {
                transition_to = OBDH_MODE_STAND_BY;
            }
            break;
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static inline int32_t handle_manual_evs(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    (void)ctx;
    int transition_to = OBDH_MODE_MANUAL;

    switch (ev->ev_id)
    {
        case EV_TC_GOTO_NORMAL_MODE:
            transition_to = OBDH_MODE_NORMAL;
            break;
        case EV_TC_GOTO_FDIR_MODE:
            transition_to = OBDH_MODE_FDIR;
            break;
        case EV_TC_GOTO_EXPERIMENT_MODE:
            transition_to = OBDH_MODE_EXPERIMENT;
            break;
        case EV_TC_GOTO_STAND_BY_MODE:
            transition_to = OBDH_MODE_STAND_BY;
            break;
        case EV_TC_GOTO_MANUAL_MODE:
            transition_to = OBDH_MODE_MANUAL;
            break;
        case EV_TC_ENABLE_PAYLOAD:
        {
            payload_t pl = (payload_t)ev->src;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Enabling Payload (");
            sys_log_print_hex((uint32_t)ev->src);
            sys_log_print_msg(") manually...");
            sys_log_new_line();

            if (payload_enable(pl) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable Payload!");
                sys_log_new_line();
            }

            break;
        }
        case EV_TC_DISABLE_PAYLOAD:
        {
            payload_t pl = (payload_t)ev->src;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Disabling Payload (");
            sys_log_print_hex((uint32_t)ev->src);
            sys_log_print_msg(") manually...");
            sys_log_new_line();

            if (payload_disable(pl) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Payload!");
                sys_log_new_line();
            }

            break;
        }
        default:
            transition_to = -1;
            break;
    }

    return transition_to;
}

static int enable_ttc_tx(void)
{
    int err = 0;
    uint8_t retry_count = 5U;

    do
    {
        err = 0;

        if (ttc_leave_hibernation(TTC_0) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable TTC 0 TX");
            sys_log_new_line();
            err--;
        }

        if (ttc_leave_hibernation(TTC_1) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable TTC 1 TX");
            sys_log_new_line();
            err--;
        }

        --retry_count;
    } while ((err < 0) && (retry_count > 0U));

    if (retry_count == 0U)
    {
        err = -1;
    }

    return err;
}

static int disable_ttc_tx(void)
{
    int err = 0;
    uint8_t retry_count = 5U;

    do
    {
        err = 0;

        if (ttc_enter_hibernation(TTC_0) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable TTC 0 TX");
            sys_log_new_line();
            err--;
        }

        if (ttc_enter_hibernation(TTC_1) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable TTC 1 TX");
            sys_log_new_line();
            err--;
        }

        --retry_count;
    } while ((err < 0) && (retry_count > 0U));

    if (retry_count == 0U)
    {
        err = -1;
    }

    return err;
}

#if defined (CONFIG_MISSION_GOLDS_UFSC) && (CONFIG_MISSION_GOLDS_UFSC == 1)

static int goto_commission_mode(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to)
{
    (void)transition_to;
    sat_data_t *sat = ctx->user_data;
    int retval = 0;
    int err = 0;
    uint8_t retry_count = 5U;

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Transitioning to Commission Mode because of event (");
    sys_log_print_hex((uint32_t)ev->ev_id);
    sys_log_print_msg(")...");
    sys_log_new_line();

    sat->obdh.data.hibernation_on = false;
    sat->obdh.data.general_telemetry_on = true;
    sat->obdh.data.eps_beacon_on = true;
    sat->obdh.data.ts_commission_timeout = system_get_time() + (uint32_t)COMMISSION_MODE_TIMEOUT_SEC;
    sat->obdh.data.hib_duration = 0U;

    if (enable_ttc_tx() < 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable TTC transmission!");
        sys_log_new_line();
    }

    do
    {
        err = eps_set_param(SL_EPS2_REG_BEACON_ENABLE, 1U);
        vTaskDelay(100U);
        --retry_count;
    } while ((err < 0) && (retry_count > 0U));

    if (retry_count == 0U)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable EPS beacon!");
        sys_log_new_line();
    }

    if ((ev->ev_id != EV_PERSIST_STATE_ON_INIT) && (retval == 0))
    {
        satellite_change_mode(OBDH_MODE_COMMISSION);
        sat->obdh.data.last_tran_ev_id = ev->ev_id;
        ctx->state = OBDH_MODE_COMMISSION;
    }

    return retval;
}

static int goto_standby_mode(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to)
{
    (void)transition_to;
    sat_data_t *sat = ctx->user_data;
    int retval = 0;
    int err = 0;
    uint8_t retry_count = 5U;

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Transitioning to Stand-by Mode because of event (");
    sys_log_print_hex((uint32_t)ev->ev_id);
    sys_log_print_msg(")...");
    sys_log_new_line();

    if (sat->obdh.data.main_payload_state != (uint8_t)PAYLOAD_NONE)
    {
        do
        {
            err = payload_disable((payload_t)sat->obdh.data.main_payload_state);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.main_payload_state = (uint8_t)PAYLOAD_NONE;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Main payload!");
            sys_log_new_line();
            retval = -1;
        }

        err = 0;
        retry_count = 5U;
    }

    if (sat->obdh.data.sec_payload_state != (uint8_t)PAYLOAD_NONE)
    {
        do
        {
            err = payload_disable((payload_t)sat->obdh.data.sec_payload_state);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.sec_payload_state = (uint8_t)PAYLOAD_NONE;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Secondary payload!");
            sys_log_new_line();
            retval = -1;
        }
    }

    if ((ev->ev_id != EV_PERSIST_STATE_ON_INIT) && (retval == 0))
    {
        satellite_change_mode(OBDH_MODE_STAND_BY);
        sat->obdh.data.last_tran_ev_id = ev->ev_id;
        ctx->state = OBDH_MODE_STAND_BY;
    }

    return retval;
}

static int goto_nominal_mode(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to)
{
    (void)transition_to;
    sat_data_t *sat = ctx->user_data;
    int retval = 0;
    int err = 0;
    uint8_t retry_count = 5U;

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Transitioning to Nominal Mode because of event (");
    sys_log_print_hex((uint32_t)ev->ev_id);
    sys_log_print_msg(")...");
    sys_log_new_line();

    if (sat->obdh.data.sec_payload_state != (uint8_t)PAYLOAD_NONE)
    {
        do
        {
            err = payload_disable((payload_t)sat->obdh.data.sec_payload_state);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.sec_payload_state = (uint8_t)PAYLOAD_NONE;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Secondary payload!");
            sys_log_new_line();
            retval = -1;
        }

        err = 0;
        retry_count = 5U;
    }

    payload_t main = (payload_t)sat->obdh.data.main_edc;

    if (main != PAYLOAD_NONE)
    {
        if ((sat->obdh.data.main_payload_state != (uint8_t)main) && (sat->obdh.data.main_payload_state != (uint8_t)PAYLOAD_NONE))
        {
            (void)payload_disable((payload_t)sat_data_buf.obdh.data.main_payload_state);
        }

        do
        {
            err = payload_enable(main);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.main_payload_state = (uint8_t)main;
            sat->state.c_edc = (main == PAYLOAD_EDC_0) ? &sat_data_buf.edc_0 : &sat_data_buf.edc_1;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable Main payload!");
            sys_log_new_line();
            retval = -1;
        }
    }

    if ((ev->ev_id != EV_PERSIST_STATE_ON_INIT) && (retval == 0))
    {
        satellite_change_mode(OBDH_MODE_NORMAL);
        sat->obdh.data.last_tran_ev_id = ev->ev_id;
        ctx->state = OBDH_MODE_NORMAL;
    }

    return retval;
}

static int goto_experiment_mode(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to)
{
    (void)transition_to;
    sat_data_t *sat = ctx->user_data;
    int retval = 0;
    int err = 0;
    uint8_t retry_count = 5U;

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Transitioning to Experiment Mode because of event (");
    sys_log_print_hex((uint32_t)ev->ev_id);
    sys_log_print_msg(")...");
    sys_log_new_line();

    if (sat->obdh.data.main_payload_state != (uint8_t)PAYLOAD_NONE)
    {
        do
        {
            err = payload_disable((payload_t)sat->obdh.data.main_payload_state);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.main_payload_state = (uint8_t)PAYLOAD_NONE;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Main payload!");
            sys_log_new_line();
            retval = -1;
        }

        err = 0;
        retry_count = 5U;
    }

    do
    {
        err = payload_enable(PAYLOAD_X);
        vTaskDelay(100U);
        --retry_count;
    } while ((err < 0) && (retry_count > 0U));

    if (retry_count != 0U)
    {
        const uint32_t px_active_time_ms = (uint32_t)PAYLOAD_X_EXPERIMENT_PERIOD_MS;
        sat->obdh.data.sec_payload_state = (uint8_t)PAYLOAD_X;

        /* Send notifcation to read_px task to initialize experiment */
        (void)xTaskNotify(xTaskReadPXHandle, px_active_time_ms, eSetValueWithOverwrite);
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable Secondary payload!");
        sys_log_new_line();
        retval = -1;
    }

    if ((ev->ev_id != EV_PERSIST_STATE_ON_INIT) && (retval == 0))
    {
        satellite_change_mode(OBDH_MODE_EXPERIMENT);
        sat->obdh.data.last_tran_ev_id = ev->ev_id;
        ctx->state = OBDH_MODE_EXPERIMENT;
    }

    return retval;
}

static int goto_fdir_mode(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to)
{
    (void)transition_to;
    sat_data_t *sat = ctx->user_data;
    int retval = 0;
    int err = 0;
    uint8_t retry_count = 5U;

    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_MISSION_MANAGER_NAME, "Transitioning to FDIR Mode because of event (");
    sys_log_print_hex((uint32_t)ev->ev_id);
    sys_log_print_msg(")...");
    sys_log_new_line();

    if (sat->obdh.data.main_payload_state != (uint8_t)PAYLOAD_NONE)
    {
        do
        {
            err = payload_disable((payload_t)sat->obdh.data.main_payload_state);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.main_payload_state = (uint8_t)PAYLOAD_NONE;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Main payload!");
            sys_log_new_line();
            retval = -1;
        }

        err = 0;
        retry_count = 5U;
    }

    if (sat->obdh.data.sec_payload_state != (uint8_t)PAYLOAD_NONE)
    {
        do
        {
            err = payload_disable((payload_t)sat->obdh.data.sec_payload_state);
            vTaskDelay(100U);
            --retry_count;
        } while ((err < 0) && (retry_count > 0U));

        if (retry_count != 0U)
        {
            sat->obdh.data.sec_payload_state = (uint8_t)PAYLOAD_NONE;
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable Secondary payload!");
            sys_log_new_line();
            retval = -1;
        }

        err = 0;
        retry_count = 5U;
    }

    do
    {
        err = eps_set_param(SL_EPS2_REG_BEACON_ENABLE, 0U);
        vTaskDelay(100U);
        --retry_count;
    } while ((err < 0) && (retry_count > 0U));

    if (retry_count == 0U)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable EPS beacon!");
        sys_log_new_line();
    }

    if ((ev->ev_id != EV_PERSIST_STATE_ON_INIT) && (retval == 0))
    {
        satellite_change_mode(OBDH_MODE_FDIR);
        sat->obdh.data.last_tran_ev_id = ev->ev_id;
        ctx->state = OBDH_MODE_FDIR;
    }

    return retval;
}

static int goto_manual_mode(struct conops_fsm *ctx, const struct conops_event *ev,
					   const uint16_t transition_to)
{
    (void)transition_to;
    sat_data_t *sat = ctx->user_data;

    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_MISSION_MANAGER_NAME, "Transitioning to Manual Mode because of event (");
    sys_log_print_hex((uint32_t)ev->ev_id);
    sys_log_print_msg(")...");
    sys_log_new_line();

    if (ev->ev_id != EV_PERSIST_STATE_ON_INIT)
    {
        satellite_change_mode(OBDH_MODE_MANUAL);
        sat->obdh.data.last_tran_ev_id = ev->ev_id;
        ctx->state = OBDH_MODE_MANUAL;
    }

    return 0;
}

static int32_t event_mapper(const struct conops_fsm *ctx, const struct conops_event *ev)
{
    sat_data_t *sat = ctx->user_data;
    int32_t transition_to = -2;

    switch (ev->ev_id)
    {
        case EV_TC_ENTER_HIBERNATION:
        {
            taskENTER_CRITICAL();

            sat->obdh.data.hibernation_on = true;
            sys_time_t hib_duration_hours = (sys_time_t)ev->src;
            sat_data_buf.obdh.data.hib_duration = hib_duration_hours * 60UL * 60UL;

            taskEXIT_CRITICAL();

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Hibernating for ");
            sys_log_print_uint(hib_duration_hours);
            sys_log_print_msg(" hours...");
            sys_log_new_line();

            if (disable_ttc_tx() < 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to disable transmissions through TTC!");
                sys_log_new_line();
            }

            transition_to = ctx->state;
            break;
        }
        case EV_TC_LEAVE_HIBERNATION:
        {
            sat->obdh.data.hibernation_on = false;
            sat_data_buf.obdh.data.hib_duration = 0U;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Leaving hibernation...");
            sys_log_new_line();

            if (enable_ttc_tx() < 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable transmissions through TTC!");
                sys_log_new_line();
            }

            transition_to = ctx->state;
            break;
        }
        case EV_HIBERNATION_TIMEOUT:
        {
            sat->obdh.data.hibernation_on = false;
            sat_data_buf.obdh.data.hib_duration = 0U;

            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Hibernation timeout! Enabling transmissions...");
            sys_log_new_line();

            if (enable_ttc_tx() < 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to enable transmissions through TTC!");
                sys_log_new_line();
            }

            transition_to = ctx->state;
            break;
        }
        default:
            break;
    }

    if (transition_to == -2)
    {
        switch (ctx->state)
        {
            case OBDH_MODE_DEPLOYMENT:
                transition_to = handle_deployment_evs(ctx, ev);
                break;
            case OBDH_MODE_COMMISSION:
                transition_to = handle_commission_evs(ctx, ev);
                break;
            case OBDH_MODE_NORMAL:
                transition_to = handle_nominal_evs(ctx, ev);
                break;
            case OBDH_MODE_STAND_BY:
                transition_to = handle_standby_evs(ctx, ev);
                break;
            case OBDH_MODE_EXPERIMENT:
                transition_to = handle_experiment_evs(ctx, ev);
                break;
            case OBDH_MODE_FDIR:
                transition_to = handle_fdir_evs(ctx, ev);
                break;
            case OBDH_MODE_MANUAL:
                if (EV_CHECK_CRIT_BITMASK(ev->ev_id))
                {
                    transition_to = handle_manual_evs(ctx, ev);
                }
                else
                {
                    sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_MISSION_MANAGER_NAME, "Manual Mode is enabled! Ignoring automatic event...");
                    sys_log_new_line();

                    transition_to = OBDH_MODE_MANUAL;
                }
                break;
            default:
                transition_to = -1;
                break;
        }
    }

    return transition_to;
}

static conops_transition_handler_t mode_transition_table[MISSION_OPERATION_MODES][MISSION_OPERATION_MODES] = {  // cppcheck-suppress misra-c2012-8.9
            /* Deployment (DM) | Commission (CM) | Normal (NM) | Stand-by (SBM) | Experiment (EXM) | FDIR (FDM) | Manual (MNM) */
/* DM */   {NULL, goto_commission_mode, goto_nominal_mode, goto_standby_mode, goto_experiment_mode, goto_fdir_mode, goto_manual_mode},
/* CM */   {NULL, goto_commission_mode, goto_nominal_mode, goto_standby_mode, goto_experiment_mode, goto_fdir_mode, goto_manual_mode},
/* NM */   {NULL, goto_commission_mode, NULL, goto_standby_mode, goto_experiment_mode, goto_fdir_mode, goto_manual_mode},
/* SBM */  {NULL, goto_commission_mode, goto_nominal_mode, NULL, goto_experiment_mode, goto_fdir_mode, goto_manual_mode},
/* EXM */  {NULL, goto_commission_mode, goto_nominal_mode, goto_standby_mode, NULL, goto_fdir_mode, goto_manual_mode},
/* FDM */  {NULL, goto_commission_mode, goto_nominal_mode, goto_standby_mode, NULL, NULL, goto_manual_mode},
/* MNM */  {NULL, goto_commission_mode, goto_nominal_mode, goto_standby_mode, goto_experiment_mode, goto_fdir_mode, NULL},
};

#endif

static int satellite_persist_op_mode(struct conops_fsm *ctx)
{
    int retval = 0;

    const conops_transition_handler_t handlers[] = {NULL, NULL, goto_nominal_mode, goto_standby_mode, goto_experiment_mode, goto_fdir_mode, goto_manual_mode};

    conops_transition_handler_t handler = goto_fdir_mode;

    if (ctx->state < MISSION_OPERATION_MODES)
    {
        handler = handlers[ctx->state];
    }

    if (handler != NULL)
    {
        const struct conops_event fsm_init = {
            .src = 0U,
            .ev_id = EV_PERSIST_STATE_ON_INIT,
            .ev_name = "INIT",
            .callback = NULL,
        };

        retval = (handler)(ctx, &fsm_init, ctx->state);
    }

    return retval;
}

QueueHandle_t event_queue;

TaskHandle_t xTaskMissionManagerHandle;

void vTaskMissionManager(void *p)
{
    (void)p;

    struct conops_fsm op_mode_fsm = {0};
    struct conops_event ev = {0};
    int err = 0;

    (void)xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_MISSION_MANAGER_STARTUP_TIMEOUT_MS));

    sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Initializing operation mode state machine in mode (");
    sys_log_print_hex((uint32_t)sat_data_buf.obdh.data.mode);
    sys_log_print_msg(")...");
    sys_log_new_line();

    err = conops_fsm_init(&op_mode_fsm, mode_transition_table, MISSION_OPERATION_MODES, EV_TC_LEAVE_HIBERNATION, sat_data_buf.obdh.data.mode, satellite_persist_op_mode);

    if (err < 0)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to initialize operation mode state machine! Mode was (");
        sys_log_print_hex((uint32_t)sat_data_buf.obdh.data.mode);
        sys_log_print_msg(")");
        sys_log_new_line();

        if (err == -ERRNO_CONOPS_INVALID_STATE)
        {
            sat_data_buf.obdh.data.mode = OBDH_MODE_FDIR;

            /* Reinitialize FSM in FDIR, since something corrupted the operation mode stored in FRAM */
            (void)conops_fsm_init(&op_mode_fsm, mode_transition_table, MISSION_OPERATION_MODES, EV_TC_LEAVE_HIBERNATION, sat_data_buf.obdh.data.mode, satellite_persist_op_mode);
        }
    }

    (void)conops_register_mapper(&op_mode_fsm, event_mapper);
    (void)conops_register_fsm_user_data(&op_mode_fsm, &sat_data_buf);

    while (1)
    {
        if (xQueueReceive(event_queue, &ev, pdMS_TO_TICKS_64(TASK_MISSION_MANAGER_EV_NOTIFICATION_TIMEOUT)) == pdPASS)
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, TASK_MISSION_MANAGER_NAME, "Received new event (");
            sys_log_print_hex((uint32_t)ev.ev_id);
            sys_log_print_msg(")!");
            sys_log_new_line();

            err = conops_fsm_process_event(&op_mode_fsm, &ev);

            if (err == 0)
            {
                if (EV_CHECK_CRIT_BITMASK(ev.ev_id))
                {
                    /* Notify TC executing task that the event was processed sucessfully */
                    (void)xEventGroupSetBits(task_startup_status, MISSION_MANAGER_NOTIFICATION_BIT);
                }
            }
            else
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_MISSION_MANAGER_NAME, "Failed to process event!");
                sys_log_new_line();
            }
        }
        else
        {
            sys_log_print_event_from_module(SYS_LOG_WARNING, TASK_MISSION_MANAGER_NAME, "Notification waiting timed out!");
            sys_log_new_line();
        }
    }
}

int8_t notify_event_to_mission_manager(const struct conops_event *ev)
{
    int8_t err = 0;

    if (xQueueSendToBack(event_queue, ev, pdMS_TO_TICKS(10U)) != pdPASS)
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, "Event Queue", "Failed to enqueue event");
        sys_log_new_line();

        err = -1;
    }

    return err;
}

void satellite_change_mode(const uint8_t mode)
{
    /* This ensures the mode change is done atomically */
    taskENTER_CRITICAL();
    sat_data_buf.obdh.data.mode = mode;
    sat_data_buf.obdh.data.ts_last_mode_change = system_get_time();
    taskEXIT_CRITICAL();
}

/** \} End of mission_man group */
