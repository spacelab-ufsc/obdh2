/*
 * data_log.c
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
 * \brief Data log task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.40
 * 
 * \date 2021/05/24
 * 
 * \addtogroup data_log
 * \{
 */

#include <stdint.h>

#include <system/sys_log/sys_log.h>
#include <devices/media/media.h>
#include <structs/satellite.h>

#include "data_log.h"
#include "startup.h"

xTaskHandle xTaskDataLogHandle;

/**
 * \brief Computes the CRC16 value of given data sequence (CCITT).
 *
 * \param[in] initial_value is the initial value.
 *
 * \param[in] data is the data sequence to compute the CRC.
 *
 * \param[in] size is the number of bytes of the given data.
 *
 * \return The computed CRC16 value.
 */
uint16_t crc16_ccitt(uint16_t initial_value, uint8_t* data, uint8_t size);

void vTaskDataLog(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_DATA_LOG_INIT_TIMEOUT_MS));

    media_info_t nor_info = media_get_info(MEDIA_NOR);

    uint32_t mem_adr = nor_info.page_size;

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        uint8_t page_buf[256] = {0xFF};
        uint16_t i = 0;

        page_buf[i++] = DATA_LOG_HK_DATA_ID;

        /* Packet length in bytes */
        page_buf[i++] = 170;

        /* OBDH data */
        page_buf[i++] = (sat_data_buf.obdh.timestamp >> 24) & 0xFF;
        page_buf[i++] = (sat_data_buf.obdh.timestamp >> 16) & 0xFF;
        page_buf[i++] = (sat_data_buf.obdh.timestamp >> 8)  & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.timestamp & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.temperature >> 8;
        page_buf[i++] = sat_data_buf.obdh.temperature & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.current >> 8;
        page_buf[i++] = sat_data_buf.obdh.current & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.voltage >> 8;
        page_buf[i++] = sat_data_buf.obdh.voltage & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.last_reset_cause;
        page_buf[i++] = sat_data_buf.obdh.reset_counter >> 8;
        page_buf[i++] = sat_data_buf.obdh.reset_counter & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.last_valid_tc;
        page_buf[i++] = sat_data_buf.obdh.radio.temperature >> 8;
        page_buf[i++] = sat_data_buf.obdh.radio.temperature & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.radio.last_valid_tc_rssi >> 8;
        page_buf[i++] = sat_data_buf.obdh.radio.last_valid_tc_rssi & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.antenna.temperature >> 8;
        page_buf[i++] = sat_data_buf.obdh.antenna.temperature & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.antenna.status >> 8;
        page_buf[i++] = sat_data_buf.obdh.antenna.status & 0xFF;
        page_buf[i++] = (sat_data_buf.obdh.hw_version >> 24) & 0xFF;
        page_buf[i++] = (sat_data_buf.obdh.fw_version >> 16) & 0xFF;
        page_buf[i++] = (sat_data_buf.obdh.fw_version >> 8)  & 0xFF;
        page_buf[i++] = sat_data_buf.obdh.fw_version & 0xFF;

        /* EPS data */
        page_buf[i++] = (sat_data_buf.eps.time_counter >> 24) & 0xFF;
        page_buf[i++] = (sat_data_buf.eps.time_counter >> 16) & 0xFF;
        page_buf[i++] = (sat_data_buf.eps.time_counter >> 8)  & 0xFF;
        page_buf[i++] = sat_data_buf.eps.time_counter & 0xFF;
        page_buf[i++] = sat_data_buf.eps.temperature_uc >> 8;
        page_buf[i++] = sat_data_buf.eps.temperature_uc & 0xFF;
        page_buf[i++] = sat_data_buf.eps.current >> 8;
        page_buf[i++] = sat_data_buf.eps.current & 0xFF;
        page_buf[i++] = sat_data_buf.eps.last_reset_cause;
        page_buf[i++] = sat_data_buf.eps.reset_counter >> 8;
        page_buf[i++] = sat_data_buf.eps.reset_counter & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_voltage_my_px >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_voltage_my_px & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_voltage_mx_pz >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_voltage_mx_pz & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_voltage_mz_py >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_voltage_mz_py & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_my >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_my & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_py >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_py & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_mx >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_mx & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_px >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_px & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_mz >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_mz & 0xFF;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_pz >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_current_pz & 0xFF;
        page_buf[i++] = sat_data_buf.eps.mppt_1_duty_cycle;
        page_buf[i++] = sat_data_buf.eps.mppt_2_duty_cycle;
        page_buf[i++] = sat_data_buf.eps.mppt_3_duty_cycle;
        page_buf[i++] = sat_data_buf.eps.solar_panel_output_voltage >> 8;
        page_buf[i++] = sat_data_buf.eps.solar_panel_output_voltage & 0xFF;
        page_buf[i++] = sat_data_buf.eps.main_power_bus_voltage >> 8;
        page_buf[i++] = sat_data_buf.eps.main_power_bus_voltage & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_0_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_0_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_1_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_1_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_2_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_2_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_3_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_3_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_4_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_4_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_5_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_5_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rtd_6_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.rtd_6_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_voltage >> 8;
        page_buf[i++] = sat_data_buf.eps.battery_voltage & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_current >> 8;
        page_buf[i++] = sat_data_buf.eps.battery_current & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_average_current >> 8;
        page_buf[i++] = sat_data_buf.eps.battery_average_current & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_acc_current >> 8;
        page_buf[i++] = sat_data_buf.eps.battery_acc_current & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_charge >> 8;
        page_buf[i++] = sat_data_buf.eps.battery_charge & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_monitor_temperature >> 8;
        page_buf[i++] = sat_data_buf.eps.battery_monitor_temperature & 0xFF;
        page_buf[i++] = sat_data_buf.eps.battery_monitor_status;
        page_buf[i++] = sat_data_buf.eps.battery_monitor_protection;
        page_buf[i++] = sat_data_buf.eps.battery_monitor_cycle_counter;
        page_buf[i++] = sat_data_buf.eps.raac >> 8;
        page_buf[i++] = sat_data_buf.eps.raac & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rsac >> 8;
        page_buf[i++] = sat_data_buf.eps.rsac & 0xFF;
        page_buf[i++] = sat_data_buf.eps.rarc;
        page_buf[i++] = sat_data_buf.eps.rsrc;
        page_buf[i++] = sat_data_buf.eps.battery_heater_1_duty_cycle;
        page_buf[i++] = sat_data_buf.eps.battery_heater_2_duty_cycle;
        page_buf[i++] = sat_data_buf.eps.mppt_1_mode;
        page_buf[i++] = sat_data_buf.eps.mppt_2_mode;
        page_buf[i++] = sat_data_buf.eps.mppt_3_mode;
        page_buf[i++] = sat_data_buf.eps.battery_heater_1_mode;
        page_buf[i++] = sat_data_buf.eps.battery_heater_2_mode;

        /* TTC 0 data */
        page_buf[i++] = (sat_data_buf.ttc_0.time_counter >> 24) & 0xFF;
        page_buf[i++] = (sat_data_buf.ttc_0.time_counter >> 16) & 0xFF;
        page_buf[i++] = (sat_data_buf.ttc_0.time_counter >> 8)  & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.time_counter & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.reset_counter >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.reset_counter & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.last_reset_cause;
        page_buf[i++] = sat_data_buf.ttc_0.voltage_mcu >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.voltage_mcu & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.current_mcu >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.current_mcu & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.temperature_mcu >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.temperature_mcu & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.voltage_radio >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.voltage_radio & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.current_radio >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.current_radio & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.temperature_radio >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.temperature_radio & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.last_valid_tc;
        page_buf[i++] = sat_data_buf.ttc_0.rssi_last_valid_tc >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.rssi_last_valid_tc & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.temperature_antenna >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.temperature_antenna & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.antenna_status >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.antenna_status & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.deployment_status;
        page_buf[i++] = sat_data_buf.ttc_0.hibernation_status;
        page_buf[i++] = sat_data_buf.ttc_0.tx_packet_counter >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.tx_packet_counter & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_0.rx_packet_counter >> 8;
        page_buf[i++] = sat_data_buf.ttc_0.rx_packet_counter & 0xFF;

        /* TTC 1 data */
        page_buf[i++] = (sat_data_buf.ttc_1.time_counter >> 24) & 0xFF;
        page_buf[i++] = (sat_data_buf.ttc_1.time_counter >> 16) & 0xFF;
        page_buf[i++] = (sat_data_buf.ttc_1.time_counter >> 8)  & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.time_counter & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.reset_counter >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.reset_counter & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.last_reset_cause;
        page_buf[i++] = sat_data_buf.ttc_1.voltage_mcu >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.voltage_mcu & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.current_mcu >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.current_mcu & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.temperature_mcu >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.temperature_mcu & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.voltage_radio >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.voltage_radio & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.current_radio >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.current_radio & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.temperature_radio >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.temperature_radio & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.last_valid_tc;
        page_buf[i++] = sat_data_buf.ttc_1.rssi_last_valid_tc >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.rssi_last_valid_tc & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.temperature_antenna >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.temperature_antenna & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.antenna_status >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.antenna_status & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.deployment_status;
        page_buf[i++] = sat_data_buf.ttc_1.hibernation_status;
        page_buf[i++] = sat_data_buf.ttc_1.tx_packet_counter >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.tx_packet_counter & 0xFF;
        page_buf[i++] = sat_data_buf.ttc_1.rx_packet_counter >> 8;
        page_buf[i++] = sat_data_buf.ttc_1.rx_packet_counter & 0xFF;

        /* CRC */
        uint16_t crc = crc16_ccitt(0, page_buf, i);

        page_buf[i++] = crc >> 8;
        page_buf[i++] = crc & 0xFF;

        if (media_write(MEDIA_NOR, mem_adr, page_buf, nor_info.page_size) != 0)
        {
            sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_DATA_LOG_NAME, "Error writing data to the NOR memory!");
            sys_log_new_line();
        }

        mem_adr += nor_info.page_size;

        if (mem_adr > nor_info.size)
        {
            mem_adr = nor_info.page_size;
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_DATA_LOG_PERIOD_MS));
    }
}

uint16_t crc16_ccitt(uint16_t initial_value, uint8_t* data, uint8_t size)
{
    uint8_t x;
    uint16_t crc = initial_value;

    while(size--)
    {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }

    return crc;
}

/** \} End of file_system group */
