/*
 * edc.c
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
 * \brief EDC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.12
 * 
 * \date 27/10/2019
 * 
 * \addtogroup edc
 * \{
 */

#include <math.h>
#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "edc.h"

/**
 * \brief EDC I2C port.
 */
i2c_port_t edc_i2c_port;

int edc_init(edc_config_t config)
{
    edc_i2c_port = config.port;

    if (i2c_init(config.port, (i2c_config_t){.speed_hz=config.bitrate}) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error initializing the I2C port!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Error initializing the I2C port */
    }

    return edc_check_device();
}

int edc_write_cmd(edc_cmd_t cmd)
{
    uint8_t cmd_str[6];
    uint16_t cmd_str_len = 1;

    cmd_str[0] = cmd.id;

    switch(cmd.id)
    {
        case EDC_CMD_RTC_SET:
            cmd_str[1] = (uint8_t)(cmd.param >> 0);
            cmd_str[2] = (uint8_t)(cmd.param >> 8);
            cmd_str[3] = (uint8_t)(cmd.param >> 16);
            cmd_str[4] = (uint8_t)(cmd.param >> 24);
            cmd_str_len = 5;
            break;
        case EDC_CMD_PTT_POP:           break;
        case EDC_CMD_PTT_PAUSE:         break;
        case EDC_CMD_PTT_RESUME:        break;
        case EDC_CMD_SAMPLER_START:     break;
        case EDC_CMD_GET_STATE:         break;
        case EDC_CMD_GET_PTT_PKG:
            cmd_str[1] = 0;     /* Position 0 */
            cmd_str_len = 2;
            break;
        case EDC_CMD_GET_HK_PKG:        break;
        case EDC_CMD_GET_ADC_SEQ:
            cmd_str[1] = 0;     /* Position 0 */
            cmd_str[2] = 0;
            cmd_str_len = 3;
            break;
        case EDC_CMD_ECHO:              break;
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Invalid command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;  /* Invalid command */
    }

    /* Transmit the command over an I2C port */
    return i2c_write(edc_i2c_port, EDC_SLAVE_ADDRESS, cmd_str, cmd_str_len);
}

int edc_read(uint8_t *data, uint16_t len)
{
    return i2c_read(edc_i2c_port, EDC_SLAVE_ADDRESS, data, len);
}

int edc_check_device()
{
    uint8_t status[EDC_FRAME_STATE_LEN];

    if (edc_get_state_pkg(status) != EDC_FRAME_STATE_LEN)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error checking the device!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    return 0;
}

int edc_set_rtc_time(uint32_t time)
{
    edc_cmd_t rtc_cmd;

    rtc_cmd.id = EDC_CMD_RTC_SET;
    rtc_cmd.param = time;

    return edc_write_cmd(rtc_cmd);
}

int edc_pop_ptt_pkg()
{
    return edc_write_cmd((edc_cmd_t){.id=EDC_CMD_PTT_POP});
}

int edc_pause_ptt_task()
{
    return edc_write_cmd((edc_cmd_t){.id=EDC_CMD_PTT_PAUSE});
}

int edc_resume_ptt_task()
{
    return edc_write_cmd((edc_cmd_t){.id=EDC_CMD_PTT_RESUME});
}

int edc_start_adc_task()
{
    return edc_write_cmd((edc_cmd_t){.id=EDC_CMD_SAMPLER_START});
}

