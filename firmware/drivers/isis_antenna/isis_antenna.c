/*
 * isis_antenna.c
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief ISIS antenna driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.15
 * 
 * \date 2020/02/01
 * 
 * \addtogroup isis_antenna
 * \{
 */

#include <drivers/i2c/i2c.h>

#include "isis_antenna.h"
#include "isis_antenna_cmds.h"

int isis_antenna_init(void)
{
    return i2c_init(ISIS_ANTENNA_I2C_PORT, (i2c_config_t){.speed_hz=ISIS_ANTENNA_I2C_CLOCK_HZ});
}

bool isis_antenna_arm(void)
{
    uint8_t cmd = ISIS_ANTENNA_CMD_ARM;

    if (i2c_write(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, &cmd, 1) != 0)
    {
        return false;
    }

    isis_antenna_delay_ms(100);

    return isis_antenna_get_arming_status();
}

bool isis_antenna_disarm(void)
{
    uint8_t cmd = ISIS_ANTENNA_CMD_DISARM;
    if (i2c_write(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, &cmd, 1) != 0)
    {
        return false;
    }

    isis_antenna_delay_ms(100);

    return isis_antenna_get_arming_status() ? false : true;
}

void isis_antenna_start_sequential_deploy(uint8_t sec)
{
    uint8_t cmd[2];

    cmd[0] = ISIS_ANTENNA_CMD_DEPLOY_SEQUENTIAL;
    cmd[1] = sec;

    if (i2c_write(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, cmd, 2) != 0)
    {
        return;
    }

    isis_antenna_delay_ms(100);
}

void isis_antenna_start_independent_deploy(uint8_t ant, uint8_t sec, uint8_t ovr)
{
    uint8_t cmd[2];

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
                cmd[0] = ISIS_ANTENNA_CMD_DISARM;
        }
    }
    else
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
                cmd[0] = ISIS_ANTENNA_CMD_DISARM;
        }
    }

    if (i2c_write(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, cmd, 2) != 0)
    {
        return;
    }

    isis_antenna_delay_ms(100);
}

uint16_t isis_antenna_read_deployment_status_code(void)
{
    uint16_t status_code = ISIS_ANTENNA_STATUS_MASK;    /* Initial state */

    uint8_t cmd = ISIS_ANTENNA_CMD_REPORT_DEPLOY_STATUS;
    if (i2c_write(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, &cmd, 1) != 0)
    {
        return 0xFFFF;
    }

    isis_antenna_delay_ms(100);

    uint8_t status_bytes[2];

    if (i2c_read(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, status_bytes, 2) != 0)
    {
        return 0xFFFF;
    }

    status_code = (uint16_t)(status_bytes[1] << 8) | status_bytes[0];

    return status_code;
}

isis_antenna_status_t isis_antenna_read_deployment_status(void)
{
    uint16_t status_code = isis_antenna_read_deployment_status_code();

    isis_antenna_status_t status;

    status.code                 = status_code;
    status.antenna_1.status     = (status_code >> 15) & 0x01;
    status.antenna_1.timeout    = (status_code >> 14) & 0x01;
    status.antenna_1.burning    = (status_code >> 13) & 0x01;
    status.antenna_2.status     = (status_code >> 11) & 0x01;
    status.antenna_2.timeout    = (status_code >> 10) & 0x01;
    status.antenna_2.burning    = (status_code >> 9) & 0x01;
    status.ignoring_switches    = (status_code >> 8) & 0x01;
    status.antenna_3.status     = (status_code >> 7) & 0x01;
    status.antenna_3.timeout    = (status_code >> 6) & 0x01;
    status.antenna_3.burning    = (status_code >> 5) & 0x01;
    status.independent_burn     = (status_code >> 4) & 0x01;
    status.antenna_4.status     = (status_code >> 3) & 0x01;
    status.antenna_4.timeout    = (status_code >> 2) & 0x01;
    status.antenna_4.burning    = (status_code >> 1) & 0x01;
    status.armed                = (status_code >> 0) & 0x01;

    return status;
}

