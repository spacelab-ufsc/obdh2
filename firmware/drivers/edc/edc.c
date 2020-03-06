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
 * \version 0.2.21
 * 
 * \date 27/10/2019
 * 
 * \addtogroup edc
 * \{
 */

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

    if (edc_get_state(status) != EDC_FRAME_STATE_LEN)
    {
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

int16_t edc_get_state(uint8_t *status)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_STATE}) != 0)
    {
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(status, EDC_FRAME_STATE_LEN) != 0)
    {
        return -1;
    }

    if (status[0] != EDC_FRAME_ID_STATE)
    {
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_STATE_LEN;
}

int16_t edc_get_ptt_pkg(uint8_t *pkg)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_PTT_PKG}) != 0)
    {
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(pkg, EDC_FRAME_PTT_LEN) != 0)
    {
        return -1;
    }

    if (pkg[0] != EDC_FRAME_ID_PTT)
    {
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_PTT_LEN;
}

int16_t edc_get_hk_pkg(uint8_t *hk)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_HK_PKG}) != 0)
    {
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(hk, EDC_FRAME_HK_LEN) != 0)
    {
        return -1;
    }

    if (hk[0] != EDC_FRAME_ID_HK)
    {
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_HK_LEN;
}

int16_t edc_get_adc_seq(uint8_t *seq)
{
    if (edc_write_cmd((edc_cmd_t){.id=EDC_CMD_GET_ADC_SEQ}) != 0)
    {
        return -1;
    }

    /* A minimum time gap of 10 ms must be forced between consecutive I2C commands */
    edc_delay_ms(10);

    if (edc_read(seq, EDC_FRAME_ADC_SEQ_LEN) != 0)
    {
        return -1;
    }

    if (seq[0] != EDC_FRAME_ID_ADC_SEQ)
    {
        return -1;  /* Invalid data! */
    }

    return EDC_FRAME_ADC_SEQ_LEN;
}

int edc_echo()
{
    return edc_write_cmd((edc_cmd_t){.id=EDC_CMD_ECHO});
}

//! \} End of edc group
