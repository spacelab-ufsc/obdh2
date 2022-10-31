/*
 * edc.c
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
 * \brief EDC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Bruno Benedetti <brunobenedetti45@gmail.com>
 * 
 * \version 0.9.13
 * 
 * \date 2019/10/27
 * 
 * \addtogroup edc
 * \{
 */

#include <math.h>
#include <string.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "edc.h"

int edc_init(edc_config_t config)
{
    int err = -1;

    if (edc_gpio_init(config) == 0)
    {
        if (edc_enable(config) == 0)
        {
            switch(config.interface)
            {
                case EDC_IF_UART:
                    if (edc_uart_init(config) == 0)
                    {
                        err = edc_check_device(config);
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error initializing the UART port!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }

                    break;
                case EDC_IF_I2C:
                    if (edc_i2c_init(config) == 0)
                    {
                        err = edc_check_device(config);
                    }
                    else
                    {
                    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error initializing the I2C port!");
                        sys_log_new_line();
                    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    }

                    break;
                default:
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Unexpected interface!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                    break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error enabling the module!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error initializing the GPIO pins!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int edc_enable(edc_config_t config)
{
    return edc_gpio_set(config);
}

int edc_disable(edc_config_t config)
{
    return edc_gpio_clear(config);
}

int edc_write_cmd(edc_config_t config, edc_cmd_t cmd)
{
    int err = 0;

    uint8_t cmd_str[6] = {0};
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
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Invalid command!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;   /* Invalid command */

            break;
    }

    if (err == 0)
    {
        switch(config.interface)
        {
            case EDC_IF_UART:   err = edc_uart_write(config, cmd_str, cmd_str_len); break;
            case EDC_IF_I2C:    err = edc_i2c_write(config, cmd_str, cmd_str_len);  break;
            default:
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Unexpected interface!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;
                break;
        }
    }

    return err;
}

int edc_read(edc_config_t config, uint8_t *data, uint16_t len)
{
    int err = -1;

    switch(config.interface)
    {
        case EDC_IF_UART:
            if (edc_uart_rx_available(config) > 0)
            {
                err = edc_uart_read(config, data, len);
            }

            break;
        case EDC_IF_I2C:
            err = edc_i2c_read(config, data, len);
            break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Unexpected interface!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            break;
    }

    return err;
}

int edc_check_device(edc_config_t config)
{
    int err = 0;

    uint8_t status[EDC_FRAME_STATE_LEN] = {0};

    if (edc_get_state_pkg(config, status) != EDC_FRAME_STATE_LEN)
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error checking the device!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    return err;
}

int edc_set_rtc_time(edc_config_t config, uint32_t time)
{
    edc_cmd_t rtc_cmd;

    rtc_cmd.id = EDC_CMD_RTC_SET;
    rtc_cmd.param = time;

    return edc_write_cmd(config, rtc_cmd);
}

int edc_pop_ptt_pkg(edc_config_t config)
{
    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_PTT_POP;

    return edc_write_cmd(config, cmd);
}

int edc_pause_ptt_task(edc_config_t config)
{
    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_PTT_PAUSE;

    return edc_write_cmd(config, cmd);
}

int edc_resume_ptt_task(edc_config_t config)
{
    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_PTT_RESUME;

    return edc_write_cmd(config, cmd);
}

int edc_start_adc_task(edc_config_t config)
{
    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_SAMPLER_START;

    return edc_write_cmd(config, cmd);
}

int16_t edc_get_state_pkg(edc_config_t config, uint8_t *status)
{
    int16_t res = -1;

    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_GET_STATE;

    if (edc_write_cmd(config, cmd) == 0)
    {
        /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
        edc_delay_ms(100);  /* 10 ms is not enough when using the UART interface! */

        if (edc_read(config, status, EDC_FRAME_STATE_LEN) == 0)
        {
            if (status[0] == EDC_FRAME_ID_STATE)
            {
                res = EDC_FRAME_STATE_LEN;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an state packet! Invalid frame ID (");
                sys_log_print_hex(status[0]);
                sys_log_print_msg(")!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an state packet!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get state\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return res;
}

int16_t edc_get_ptt_pkg(edc_config_t config, uint8_t *pkg)
{
    int res = -1;

    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_GET_PTT_PKG;

    if (edc_write_cmd(config, cmd) == 0)
    {
        /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
        edc_delay_ms(100);  /* 10 ms is not enough when using the UART interface! */

        if (edc_read(config, pkg, EDC_FRAME_PTT_LEN) == 0)
        {
            if (pkg[0] == EDC_FRAME_ID_PTT)
            {
                res = EDC_FRAME_PTT_LEN;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a PTT packet! Invalid frame ID (");
                sys_log_print_hex(pkg[0]);
                sys_log_print_msg(")!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a PTT packet!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get PTT\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return res;
}

int16_t edc_get_hk_pkg(edc_config_t config, uint8_t *hk)
{
    int16_t res = -1;

    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_GET_HK_PKG;

    if (edc_write_cmd(config, cmd) == 0)
    {
        /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
        edc_delay_ms(100);  /* 10 ms is not enough when using the UART interface! */

        if (edc_read(config, hk, EDC_FRAME_HK_LEN) == 0)
        {
            if (hk[0] == EDC_FRAME_ID_HK)
            {
                res = EDC_FRAME_HK_LEN;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a HK packet! Invalid frame ID (");
                sys_log_print_hex(hk[0]);
                sys_log_print_msg(")!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading a HK packet!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get HK\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return res;
}

int16_t edc_get_adc_seq(edc_config_t config, uint8_t *seq)
{
    int16_t res = -1;

    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_GET_ADC_SEQ;

    if (edc_write_cmd(config, cmd) == 0)
    {
        /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
        edc_delay_ms(100);  /* 10 ms is not enough when using the UART interface! */

        if (edc_read(config, seq, EDC_FRAME_ADC_SEQ_LEN) == 0)
        {
            if (seq[0] == EDC_FRAME_ID_ADC_SEQ)
            {
                res = EDC_FRAME_ADC_SEQ_LEN;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an ADC sequence! Invalid frame ID (");
                sys_log_print_hex(seq[0]);
                sys_log_print_msg(")!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading an ADC sequence!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"get ADC\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return res;
}

int edc_echo(edc_config_t config)
{
    int res = -1;

    edc_cmd_t cmd = {0};

    cmd.id = EDC_CMD_ECHO;

    if (edc_write_cmd(config, cmd) == 0)
    {
        if (config.interface == EDC_IF_UART)    /* The echo command just answers when using the UART interface (I think...) */
        {
            /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
            edc_delay_ms(100);  /* 10 ms is not enough when using the UART interface! */

            uint8_t echo_ans[5] = {0};

            if (edc_read(config, echo_ans, EDC_FRAME_ECHO_LEN) == 0)
            {
                uint8_t echo[4] = {'E', 'C', 'H', 'O'}; /* Expected response */

                if (memcmp(echo_ans, echo, 4) != 0)
                {
                #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                    sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading the \"ECHO\" command response! Wrong answer!");
                    sys_log_new_line();
                #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                }
                else
                {
                    res = 0;
                }
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error reading the \"ECHO\" command response!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
            res = 0;
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, EDC_MODULE_NAME, "Error writing the \"ECHO\" command!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return res;
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

int edc_get_state(edc_config_t config, edc_state_t *state_data)
{
    int err = -1;

    uint8_t state_raw[EDC_FRAME_STATE_LEN] = {0};

    /* Get state bytes */
    if (edc_get_state_pkg(config, state_raw) == EDC_FRAME_STATE_LEN)
    {
        /* Check packet ID */
        if (state_raw[0] == EDC_FRAME_ID_STATE)
        {
            /* Verify checksum */
            if (state_raw[EDC_FRAME_STATE_LEN-1] == edc_calc_checksum(state_raw, EDC_FRAME_STATE_LEN-1))
            {
                state_data->current_time    = ((uint32_t)state_raw[4] << 24) |
                                              ((uint32_t)state_raw[3] << 16) |
                                              ((uint32_t)state_raw[2] << 8)  |
                                              ((uint32_t)state_raw[1] << 0);
                state_data->ptt_available   = state_raw[5];
                state_data->ptt_is_paused   = (bool)state_raw[6];
                state_data->sampler_state   = state_raw[7];

                err = 0;
            }
        }
    }

    return err;
}

int edc_get_ptt(edc_config_t config, edc_ptt_t *ptt_data)
{
    int err = -1;

    uint8_t ptt_raw[EDC_FRAME_PTT_LEN] = {0};

    /* Get PTT bytes */
    if (edc_get_ptt_pkg(config, ptt_raw) == EDC_FRAME_PTT_LEN)
    {
        /* Check packet ID */
        if (ptt_raw[0] == EDC_FRAME_ID_PTT)
        {
            /* Verify checksum */
            if (ptt_raw[EDC_FRAME_PTT_LEN-1] == edc_calc_checksum(ptt_raw, EDC_FRAME_PTT_LEN-1))
            {
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

                if (memcpy(ptt_data->user_msg, &ptt_raw[13], 36) == ptt_data->user_msg)
                {
                    err = 0;
                }
            }
        }
    }

    return err;
}

int edc_get_hk(edc_config_t config, edc_hk_t *hk_data)
{
    int err = -1;

    uint8_t hk_raw[EDC_FRAME_HK_LEN] = {0};

    /* Get hk bytes */
    if (edc_get_hk_pkg(config, hk_raw) == EDC_FRAME_HK_LEN)
    {
        /* Check packet ID */
        if (hk_raw[0] == EDC_FRAME_ID_HK)
        {
            /* Verify checksum */
            if (hk_raw[EDC_FRAME_HK_LEN-1] == edc_calc_checksum(hk_raw, EDC_FRAME_HK_LEN-1))
            {
                hk_data->current_time       = ((uint32_t)hk_raw[4] << 24) |
                                              ((uint32_t)hk_raw[3] << 16) |
                                              ((uint32_t)hk_raw[2] << 8)  |
                                              ((uint32_t)hk_raw[1] << 0);
                hk_data->elapsed_time       = ((uint32_t)hk_raw[8] << 24) |
                                              ((uint32_t)hk_raw[7] << 16) |
                                              ((uint32_t)hk_raw[6] << 8)  |
                                              ((uint32_t)hk_raw[5] << 0);
                hk_data->current_supply_d   = ((uint32_t)hk_raw[10] << 8) | ((uint32_t)hk_raw[9] << 0);
                hk_data->current_supply_a   = ((uint32_t)hk_raw[12] << 8) | ((uint32_t)hk_raw[11] << 0);
                hk_data->voltage_supply     = ((uint32_t)hk_raw[14] << 8) | ((uint32_t)hk_raw[13] << 0);
                hk_data->temp               = (int8_t)hk_raw[15] - 40;
                hk_data->pll_sync_bit       = hk_raw[16];
                hk_data->adc_rms            = ((uint16_t)hk_raw[18] << 8) | ((uint16_t)hk_raw[17] << 0);
                hk_data->num_rx_ptt         = ((uint32_t)hk_raw[22] << 24) |
                                              ((uint32_t)hk_raw[21] << 16) |
                                              ((uint32_t)hk_raw[20] << 8) |
                                              ((uint32_t)hk_raw[19] << 0);
                hk_data->max_parl_decod     = hk_raw[23];
                hk_data->mem_err_count      = hk_raw[24];

                err = 0;
            }
        }
    }

    return err;
}

/** \} End of edc group */