int16_t edc_get_state_pkg(uint8_t *status)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_STATE}) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get state\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(status, EDC_FRAME_STATE_LEN) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an state packet!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    if (status[0] != EDC_FRAME_ID_STATE)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an state packet! Invalid frame ID (");
        sys_log_print_hex(status[0]);
        sys_log_print_msg(")!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_STATE_LEN;
}

int16_t edc_get_ptt_pkg(uint8_t *pkg)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_PTT_PKG}) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get PTT\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(pkg, EDC_FRAME_PTT_LEN) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a PTT packet!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    if (pkg[0] != EDC_FRAME_ID_PTT)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a PTT packet! Invalid frame ID (");
        sys_log_print_hex(pkg[0]);
        sys_log_print_msg(")!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_PTT_LEN;
}

int16_t edc_get_hk_pkg(uint8_t *hk)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_HK_PKG}) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get HK\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(hk, EDC_FRAME_HK_LEN) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a HK packet!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    if (hk[0] != EDC_FRAME_ID_HK)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a HK packet! Invalid frame ID (");
        sys_log_print_hex(hk[0]);
        sys_log_print_msg(")!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_HK_LEN;
}

int16_t edc_get_adc_seq(uint8_t *seq)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_ADC_SEQ}) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get ADC\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(seq, EDC_FRAME_ADC_SEQ_LEN) != 0)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an ADC sequence!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;
    }

    if (seq[0] != EDC_FRAME_ID_ADC_SEQ)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an ADC sequence! Invalid frame ID (");
        sys_log_print_hex(seq[0]);
        sys_log_print_msg(")!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_ADC_SEQ_LEN;
}

int edc_echo()
{
    return edc_write_cmd((edc_cmd_t){.id=EDC_CMD_ECHO});
}

uint16_t edc_calc_checksum(uint8_t *data, uint16_t len)
{
    uint16_t checksum = 0;

    for(uint16_t i=0; i<len; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

int edc_get_state(edc_state_t *state_data)
{
    uint8_t state_raw[EDC_FRAME_STATE_LEN];

    /* Get state bytes */
    if (edc_get_state_pkg(state_raw) != EDC_FRAME_STATE_LEN)
    {
        return -1;  /* Error reading state bytes. */
    }

    /* Check packet ID */
    if (state_raw[0] != EDC_FRAME_ID_STATE)
    {
        return -1;  /* The given packet is not a state packet */
    }

    /* Verify checksum */
    if (state_raw[EDC_FRAME_STATE_LEN-1] != edc_calc_checksum(state_raw, EDC_FRAME_STATE_LEN-1))
    {
        return -1;  /* Invalid data! */
    }

    state_data->current_time    = ((uint32_t)state_raw[4] << 24) |
                                  ((uint32_t)state_raw[3] << 16) |
                                  ((uint32_t)state_raw[2] << 8)  |
                                  ((uint32_t)state_raw[1] << 0);
    state_data->ptt_available   = state_raw[5];
    state_data->ptt_is_paused   = (bool)state_raw[6];
    state_data->sampler_state   = state_raw[7];

    return 0;
}

int edc_get_ptt(edc_ptt_t *ptt_data)
{
    uint8_t ptt_raw[EDC_FRAME_ID_PTT];

    /* Get PTT bytes */
    if (edc_get_ptt_pkg(ptt_raw) != EDC_FRAME_PTT_LEN)
    {
        return -1;  /* Error reading PTT bytes. */
    }

    /* Check packet ID */
    if (ptt_raw[0] != EDC_FRAME_ID_PTT)
    {
        return -1;  /* The given packet is not a PTT packet */
    }

    /* Verify checksum */
    if (ptt_raw[EDC_FRAME_PTT_LEN-1] != edc_calc_checksum(ptt_raw, EDC_FRAME_PTT_LEN-1))
    {
        return -1;  /* Invalid data! */
    }

    ptt_data->time_tag          = ((uint32_t)ptt_raw[4] << 24) |
                                  ((uint32_t)ptt_raw[3] << 16) |
                                  ((uint32_t)ptt_raw[2] << 8)  |
                                  ((uint32_t)ptt_raw[1] << 0);
    ptt_data->error_code        = ptt_raw[5];
    ptt_data->carrier_freq      = ((uint32_t)ptt_raw[9] << 24) |
                                  ((uint32_t)ptt_raw[8] << 16) |
                                  ((uint32_t)ptt_raw[7] << 8)  |
                                  ((uint32_t)ptt_raw[6] << 0);
    ptt_data->carrier_freq      = ptt_data->carrier_freq*128/pow(2, 11) + 401635;
    ptt_data->carrier_abs       = ((uint16_t)ptt_raw[11] << 8) | ((uint16_t)ptt_raw[10] << 0);
    ptt_data->msg_byte_length   = ptt_raw[12];

    memcpy(ptt_data->user_msg, ptt_raw+13, 36);

    return 0;
}

int edc_get_hk(edc_hk_t *hk_data)
{
    uint8_t hk_raw[EDC_FRAME_HK_LEN];

    /* Get hk bytes */
    if (edc_get_hk_pkg(hk_raw) != EDC_FRAME_HK_LEN)
    {
        return -1;  /* Error reading hk bytes. */
    }

    /* Check packet ID */
    if (hk_raw[0] != EDC_FRAME_ID_HK)
    {
        return -1;  /* The given packet is not a housekeeping packet */
    }

    /* Verify checksum */
    if (hk_raw[EDC_FRAME_HK_LEN-1] != edc_calc_checksum(hk_raw, EDC_FRAME_HK_LEN-1))
    {
        return -1;  /* Invalid data! */
    }

    hk_data->current_time       = ((uint32_t)hk_raw[4] << 24) |
                                  ((uint32_t)hk_raw[3] << 16) |
                                  ((uint32_t)hk_raw[2] << 8)  |
                                  ((uint32_t)hk_raw[1] << 0);
    hk_data->elapsed_time       = ((uint32_t)hk_raw[8] << 24) |
                                  ((uint32_t)hk_raw[7] << 16) |
                                  ((uint32_t)hk_raw[6] << 8)  |
                                  ((uint32_t)hk_raw[5] << 0);
    hk_data->current_supply     = ((uint32_t)hk_raw[10] << 8) | ((uint32_t)hk_raw[9] << 0);
    hk_data->voltage_supply     = ((uint32_t)hk_raw[12] << 8) | ((uint32_t)hk_raw[11] << 0);
    hk_data->temp               = (int8_t)hk_raw[13] - 40;
    hk_data->pll_sync_bit       = hk_raw[14];
    hk_data->adc_rms            = (int16_t)(((uint32_t)hk_raw[16] << 8) | ((uint32_t)hk_raw[15] << 0));
    hk_data->num_rx_ptt         = hk_raw[17];
    hk_data->max_parl_decod     = hk_raw[18];
    hk_data->mem_err_count      = hk_raw[19];

    return 0;
}

/** \} End of edc group */