uint8_t isis_antenna_get_antenna_status(uint8_t ant)
{
    isis_antenna_status_t status = isis_antenna_read_deployment_status();

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:
            return status.antenna_1.status;
        case ISIS_ANTENNA_ANT_2:
            return status.antenna_2.status;
        case ISIS_ANTENNA_ANT_3:
            return status.antenna_3.status;
        case ISIS_ANTENNA_ANT_4:
            return status.antenna_4.status;
        default:
            return ISIS_ANTENNA_STATUS_DEPLOYED;
    }
}

uint8_t isis_antenna_get_antenna_timeout(uint8_t ant)
{
    isis_antenna_status_t status = isis_antenna_read_deployment_status();

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:
            return status.antenna_1.timeout;
        case ISIS_ANTENNA_ANT_2:
            return status.antenna_2.timeout;
        case ISIS_ANTENNA_ANT_3:
            return status.antenna_3.timeout;
        case ISIS_ANTENNA_ANT_4:
            return status.antenna_4.timeout;
        default:
            return ISIS_ANTENNA_OTHER_CAUSE;
    }
}

uint8_t isis_antenna_get_burning(uint8_t ant)
{
    isis_antenna_status_t status = isis_antenna_read_deployment_status();

    switch(ant)
    {
        case ISIS_ANTENNA_ANT_1:
            return status.antenna_1.burning;
        case ISIS_ANTENNA_ANT_2:
            return status.antenna_2.burning;
        case ISIS_ANTENNA_ANT_3:
            return status.antenna_3.burning;
        case ISIS_ANTENNA_ANT_4:
            return status.antenna_4.burning;
        default:
            return ISIS_ANTENNA_BURN_INACTIVE;
    }
}

bool isis_antenna_get_arming_status(void)
{
    return (bool)isis_antenna_read_deployment_status().armed;
}

uint16_t isis_antenna_get_raw_temperature(void)
{
    uint8_t cmd = ISIS_ANTENNA_CMD_MEASURE_TEMPERATURE;
    if (i2c_write(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, &cmd, 1) != 0)
    {
        return 0xFFFF;
    }

    isis_antenna_delay_ms(100);

    uint8_t temp_bytes[2];

    if (i2c_read(ISIS_ANTENNA_I2C_PORT, ISIS_ANTENNA_I2C_SLAVE_ADDRESS, temp_bytes, 2) != 0)
    {
        return 0xFFFF;
    }

    uint16_t raw_data = (uint16_t)(temp_bytes[1] << 8) | temp_bytes[0];

    return raw_data;
}

