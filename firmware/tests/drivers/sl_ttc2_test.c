/*
 * sl_ttc2_test.c
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
 * \brief Unit test of the SpaceLab TTC 2.0 driver.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.12
 * 
 * \date 2021/09/08
 * 
 * \defgroup sl_ttc2_unit_test SpaceLab TTC 2.0
 * \ingroup tests
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include <drivers/i2c/i2c.h>
#include <drivers/sl_ttc2/sl_ttc2.h>

static void sl_ttc2_init_test(void **state)
{
}

static void sl_ttc2_check_device_test(void **state)
{
}

static void sl_ttc2_write_reg_test(void **state)
{
}

static void sl_ttc2_read_reg_test(void **state)
{
}

static void sl_ttc2_read_hk_data_test(void **state)
{
}

static void sl_ttc2_read_device_id_test(void **state)
{
}

static void sl_ttc2_read_hardware_version_test(void **state)
{
}

static void sl_ttc2_read_firmware_version_test(void **state)
{
}

static void sl_ttc2_read_time_counter_test(void **state)
{
}

static void sl_ttc2_read_reset_counter_test(void **state)
{
}

static void sl_ttc2_read_reset_cause_test(void **state)
{
}

static void sl_ttc2_read_voltage_test(void **state)
{
}

static void sl_ttc2_read_current_test(void **state)
{
}

static void sl_ttc2_read_temp_test(void **state)
{
}

static void sl_ttc2_read_last_valid_tc_test(void **state)
{
}

static void sl_ttc2_read_rssi_test(void **state)
{
}

static void sl_ttc2_read_antenna_status_test(void **state)
{
}

static void sl_ttc2_read_antenna_deployment_status_test(void **state)
{
}

static void sl_ttc2_read_antenna_deployment_hibernation_status_test(void **state)
{
}

static void sl_ttc2_read_tx_enable_test(void **state)
{
}

static void sl_ttc2_set_tx_enable_test(void **state)
{
}

static void sl_ttc2_read_pkt_counter_test(void **state)
{
}

static void sl_ttc2_read_fifo_pkts_test(void **state)
{
}

static void sl_ttc2_read_len_rx_pkt_in_fifo_test(void **state)
{
}

static void sl_ttc2_check_pkt_avail_test(void **state)
{
}

static void sl_ttc2_transmit_packet_test(void **state)
{
}

static void sl_ttc2_read_packet_test(void **state)
{
}

int main(void)
{
    const struct CMUnitTest sl_ttc2_tests[] = {
        cmocka_unit_test(sl_ttc2_init_test),
        cmocka_unit_test(sl_ttc2_check_device_test),
        cmocka_unit_test(sl_ttc2_write_reg_test),
        cmocka_unit_test(sl_ttc2_read_reg_test),
        cmocka_unit_test(sl_ttc2_read_hk_data_test),
        cmocka_unit_test(sl_ttc2_read_device_id_test),
        cmocka_unit_test(sl_ttc2_read_hardware_version_test),
        cmocka_unit_test(sl_ttc2_read_firmware_version_test),
        cmocka_unit_test(sl_ttc2_read_time_counter_test),
        cmocka_unit_test(sl_ttc2_read_reset_counter_test),
        cmocka_unit_test(sl_ttc2_read_reset_cause_test),
        cmocka_unit_test(sl_ttc2_read_voltage_test),
        cmocka_unit_test(sl_ttc2_read_current_test),
        cmocka_unit_test(sl_ttc2_read_temp_test),
        cmocka_unit_test(sl_ttc2_read_last_valid_tc_test),
        cmocka_unit_test(sl_ttc2_read_rssi_test),
        cmocka_unit_test(sl_ttc2_read_antenna_status_test),
        cmocka_unit_test(sl_ttc2_read_antenna_deployment_status_test),
        cmocka_unit_test(sl_ttc2_read_antenna_deployment_hibernation_status_test),
        cmocka_unit_test(sl_ttc2_read_tx_enable_test),
        cmocka_unit_test(sl_ttc2_set_tx_enable_test),
        cmocka_unit_test(sl_ttc2_read_pkt_counter_test),
        cmocka_unit_test(sl_ttc2_read_fifo_pkts_test),
        cmocka_unit_test(sl_ttc2_read_len_rx_pkt_in_fifo_test),
        cmocka_unit_test(sl_ttc2_check_pkt_avail_test),
        cmocka_unit_test(sl_ttc2_transmit_packet_test),
        cmocka_unit_test(sl_ttc2_read_packet_test),
    };

    return cmocka_run_group_tests(sl_ttc2_tests, NULL, NULL);
}

/** \} End of sl_ttc2_test group */
