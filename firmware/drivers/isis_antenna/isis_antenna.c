/*
 * isis_antenna.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.8
 * 
 * \date 2020/02/01
 * 
 * \addtogroup isis_antenna
 * \{
 */

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "isis_antenna.h"
#include "isis_antenna_cmds.h"

int isis_antenna_init(void)
{
    return isis_antenna_i2c_init();
}

int isis_antenna_arm(void)
{
    int err = -1;

    uint8_t cmd = ISIS_ANTENNA_CMD_ARM;

    if (isis_antenna_i2c_write(&cmd, 1) == 0)
    {
        isis_antenna_delay_ms(100);

        bool armed = false;

        if (isis_antenna_get_arming_status(&armed) == 0)
        {
            if (armed == true)
            {
                err = 0;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "The antenna is not armed after the command to arm!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading if the antenna is armed or not!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to arm!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_disarm(void)
{
    int err = -1;

    uint8_t cmd = ISIS_ANTENNA_CMD_DISARM;

    if (isis_antenna_i2c_write(&cmd, 1) == 0)
    {
        isis_antenna_delay_ms(100);

        bool armed = true;

        if (isis_antenna_get_arming_status(&armed) == 0)
        {
            if (armed == false)
            {
                err = 0;
            }
            else
            {
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "The antenna is not disarmed after the command to disarm!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading if the antenna is disarmed or not!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to disarm!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_start_sequential_deploy(uint8_t sec)
{
    int err = -1;

    uint8_t cmd[2] = {0};

    cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_SEQUENTIAL;
    cmd[1] = sec;

    if (isis_antenna_i2c_write(cmd, 2) == 0)
    {
        isis_antenna_delay_ms(100);

        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to start the sequential deploy!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_start_independent_deploy(isis_antenna_ant_t ant, uint8_t sec, isis_antenna_override_t ovr)
{
    int err = 0;

    uint8_t cmd[2] = {0};

    cmd[1] = sec;

    if (ovr == ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITH_OVERRIDE)
    {
        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_2:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_3:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3_WITH_OVERRIDE;
                break;
            case ISIS_ANTENNA_ANT_4:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_4_WITH_OVERRIDE;
                break;
            default:
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to start the sequential deploy: ");
                sys_log_print_int(ant);
                sys_log_print_msg(" is an invalid antenna!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;
                break;
        }
    }
    else if (ovr == ISIS_ANTENNA_INDEPENDENT_DEPLOY_WITHOUT_OVERRIDE)
    {
        switch(ant)
        {
            case ISIS_ANTENNA_ANT_1:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_1;
                break;
            case ISIS_ANTENNA_ANT_2:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_2;
                break;
            case ISIS_ANTENNA_ANT_3:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_3;
                break;
            case ISIS_ANTENNA_ANT_4:
                cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_ANT_4;
                break;
            default:
            #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
                sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to start the sequential deploy: ");
                sys_log_print_int(ant);
                sys_log_print_msg(" is an invalid antenna!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                err = -1;
                break;
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to start the sequential deploy: ");
        sys_log_print_int(ovr);
        sys_log_print_msg(" is an invalid override option!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        err = -1;
    }

    if (err == 0)
    {
        if (isis_antenna_i2c_write(cmd, 2) == 0)
        {
            isis_antenna_delay_ms(100);
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to start the independent deploy!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int isis_antenna_read_deployment_status_code(uint16_t *status)
{
    int err = -1;

    uint16_t status_code = ISIS_ANTENNA_STATUS_MASK;    /* Initial state */

    uint8_t cmd = ISIS_ANTENNA_CMD_REPORT_DEPLOY_STATUS;

    if (isis_antenna_i2c_write(&cmd, 1) == 0)
    {
        isis_antenna_delay_ms(100);

        uint8_t status_bytes[2] = {0};

        if (isis_antenna_i2c_read(status_bytes, 2) == 0)
        {
            status_code = ((uint16_t)status_bytes[0] << 8) | (uint16_t)status_bytes[1];

            *status = status_code;

            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the deployment status code!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to read the deployment status code!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_read_deployment_status(isis_antenna_status_t *status)
{
    int err = -1;

    uint16_t status_code = UINT16_MAX;

    if (isis_antenna_read_deployment_status_code(&status_code) == 0)
    {
        status->code                = status_code;
        status->antenna_1.status    = (status_code >> 15) & 1U;
        status->antenna_1.timeout   = (status_code >> 14) & 1U;
        status->antenna_1.burning   = (status_code >> 13) & 1U;
        status->antenna_2.status    = (status_code >> 11) & 1U;
        status->antenna_2.timeout   = (status_code >> 10) & 1U;
        status->antenna_2.burning   = (status_code >> 9) & 1U;
        status->ignoring_switches   = (status_code >> 8) & 1U;
        status->antenna_3.status    = (status_code >> 7) & 1U;
        status->antenna_3.timeout   = (status_code >> 6) & 1U;
        status->antenna_3.burning   = (status_code >> 5) & 1U;
        status->independent_burn    = (status_code >> 4) & 1U;
        status->antenna_4.status    = (status_code >> 3) & 1U;
        status->antenna_4.timeout   = (status_code >> 2) & 1U;
        status->antenna_4.burning   = (status_code >> 1) & 1U;
        status->armed               = (status_code >> 0) & 1U;

        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the deployment status code!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_get_data(isis_antenna_data_t *data)
{
    int err = -1;

    if (isis_antenna_read_deployment_status(&data->status) == 0)
    {
        if (isis_antenna_get_temperature_k(&data->temperature) == 0)
        {
            err = 0;
        }
    }

    return err;
}

int isis_antenna_get_antenna_status(isis_antenna_ant_t ant, uint8_t *ant_status)
{
    int err = 0;

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:    break;
        case ISIS_ANTENNA_ANT_2:    break;
        case ISIS_ANTENNA_ANT_3:    break;
        case ISIS_ANTENNA_ANT_4:    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the antenna status! ");
            sys_log_print_int(ant);
            sys_log_print_msg(" is an invalid antenna!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
            break;
    }

    if (err == 0)
    {
        isis_antenna_status_t status = {0};

        if (isis_antenna_read_deployment_status(&status) == 0)
        {
            switch(ant)
            {
                case ISIS_ANTENNA_ANT_1:    *ant_status = status.antenna_1.status;  break;
                case ISIS_ANTENNA_ANT_2:    *ant_status = status.antenna_2.status;  break;
                case ISIS_ANTENNA_ANT_3:    *ant_status = status.antenna_3.status;  break;
                case ISIS_ANTENNA_ANT_4:    *ant_status = status.antenna_4.status;  break;
                default:                                                            break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the antenna status!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int isis_antenna_get_antenna_timeout(isis_antenna_ant_t ant, uint8_t *ant_timeout)
{
    int err = 0;

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:    break;
        case ISIS_ANTENNA_ANT_2:    break;
        case ISIS_ANTENNA_ANT_3:    break;
        case ISIS_ANTENNA_ANT_4:    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the antenna timeout! ");
            sys_log_print_int(ant);
            sys_log_print_msg(" is an invalid antenna!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
            break;
    }

    if (err == 0)
    {
        isis_antenna_status_t status = {0};

        if (isis_antenna_read_deployment_status(&status) == 0)
        {
            switch(ant)
            {
                case ISIS_ANTENNA_ANT_1:    *ant_timeout = status.antenna_1.timeout;    break;
                case ISIS_ANTENNA_ANT_2:    *ant_timeout = status.antenna_2.timeout;    break;
                case ISIS_ANTENNA_ANT_3:    *ant_timeout = status.antenna_3.timeout;    break;
                case ISIS_ANTENNA_ANT_4:    *ant_timeout = status.antenna_4.timeout;    break;
                default:                                                                break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the antenna timeout!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int isis_antenna_get_burning(isis_antenna_ant_t ant, uint8_t *ant_burn)
{
    int err = 0;

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:    break;
        case ISIS_ANTENNA_ANT_2:    break;
        case ISIS_ANTENNA_ANT_3:    break;
        case ISIS_ANTENNA_ANT_4:    break;
        default:
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the antenna burning! ");
            sys_log_print_int(ant);
            sys_log_print_msg(" is an invalid antenna!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
            break;
    }

    if (err == 0)
    {
        isis_antenna_status_t status = {0};

        if (isis_antenna_read_deployment_status(&status) == 0)
        {
            switch(ant)
            {
                case ISIS_ANTENNA_ANT_1:    *ant_burn = status.antenna_1.burning;   break;
                case ISIS_ANTENNA_ANT_2:    *ant_burn = status.antenna_2.burning;   break;
                case ISIS_ANTENNA_ANT_3:    *ant_burn = status.antenna_3.burning;   break;
                case ISIS_ANTENNA_ANT_4:    *ant_burn = status.antenna_4.burning;   break;
                default:                                                            break;
            }
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the antenna burning!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            err = -1;
        }
    }

    return err;
}

int isis_antenna_get_arming_status(bool *armed)
{
    int err = -1;

    isis_antenna_status_t status = {0};

    if (isis_antenna_read_deployment_status(&status) == 0)
    {
        *armed = (bool)status.armed;

        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the arming status!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_get_raw_temperature(uint16_t *temp)
{
    int err = -1;

    uint8_t cmd = ISIS_ANTENNA_CMD_MEASURE_TEMPERATURE;

    if (isis_antenna_i2c_write(&cmd, 1) == 0)
    {
        isis_antenna_delay_ms(100);

        uint8_t temp_bytes[2] = {0};

        if (isis_antenna_i2c_read(temp_bytes, 2) == 0)
        {
            uint16_t raw_data = ((uint16_t)temp_bytes[0] << 8) | temp_bytes[1];

            *temp = raw_data;

            err = 0;
        }
        else
        {
        #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
            sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the raw temperature!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        }
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error writing the command to read the raw temperature!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int16_t isis_antenna_raw_to_temp_c(uint16_t raw)
{
    int16_t res = INT16_MAX;

    uint32_t vout = (ISIS_ANTENNA_REF_VOLTAGE/1023.0) * (uint32_t)raw;

/*    if (vout >= 2616U)          { res = -50; }
    else if (vout >= 2607U)     { res = -49; }
    else if (vout >= 2598U)     { res = -48; }
    else if (vout >= 2589U)     { res = -47; }
    else if (vout >= 2580U)     { res = -46; }
    else if (vout >= 2571U)     { res = -45; }
    else if (vout >= 2562U)     { res = -44; }
    else if (vout >= 2553U)     { res = -43; }
    else if (vout >= 2543U)     { res = -42; }
    else if (vout >= 2533U)     { res = -41; }*/
    if (vout >= 2522U)     { res = -40; }
    else if (vout >= 2512U)     { res = -39; }
    else if (vout >= 2501U)     { res = -38; }
    else if (vout >= 2491U)     { res = -37; }
    else if (vout >= 2481U)     { res = -36; }
    else if (vout >= 2470U)     { res = -35; }
    else if (vout >= 2460U)     { res = -34; }
    else if (vout >= 2449U)     { res = -33; }
    else if (vout >= 2439U)     { res = -32; }
    else if (vout >= 2429U)     { res = -31; }
    else if (vout >= 2418U)     { res = -30; }
    else if (vout >= 2408U)     { res = -29; }
    else if (vout >= 2397U)     { res = -28; }
    else if (vout >= 2387U)     { res = -27; }
    else if (vout >= 2376U)     { res = -26; }
    else if (vout >= 2366U)     { res = -25; }
    else if (vout >= 2355U)     { res = -24; }
    else if (vout >= 2345U)     { res = -23; }
    else if (vout >= 2334U)     { res = -22; }
    else if (vout >= 2324U)     { res = -21; }
    else if (vout >= 2313U)     { res = -20; }
    else if (vout >= 2302U)     { res = -19; }
    else if (vout >= 2292U)     { res = -18; }
    else if (vout >= 2281U)     { res = -17; }
    else if (vout >= 2271U)     { res = -16; }
    else if (vout >= 2260U)     { res = -15; }
    else if (vout >= 2250U)     { res = -14; }
    else if (vout >= 2239U)     { res = -13; }
    else if (vout >= 2228U)     { res = -12; }
    else if (vout >= 2218U)     { res = -11; }
    else if (vout >= 2207U)     { res = -10; }
    else if (vout >= 2197U)     { res = -9; }
    else if (vout >= 2186U)     { res = -8; }
    else if (vout >= 2175U)     { res = -7; }
    else if (vout >= 2164U)     { res = -6; }
    else if (vout >= 2154U)     { res = -5; }
    else if (vout >= 2143U)     { res = -4; }
    else if (vout >= 2132U)     { res = -3; }
    else if (vout >= 2122U)     { res = -2; }
    else if (vout >= 2111U)     { res = -1; }
    else if (vout >= 2100U)     { res = 0; }
    else if (vout >= 2089U)     { res = 1; }
    else if (vout >= 2079U)     { res = 2; }
    else if (vout >= 2068U)     { res = 3; }
    else if (vout >= 2057U)     { res = 4; }
    else if (vout >= 2047U)     { res = 5; }
    else if (vout >= 2036U)     { res = 6; }
    else if (vout >= 2025U)     { res = 7; }
    else if (vout >= 2014U)     { res = 8; }
    else if (vout >= 2004U)     { res = 9; }
    else if (vout >= 1993U)     { res = 10; }
    else if (vout >= 1982U)     { res = 11; }
    else if (vout >= 1971U)     { res = 12; }
    else if (vout >= 1961U)     { res = 13; }
    else if (vout >= 1950U)     { res = 14; }
    else if (vout >= 1939U)     { res = 15; }
    else if (vout >= 1928U)     { res = 16; }
    else if (vout >= 1918U)     { res = 17; }
    else if (vout >= 1907U)     { res = 18; }
    else if (vout >= 1896U)     { res = 19; }
    else if (vout >= 1885U)     { res = 20; }
    else if (vout >= 1874U)     { res = 21; }
    else if (vout >= 1864U)     { res = 22; }
    else if (vout >= 1853U)     { res = 23; }
    else if (vout >= 1842U)     { res = 24; }
    else if (vout >= 1831U)     { res = 25; }
    else if (vout >= 1820U)     { res = 26; }
    else if (vout >= 1810U)     { res = 27; }
    else if (vout >= 1799U)     { res = 28; }
    else if (vout >= 1788U)     { res = 29; }
    else if (vout >= 1777U)     { res = 30; }
    else if (vout >= 1766U)     { res = 31; }
    else if (vout >= 1756U)     { res = 32; }
    else if (vout >= 1745U)     { res = 33; }
    else if (vout >= 1734U)     { res = 34; }
    else if (vout >= 1723U)     { res = 35; }
    else if (vout >= 1712U)     { res = 36; }
    else if (vout >= 1701U)     { res = 37; }
    else if (vout >= 1690U)     { res = 38; }
    else if (vout >= 1679U)     { res = 39; }
    else if (vout >= 1668U)     { res = 40; }
    else if (vout >= 1657U)     { res = 41; }
    else if (vout >= 1646U)     { res = 42; }
    else if (vout >= 1635U)     { res = 43; }
    else if (vout >= 1624U)     { res = 44; }
    else if (vout >= 1613U)     { res = 45; }
    else if (vout >= 1602U)     { res = 46; }
    else if (vout >= 1591U)     { res = 47; }
    else if (vout >= 1580U)     { res = 48; }
    else if (vout >= 1569U)     { res = 49; }
    else if (vout >= 1558U)     { res = 50; }
    else if (vout >= 1547U)     { res = 51; }
    else if (vout >= 1536U)     { res = 52; }
    else if (vout >= 1525U)     { res = 53; }
    else if (vout >= 1514U)     { res = 54; }
    else if (vout >= 1503U)     { res = 55; }
    else if (vout >= 1492U)     { res = 56; }
    else if (vout >= 1481U)     { res = 57; }
    else if (vout >= 1470U)     { res = 58; }
    else if (vout >= 1459U)     { res = 59; }
    else if (vout >= 1448U)     { res = 60; }
    else if (vout >= 1436U)     { res = 61; }
    else if (vout >= 1425U)     { res = 62; }
    else if (vout >= 1414U)     { res = 63; }
    else if (vout >= 1403U)     { res = 64; }
    else if (vout >= 1391U)     { res = 65; }
    else if (vout >= 1380U)     { res = 66; }
    else if (vout >= 1369U)     { res = 67; }
    else if (vout >= 1358U)     { res = 68; }
    else if (vout >= 1346U)     { res = 69; }
    else if (vout >= 1335U)     { res = 70; }
/*    else if (vout >= 1324U)     { res = 71; }
    else if (vout >= 1313U)     { res = 72; }
    else if (vout >= 1301U)     { res = 73; }
    else if (vout >= 1290U)     { res = 74; }
    else if (vout >= 1279U)     { res = 75; }
    else if (vout >= 1268U)     { res = 76; }
    else if (vout >= 1257U)     { res = 77; }
    else if (vout >= 1245U)     { res = 78; }
    else if (vout >= 1234U)     { res = 79; }
    else if (vout >= 1223U)     { res = 80; }
    else if (vout >= 1212U)     { res = 81; }
    else if (vout >= 1201U)     { res = 82; }
    else if (vout >= 1189U)     { res = 83; }
    else if (vout >= 1178U)     { res = 84; }
    else if (vout >= 1167U)     { res = 85; }
    else if (vout >= 1155U)     { res = 86; }
    else if (vout >= 1144U)     { res = 87; }
    else if (vout >= 1133U)     { res = 88; }
    else if (vout >= 1122U)     { res = 89; }
    else if (vout >= 1110U)     { res = 90; }
    else if (vout >= 1099U)     { res = 91; }
    else if (vout >= 1088U)     { res = 92; }
    else if (vout >= 1076U)     { res = 93; }
    else if (vout >= 1065U)     { res = 94; }
    else if (vout >= 1054U)     { res = 95; }
    else if (vout >= 1042U)     { res = 96; }
    else if (vout >= 1031U)     { res = 97; }
    else if (vout >= 1020U)     { res = 98; }
    else if (vout >= 1008U)     { res = 99; }
    else if (vout >= 997U)      { res = 100; }
    else if (vout >= 986U)      { res = 101; }
    else if (vout >= 974U)      { res = 102; }
    else if (vout >= 963U)      { res = 103; }
    else if (vout >= 951U)      { res = 104; }
    else if (vout >= 940U)      { res = 105; }
    else if (vout >= 929U)      { res = 106; }
    else if (vout >= 917U)      { res = 107; }
    else if (vout >= 906U)      { res = 108; }
    else if (vout >= 895U)      { res = 109; }
    else if (vout >= 883U)      { res = 110; }
    else if (vout >= 872U)      { res = 111; }
    else if (vout >= 860U)      { res = 112; }
    else if (vout >= 849U)      { res = 113; }
    else if (vout >= 837U)      { res = 114; }
    else if (vout >= 826U)      { res = 115; }
    else if (vout >= 814U)      { res = 116; }
    else if (vout >= 803U)      { res = 117; }
    else if (vout >= 791U)      { res = 118; }
    else if (vout >= 780U)      { res = 119; }
    else if (vout >= 769U)      { res = 120; }
    else if (vout >= 757U)      { res = 121; }
    else if (vout >= 745U)      { res = 122; }
    else if (vout >= 734U)      { res = 123; }
    else if (vout >= 722U)      { res = 124; }
    else if (vout >= 711U)      { res = 125; }
    else if (vout >= 699U)      { res = 126; }
    else if (vout >= 688U)      { res = 127; }
    else if (vout >= 676U)      { res = 128; }
    else if (vout >= 665U)      { res = 129; }
    else if (vout >= 653U)      { res = 130; }
    else if (vout >= 642U)      { res = 131; }
    else if (vout >= 630U)      { res = 132; }
    else if (vout >= 618U)      { res = 133; }
    else if (vout >= 607U)      { res = 134; }
    else if (vout >= 595U)      { res = 135; }
    else if (vout >= 584U)      { res = 136; }
    else if (vout >= 572U)      { res = 137; }
    else if (vout >= 560U)      { res = 138; }
    else if (vout >= 549U)      { res = 139; }
    else if (vout >= 537U)      { res = 140; }
    else if (vout >= 525U)      { res = 141; }
    else if (vout >= 514U)      { res = 142; }
    else if (vout >= 502U)      { res = 143; }
    else if (vout >= 490U)      { res = 144; }
    else if (vout >= 479U)      { res = 145; }
    else if (vout >= 467U)      { res = 146; }
    else if (vout >= 455U)      { res = 147; }
    else if (vout >= 443U)      { res = 148; }
    else if (vout >= 432U)      { res = 149; }
    else if (vout >= 420U)      { res = 150; }*/
    else                        { res = INT16_MAX; }

    return res;
}

int isis_antenna_get_temperature_c(int16_t *temp)
{
    int err = -1;

    uint16_t raw_temp = UINT16_MAX;

    if (isis_antenna_get_raw_temperature(&raw_temp) == 0)
    {
        *temp = isis_antenna_raw_to_temp_c(raw_temp);

        err = 0;
    }
    else
    {
    #if defined(CONFIG_DRIVERS_DEBUG_ENABLED) && (CONFIG_DRIVERS_DEBUG_ENABLED == 1)
        sys_log_print_event_from_module(SYS_LOG_ERROR, ISIS_ANTENNA_MODULE_NAME, "Error reading the temperature!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return err;
}

int isis_antenna_get_temperature_k(isis_antenna_temp_t *temp)
{
    int err = -1;

    int16_t temp_c = INT16_MAX;

    if (isis_antenna_get_temperature_c(&temp_c) == 0)
    {
        if (temp_c != INT16_MAX)
        {
            *temp = temp_c + 273;
        }
        else
        {
            *temp = UINT16_MAX;
        }

        err = 0;
    }

    return err;
}

/** \} End of isis_antenna group */