int16_t isis_antenna_raw_to_temp_c(uint16_t raw)
{
    uint16_t vout = ISIS_ANTENNA_REF_VOLTAGE/1023.0 * raw;

    if (vout >= 2616)           return -50;
    else if (vout >= 2607)      return -49;
    else if (vout >= 2598)      return -48;
    else if (vout >= 2589)      return -47;
    else if (vout >= 2580)      return -46;
    else if (vout >= 2571)      return -45;
    else if (vout >= 2562)      return -44;
    else if (vout >= 2553)      return -43;
    else if (vout >= 2543)      return -42;
    else if (vout >= 2533)      return -41;
    else if (vout >= 2522)      return -40;
    else if (vout >= 2512)      return -39;
    else if (vout >= 2501)      return -38;
    else if (vout >= 2491)      return -37;
    else if (vout >= 2481)      return -36;
    else if (vout >= 2470)      return -35;
    else if (vout >= 2460)      return -34;
    else if (vout >= 2449)      return -33;
    else if (vout >= 2439)      return -32;
    else if (vout >= 2429)      return -31;
    else if (vout >= 2418)      return -30;
    else if (vout >= 2408)      return -29;
    else if (vout >= 2397)      return -28;
    else if (vout >= 2387)      return -27;
    else if (vout >= 2376)      return -26;
    else if (vout >= 2366)      return -25;
    else if (vout >= 2355)      return -24;
    else if (vout >= 2345)      return -23;
    else if (vout >= 2334)      return -22;
    else if (vout >= 2324)      return -21;
    else if (vout >= 2313)      return -20;
    else if (vout >= 2302)      return -19;
    else if (vout >= 2292)      return -18;
    else if (vout >= 2281)      return -17;
    else if (vout >= 2271)      return -16;
    else if (vout >= 2260)      return -15;
    else if (vout >= 2250)      return -14;
    else if (vout >= 2239)      return -13;
    else if (vout >= 2228)      return -12;
    else if (vout >= 2218)      return -11;
    else if (vout >= 2207)      return -10;
    else if (vout >= 2197)      return -9;
    else if (vout >= 2186)      return -8;
    else if (vout >= 2175)      return -7;
    else if (vout >= 2164)      return -6;
    else if (vout >= 2154)      return -5;
    else if (vout >= 2143)      return -4;
    else if (vout >= 2132)      return -3;
    else if (vout >= 2122)      return -2;
    else if (vout >= 2111)      return -1;
    else if (vout >= 2100)      return 0;
    else if (vout >= 2089)      return 1;
    else if (vout >= 2079)      return 2;
    else if (vout >= 2068)      return 3;
    else if (vout >= 2057)      return 4;
    else if (vout >= 2047)      return 5;
    else if (vout >= 2036)      return 6;
    else if (vout >= 2025)      return 7;
    else if (vout >= 2014)      return 8;
    else if (vout >= 2004)      return 9;
    else if (vout >= 1993)      return 10;
    else if (vout >= 1982)      return 11;
    else if (vout >= 1971)      return 12;
    else if (vout >= 1961)      return 13;
    else if (vout >= 1950)      return 14;
    else if (vout >= 1939)      return 15;
    else if (vout >= 1928)      return 16;
    else if (vout >= 1918)      return 17;
    else if (vout >= 1907)      return 18;
    else if (vout >= 1896)      return 19;
    else if (vout >= 1885)      return 20;
    else if (vout >= 1874)      return 21;
    else if (vout >= 1864)      return 22;
    else if (vout >= 1853)      return 23;
    else if (vout >= 1842)      return 24;
    else if (vout >= 1831)      return 25;
    else if (vout >= 1820)      return 26;
    else if (vout >= 1810)      return 27;
    else if (vout >= 1799)      return 28;
    else if (vout >= 1788)      return 29;
    else if (vout >= 1777)      return 30;
    else if (vout >= 1766)      return 31;
    else if (vout >= 1756)      return 32;
    else if (vout >= 1745)      return 33;
    else if (vout >= 1734)      return 34;
    else if (vout >= 1723)      return 35;
    else if (vout >= 1712)      return 36;
    else if (vout >= 1701)      return 37;
    else if (vout >= 1690)      return 38;
    else if (vout >= 1679)      return 39;
    else if (vout >= 1668)      return 40;
    else if (vout >= 1657)      return 41;
    else if (vout >= 1646)      return 42;
    else if (vout >= 1635)      return 43;
    else if (vout >= 1624)      return 44;
    else if (vout >= 1613)      return 45;
    else if (vout >= 1602)      return 46;
    else if (vout >= 1591)      return 47;
    else if (vout >= 1580)      return 48;
    else if (vout >= 1569)      return 49;
    else if (vout >= 1558)      return 50;
    else if (vout >= 1547)      return 51;
    else if (vout >= 1536)      return 52;
    else if (vout >= 1525)      return 53;
    else if (vout >= 1514)      return 54;
    else if (vout >= 1503)      return 55;
    else if (vout >= 1492)      return 56;
    else if (vout >= 1481)      return 57;
    else if (vout >= 1470)      return 58;
    else if (vout >= 1459)      return 59;
    else if (vout >= 1448)      return 60;
    else if (vout >= 1436)      return 61;
    else if (vout >= 1425)      return 62;
    else if (vout >= 1414)      return 63;
    else if (vout >= 1403)      return 64;
    else if (vout >= 1391)      return 65;
    else if (vout >= 1380)      return 66;
    else if (vout >= 1369)      return 67;
    else if (vout >= 1358)      return 68;
    else if (vout >= 1346)      return 69;
    else if (vout >= 1335)      return 70;
    else if (vout >= 1324)      return 71;
    else if (vout >= 1313)      return 72;
    else if (vout >= 1301)      return 73;
    else if (vout >= 1290)      return 74;
    else if (vout >= 1279)      return 75;
    else if (vout >= 1268)      return 76;
    else if (vout >= 1257)      return 77;
    else if (vout >= 1245)      return 78;
    else if (vout >= 1234)      return 79;
    else if (vout >= 1223)      return 80;
    else if (vout >= 1212)      return 81;
    else if (vout >= 1201)      return 82;
    else if (vout >= 1189)      return 83;
    else if (vout >= 1178)      return 84;
    else if (vout >= 1167)      return 85;
    else if (vout >= 1155)      return 86;
    else if (vout >= 1144)      return 87;
    else if (vout >= 1133)      return 88;
    else if (vout >= 1122)      return 89;
    else if (vout >= 1110)      return 90;
    else if (vout >= 1099)      return 91;
    else if (vout >= 1088)      return 92;
    else if (vout >= 1076)      return 93;
    else if (vout >= 1065)      return 94;
    else if (vout >= 1054)      return 95;
    else if (vout >= 1042)      return 96;
    else if (vout >= 1031)      return 97;
    else if (vout >= 1020)      return 98;
    else if (vout >= 1008)      return 99;
    else if (vout >= 997)       return 100;
    else if (vout >= 986)       return 101;
    else if (vout >= 974)       return 102;
    else if (vout >= 963)       return 103;
    else if (vout >= 951)       return 104;
    else if (vout >= 940)       return 105;
    else if (vout >= 929)       return 106;
    else if (vout >= 917)       return 107;
    else if (vout >= 906)       return 108;
    else if (vout >= 895)       return 109;
    else if (vout >= 883)       return 110;
    else if (vout >= 872)       return 111;
    else if (vout >= 860)       return 112;
    else if (vout >= 849)       return 113;
    else if (vout >= 837)       return 114;
    else if (vout >= 826)       return 115;
    else if (vout >= 814)       return 116;
    else if (vout >= 803)       return 117;
    else if (vout >= 791)       return 118;
    else if (vout >= 780)       return 119;
    else if (vout >= 769)       return 120;
    else if (vout >= 757)       return 121;
    else if (vout >= 745)       return 122;
    else if (vout >= 734)       return 123;
    else if (vout >= 722)       return 124;
    else if (vout >= 711)       return 125;
    else if (vout >= 699)       return 126;
    else if (vout >= 688)       return 127;
    else if (vout >= 676)       return 128;
    else if (vout >= 665)       return 129;
    else if (vout >= 653)       return 130;
    else if (vout >= 642)       return 131;
    else if (vout >= 630)       return 132;
    else if (vout >= 618)       return 133;
    else if (vout >= 607)       return 134;
    else if (vout >= 595)       return 135;
    else if (vout >= 584)       return 136;
    else if (vout >= 572)       return 137;
    else if (vout >= 560)       return 138;
    else if (vout >= 549)       return 139;
    else if (vout >= 537)       return 140;
    else if (vout >= 525)       return 141;
    else if (vout >= 514)       return 142;
    else if (vout >= 502)       return 143;
    else if (vout >= 490)       return 144;
    else if (vout >= 479)       return 145;
    else if (vout >= 467)       return 146;
    else if (vout >= 455)       return 147;
    else if (vout >= 443)       return 148;
    else if (vout >= 432)       return 149;
    else if (vout >= 420)       return 150;
    else                        return INT16_MAX;
}

int16_t isis_antenna_get_temperature(void)
{
    uint16_t raw_temp = isis_antenna_get_raw_temperature();

    return isis_antenna_raw_to_temp_c(raw_temp);
}

/** \} End of isis_antenna group */
