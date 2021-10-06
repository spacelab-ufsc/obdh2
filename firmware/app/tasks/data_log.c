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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Data log task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.47
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

void vTaskDataLog(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_DATA_LOG_INIT_TIMEOUT_MS));

    media_info_t nor_info = media_get_info(MEDIA_NOR);

    uint32_t mem_adr = nor_info.page_size;

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        uint8_t page_buf[256] = {0};
        uint16_t i = 0;

        page_buf[0] = DATA_LOG_HK_DATA_ID;

        /* Packet length in bytes */
        page_buf[1] = 170U;

        /* OBDH data */
        page_buf[2] = (sat_data_buf.obdh.timestamp >> 24) & 0xFF;
        page_buf[3] = (sat_data_buf.obdh.timestamp >> 16) & 0xFF;
        page_buf[4] = (sat_data_buf.obdh.timestamp >> 8)  & 0xFF;
        page_buf[5] = sat_data_buf.obdh.timestamp & 0xFF;
        page_buf[6] = sat_data_buf.obdh.temperature >> 8;
        page_buf[7] = sat_data_buf.obdh.temperature & 0xFF;
        page_buf[8] = sat_data_buf.obdh.current >> 8;
        page_buf[9] = sat_data_buf.obdh.current & 0xFF;
        page_buf[10] = sat_data_buf.obdh.voltage >> 8;
        page_buf[11] = sat_data_buf.obdh.voltage & 0xFF;
        page_buf[12] = sat_data_buf.obdh.last_reset_cause;
        page_buf[13] = sat_data_buf.obdh.reset_counter >> 8;
        page_buf[14] = sat_data_buf.obdh.reset_counter & 0xFF;
        page_buf[15] = sat_data_buf.obdh.last_valid_tc;
        page_buf[16] = sat_data_buf.obdh.radio.temperature >> 8;
        page_buf[17] = sat_data_buf.obdh.radio.temperature & 0xFF;
        page_buf[18] = sat_data_buf.obdh.radio.last_valid_tc_rssi >> 8;
        page_buf[19] = sat_data_buf.obdh.radio.last_valid_tc_rssi & 0xFF;
        page_buf[20] = sat_data_buf.obdh.antenna.temperature >> 8;
        page_buf[21] = sat_data_buf.obdh.antenna.temperature & 0xFF;
        page_buf[22] = sat_data_buf.obdh.antenna.status >> 8;
        page_buf[23] = sat_data_buf.obdh.antenna.status & 0xFF;
        page_buf[24] = (sat_data_buf.obdh.hw_version >> 24) & 0xFF;
        page_buf[25] = (sat_data_buf.obdh.fw_version >> 16) & 0xFF;
        page_buf[26] = (sat_data_buf.obdh.fw_version >> 8)  & 0xFF;
        page_buf[27] = sat_data_buf.obdh.fw_version & 0xFF;

        /* EPS data */
        page_buf[28] = (sat_data_buf.eps.time_counter >> 24) & 0xFF;
        page_buf[29] = (sat_data_buf.eps.time_counter >> 16) & 0xFF;
        page_buf[30] = (sat_data_buf.eps.time_counter >> 8)  & 0xFF;
        page_buf[31] = sat_data_buf.eps.time_counter & 0xFF;
        page_buf[32] = sat_data_buf.eps.temperature_uc >> 8;
        page_buf[33] = sat_data_buf.eps.temperature_uc & 0xFF;
        page_buf[34] = sat_data_buf.eps.current >> 8;
        page_buf[35] = sat_data_buf.eps.current & 0xFF;
        page_buf[36] = sat_data_buf.eps.last_reset_cause;
        page_buf[37] = sat_data_buf.eps.reset_counter >> 8;
        page_buf[38] = sat_data_buf.eps.reset_counter & 0xFF;
        page_buf[39] = sat_data_buf.eps.solar_panel_voltage_my_px >> 8;
        page_buf[40] = sat_data_buf.eps.solar_panel_voltage_my_px & 0xFF;
        page_buf[41] = sat_data_buf.eps.solar_panel_voltage_mx_pz >> 8;
        page_buf[42] = sat_data_buf.eps.solar_panel_voltage_mx_pz & 0xFF;
        page_buf[43] = sat_data_buf.eps.solar_panel_voltage_mz_py >> 8;
        page_buf[44] = sat_data_buf.eps.solar_panel_voltage_mz_py & 0xFF;
        page_buf[45] = sat_data_buf.eps.solar_panel_current_my >> 8;
        page_buf[46] = sat_data_buf.eps.solar_panel_current_my & 0xFF;
        page_buf[47] = sat_data_buf.eps.solar_panel_current_py >> 8;
        page_buf[48] = sat_data_buf.eps.solar_panel_current_py & 0xFF;
        page_buf[49] = sat_data_buf.eps.solar_panel_current_mx >> 8;
        page_buf[50] = sat_data_buf.eps.solar_panel_current_mx & 0xFF;
        page_buf[51] = sat_data_buf.eps.solar_panel_current_px >> 8;
        page_buf[52] = sat_data_buf.eps.solar_panel_current_px & 0xFF;
        page_buf[53] = sat_data_buf.eps.solar_panel_current_mz >> 8;
        page_buf[54] = sat_data_buf.eps.solar_panel_current_mz & 0xFF;
        page_buf[55] = sat_data_buf.eps.solar_panel_current_pz >> 8;
        page_buf[56] = sat_data_buf.eps.solar_panel_current_pz & 0xFF;
        page_buf[57] = sat_data_buf.eps.mppt_1_duty_cycle;
        page_buf[58] = sat_data_buf.eps.mppt_2_duty_cycle;
        page_buf[59] = sat_data_buf.eps.mppt_3_duty_cycle;
        page_buf[60] = sat_data_buf.eps.solar_panel_output_voltage >> 8;
        page_buf[61] = sat_data_buf.eps.solar_panel_output_voltage & 0xFF;
        page_buf[62] = sat_data_buf.eps.main_power_bus_voltage >> 8;
        page_buf[63] = sat_data_buf.eps.main_power_bus_voltage & 0xFF;
        page_buf[64] = sat_data_buf.eps.rtd_0_temperature >> 8;
        page_buf[65] = sat_data_buf.eps.rtd_0_temperature & 0xFF;
        page_buf[66] = sat_data_buf.eps.rtd_1_temperature >> 8;
        page_buf[67] = sat_data_buf.eps.rtd_1_temperature & 0xFF;
        page_buf[68] = sat_data_buf.eps.rtd_2_temperature >> 8;
        page_buf[69] = sat_data_buf.eps.rtd_2_temperature & 0xFF;
        page_buf[70] = sat_data_buf.eps.rtd_3_temperature >> 8;
        page_buf[71] = sat_data_buf.eps.rtd_3_temperature & 0xFF;
        page_buf[72] = sat_data_buf.eps.rtd_4_temperature >> 8;
        page_buf[73] = sat_data_buf.eps.rtd_4_temperature & 0xFF;
        page_buf[74] = sat_data_buf.eps.rtd_5_temperature >> 8;
        page_buf[75] = sat_data_buf.eps.rtd_5_temperature & 0xFF;
        page_buf[76] = sat_data_buf.eps.rtd_6_temperature >> 8;
        page_buf[77] = sat_data_buf.eps.rtd_6_temperature & 0xFF;
        page_buf[78] = sat_data_buf.eps.battery_voltage >> 8;
        page_buf[79] = sat_data_buf.eps.battery_voltage & 0xFF;
        page_buf[80] = sat_data_buf.eps.battery_current >> 8;
        page_buf[81] = sat_data_buf.eps.battery_current & 0xFF;
        page_buf[82] = sat_data_buf.eps.battery_average_current >> 8;
        page_buf[83] = sat_data_buf.eps.battery_average_current & 0xFF;
        page_buf[84] = sat_data_buf.eps.battery_acc_current >> 8;
        page_buf[85] = sat_data_buf.eps.battery_acc_current & 0xFF;
        page_buf[86] = sat_data_buf.eps.battery_charge >> 8;
        page_buf[87] = sat_data_buf.eps.battery_charge & 0xFF;
        page_buf[88] = sat_data_buf.eps.battery_monitor_temperature >> 8;
        page_buf[89] = sat_data_buf.eps.battery_monitor_temperature & 0xFF;
        page_buf[90] = sat_data_buf.eps.battery_monitor_status;
        page_buf[91] = sat_data_buf.eps.battery_monitor_protection;
        page_buf[92] = sat_data_buf.eps.battery_monitor_cycle_counter;
        page_buf[93] = sat_data_buf.eps.raac >> 8;
        page_buf[94] = sat_data_buf.eps.raac & 0xFF;
        page_buf[95] = sat_data_buf.eps.rsac >> 8;
        page_buf[96] = sat_data_buf.eps.rsac & 0xFF;
        page_buf[97] = sat_data_buf.eps.rarc;
        page_buf[98] = sat_data_buf.eps.rsrc;
        page_buf[99] = sat_data_buf.eps.battery_heater_1_duty_cycle;
        page_buf[100] = sat_data_buf.eps.battery_heater_2_duty_cycle;
        page_buf[101] = sat_data_buf.eps.mppt_1_mode;
        page_buf[102] = sat_data_buf.eps.mppt_2_mode;
        page_buf[103] = sat_data_buf.eps.mppt_3_mode;
        page_buf[104] = sat_data_buf.eps.battery_heater_1_mode;
        page_buf[105] = sat_data_buf.eps.battery_heater_2_mode;

        /* TTC 0 data */
        page_buf[106] = (sat_data_buf.ttc_0.time_counter >> 24) & 0xFF;
        page_buf[107] = (sat_data_buf.ttc_0.time_counter >> 16) & 0xFF;
        page_buf[108] = (sat_data_buf.ttc_0.time_counter >> 8)  & 0xFF;
        page_buf[109] = sat_data_buf.ttc_0.time_counter & 0xFF;
        page_buf[110] = sat_data_buf.ttc_0.reset_counter >> 8;
        page_buf[111] = sat_data_buf.ttc_0.reset_counter & 0xFF;
        page_buf[112] = sat_data_buf.ttc_0.last_reset_cause;
        page_buf[113] = sat_data_buf.ttc_0.voltage_mcu >> 8;
        page_buf[114] = sat_data_buf.ttc_0.voltage_mcu & 0xFF;
        page_buf[115] = sat_data_buf.ttc_0.current_mcu >> 8;
        page_buf[116] = sat_data_buf.ttc_0.current_mcu & 0xFF;
        page_buf[117] = sat_data_buf.ttc_0.temperature_mcu >> 8;
        page_buf[118] = sat_data_buf.ttc_0.temperature_mcu & 0xFF;
        page_buf[119] = sat_data_buf.ttc_0.voltage_radio >> 8;
        page_buf[120] = sat_data_buf.ttc_0.voltage_radio & 0xFF;
        page_buf[121] = sat_data_buf.ttc_0.current_radio >> 8;
        page_buf[122] = sat_data_buf.ttc_0.current_radio & 0xFF;
        page_buf[123] = sat_data_buf.ttc_0.temperature_radio >> 8;
        page_buf[124] = sat_data_buf.ttc_0.temperature_radio & 0xFF;
        page_buf[125] = sat_data_buf.ttc_0.last_valid_tc;
        page_buf[126] = sat_data_buf.ttc_0.rssi_last_valid_tc >> 8;
        page_buf[127] = sat_data_buf.ttc_0.rssi_last_valid_tc & 0xFF;
        page_buf[128] = sat_data_buf.ttc_0.temperature_antenna >> 8;
        page_buf[129] = sat_data_buf.ttc_0.temperature_antenna & 0xFF;
        page_buf[130] = sat_data_buf.ttc_0.antenna_status >> 8;
        page_buf[131] = sat_data_buf.ttc_0.antenna_status & 0xFF;
        page_buf[132] = sat_data_buf.ttc_0.deployment_status;
        page_buf[133] = sat_data_buf.ttc_0.hibernation_status;
        page_buf[134] = sat_data_buf.ttc_0.tx_packet_counter >> 8;
        page_buf[135] = sat_data_buf.ttc_0.tx_packet_counter & 0xFF;
        page_buf[136] = sat_data_buf.ttc_0.rx_packet_counter >> 8;
        page_buf[137] = sat_data_buf.ttc_0.rx_packet_counter & 0xFF;

        /* TTC 1 data */
        page_buf[138] = (sat_data_buf.ttc_1.time_counter >> 24) & 0xFF;
        page_buf[139] = (sat_data_buf.ttc_1.time_counter >> 16) & 0xFF;
        page_buf[140] = (sat_data_buf.ttc_1.time_counter >> 8)  & 0xFF;
        page_buf[141] = sat_data_buf.ttc_1.time_counter & 0xFF;
        page_buf[142] = sat_data_buf.ttc_1.reset_counter >> 8;
        page_buf[143] = sat_data_buf.ttc_1.reset_counter & 0xFF;
        page_buf[144] = sat_data_buf.ttc_1.last_reset_cause;
        page_buf[145] = sat_data_buf.ttc_1.voltage_mcu >> 8;
        page_buf[146] = sat_data_buf.ttc_1.voltage_mcu & 0xFF;
        page_buf[147] = sat_data_buf.ttc_1.current_mcu >> 8;
        page_buf[148] = sat_data_buf.ttc_1.current_mcu & 0xFF;
        page_buf[149] = sat_data_buf.ttc_1.temperature_mcu >> 8;
        page_buf[150] = sat_data_buf.ttc_1.temperature_mcu & 0xFF;
        page_buf[151] = sat_data_buf.ttc_1.voltage_radio >> 8;
        page_buf[152] = sat_data_buf.ttc_1.voltage_radio & 0xFF;
        page_buf[153] = sat_data_buf.ttc_1.current_radio >> 8;
        page_buf[154] = sat_data_buf.ttc_1.current_radio & 0xFF;
        page_buf[155] = sat_data_buf.ttc_1.temperature_radio >> 8;
        page_buf[156] = sat_data_buf.ttc_1.temperature_radio & 0xFF;
        page_buf[157] = sat_data_buf.ttc_1.last_valid_tc;
        page_buf[158] = sat_data_buf.ttc_1.rssi_last_valid_tc >> 8;
        page_buf[159] = sat_data_buf.ttc_1.rssi_last_valid_tc & 0xFF;
        page_buf[160] = sat_data_buf.ttc_1.temperature_antenna >> 8;
        page_buf[161] = sat_data_buf.ttc_1.temperature_antenna & 0xFF;
        page_buf[162] = sat_data_buf.ttc_1.antenna_status >> 8;
        page_buf[163] = sat_data_buf.ttc_1.antenna_status & 0xFF;
        page_buf[164] = sat_data_buf.ttc_1.deployment_status;
        page_buf[165] = sat_data_buf.ttc_1.hibernation_status;
        page_buf[166] = sat_data_buf.ttc_1.tx_packet_counter >> 8;
        page_buf[167] = sat_data_buf.ttc_1.tx_packet_counter & 0xFF;
        page_buf[168] = sat_data_buf.ttc_1.rx_packet_counter >> 8;
        page_buf[169] = sat_data_buf.ttc_1.rx_packet_counter & 0xFF;

        /* CRC */
        uint16_t crc = crc16_ccitt(0, page_buf, i);

        page_buf[170] = crc >> 8;
        page_buf[171] = crc & 0xFFU;

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

    uint8_t i = 0;
    for(i = 0; i < size; i++)
    {
        x = (crc >> 8) ^ data[i];
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)x << 12) ^ ((uint16_t)x << 5) ^ (uint16_t)x;
    }

    return crc;
}

/** \} End of file_system group */
