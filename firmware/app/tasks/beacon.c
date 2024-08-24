/*
 * beacon.c
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
 * \brief Beacon task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 * 
 * \version 0.10.19
 * 
 * \date 2019/10/27
 * 
 * \addtogroup beacon
 * \{
 */

#include <config/config.h>
#include <fsat_pkt/fsat_pkt.h>

#include <system/sys_log/sys_log.h>
#include <system/system.h>

#include <structs/satellite.h>

#include <devices/ttc/ttc.h>

#include "beacon.h"
#include "startup.h"

xTaskHandle xTaskBeaconHandle;

void vTaskBeacon(void)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_BEACON_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_BEACON_INITIAL_DELAY_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        fsat_pkt_pl_t beacon_pl = {0};

        /* Packet ID */
        fsat_pkt_add_id(&beacon_pl, CONFIG_PKT_ID_DOWNLINK_GENERAL_TELEMETRY);

        /* Source callsign */
        fsat_pkt_add_callsign(&beacon_pl, CONFIG_SATELLITE_CALLSIGN);

        uint32_t timestamp = system_get_time();

        /* Payload data */
        beacon_pl.payload[0] = (timestamp >> 24) & 0xFFU;
        beacon_pl.payload[1] = (timestamp >> 16) & 0xFFU;
        beacon_pl.payload[2] = (timestamp >> 8)  & 0xFFU;
        beacon_pl.payload[3] = timestamp & 0xFFU;
        beacon_pl.payload[4] = sat_data_buf.obdh.data.temperature >> 8;
        beacon_pl.payload[5] = sat_data_buf.obdh.data.temperature & 0xFF;
        beacon_pl.payload[6] = sat_data_buf.obdh.data.current >> 8;
        beacon_pl.payload[7] = sat_data_buf.obdh.data.current & 0xFF;
        beacon_pl.payload[8] = sat_data_buf.obdh.data.voltage >> 8;
        beacon_pl.payload[9] = sat_data_buf.obdh.data.voltage & 0xFF;
        beacon_pl.payload[10] = sat_data_buf.obdh.data.last_reset_cause;
        beacon_pl.payload[11] = sat_data_buf.obdh.data.reset_counter >> 8;
        beacon_pl.payload[12] = sat_data_buf.obdh.data.reset_counter & 0xFF;
        beacon_pl.payload[13] = sat_data_buf.obdh.data.last_valid_tc;
        beacon_pl.payload[14] = sat_data_buf.ttc_1.data.temperature_radio >> 8;
        beacon_pl.payload[15] = sat_data_buf.ttc_1.data.temperature_radio & 0xFF;
        beacon_pl.payload[16] = sat_data_buf.ttc_1.data.rssi_last_valid_tc >> 8;
        beacon_pl.payload[17] = sat_data_buf.ttc_1.data.rssi_last_valid_tc & 0xFF;
        beacon_pl.payload[18] = (sat_data_buf.antenna.data.temperature >> 8) & 0xFF;
        beacon_pl.payload[19] = sat_data_buf.antenna.data.temperature & 0xFF;
        beacon_pl.payload[20] = (sat_data_buf.antenna.data.status.code >> 8) & 0xFF;
        beacon_pl.payload[21] = sat_data_buf.antenna.data.status.code & 0xFF;
        beacon_pl.payload[22] = sat_data_buf.eps.data.temperature_uc >> 8;
        beacon_pl.payload[23] = sat_data_buf.eps.data.temperature_uc & 0xFF;
        beacon_pl.payload[24] = sat_data_buf.eps.data.current >> 8;
        beacon_pl.payload[25] = sat_data_buf.eps.data.current & 0xFF;
        beacon_pl.payload[26] = sat_data_buf.eps.data.last_reset_cause;
        beacon_pl.payload[27] = sat_data_buf.eps.data.reset_counter >> 8;
        beacon_pl.payload[28] = sat_data_buf.eps.data.reset_counter & 0xFF;
        beacon_pl.payload[29] = sat_data_buf.eps.data.solar_panel_voltage_my_px >> 8;
        beacon_pl.payload[30] = sat_data_buf.eps.data.solar_panel_voltage_my_px & 0xFF;
        beacon_pl.payload[31] = sat_data_buf.eps.data.solar_panel_voltage_mx_pz >> 8;
        beacon_pl.payload[32] = sat_data_buf.eps.data.solar_panel_voltage_mx_pz & 0xFF;
        beacon_pl.payload[33] = sat_data_buf.eps.data.solar_panel_voltage_mz_py >> 8;
        beacon_pl.payload[34] = sat_data_buf.eps.data.solar_panel_voltage_mz_py & 0xFF;
        beacon_pl.payload[35] = sat_data_buf.eps.data.solar_panel_current_my >> 8;
        beacon_pl.payload[36] = sat_data_buf.eps.data.solar_panel_current_my & 0xFF;
        beacon_pl.payload[37] = sat_data_buf.eps.data.solar_panel_current_py >> 8;
        beacon_pl.payload[38] = sat_data_buf.eps.data.solar_panel_current_py & 0xFF;
        beacon_pl.payload[39] = sat_data_buf.eps.data.solar_panel_current_mx >> 8;
        beacon_pl.payload[40] = sat_data_buf.eps.data.solar_panel_current_mx & 0xFF;
        beacon_pl.payload[41] = sat_data_buf.eps.data.solar_panel_current_px >> 8;
        beacon_pl.payload[42] = sat_data_buf.eps.data.solar_panel_current_px & 0xFF;
        beacon_pl.payload[43] = sat_data_buf.eps.data.solar_panel_current_mz >> 8;
        beacon_pl.payload[44] = sat_data_buf.eps.data.solar_panel_current_mz & 0xFF;
        beacon_pl.payload[45] = sat_data_buf.eps.data.solar_panel_current_pz >> 8;
        beacon_pl.payload[46] = sat_data_buf.eps.data.solar_panel_current_pz & 0xFF;
        beacon_pl.payload[47] = sat_data_buf.eps.data.mppt_1_duty_cycle;
        beacon_pl.payload[48] = sat_data_buf.eps.data.mppt_2_duty_cycle;
        beacon_pl.payload[49] = sat_data_buf.eps.data.mppt_3_duty_cycle;
        beacon_pl.payload[50] = sat_data_buf.eps.data.main_power_bus_voltage >> 8;
        beacon_pl.payload[51] = sat_data_buf.eps.data.main_power_bus_voltage & 0xFF;
        beacon_pl.payload[52] = sat_data_buf.eps.data.battery_voltage >> 8;
        beacon_pl.payload[53] = sat_data_buf.eps.data.battery_voltage & 0xFF;
        beacon_pl.payload[54] = sat_data_buf.eps.data.battery_current >> 8;
        beacon_pl.payload[55] = sat_data_buf.eps.data.battery_current & 0xFF;
        beacon_pl.payload[56] = sat_data_buf.eps.data.battery_average_current >> 8;
        beacon_pl.payload[57] = sat_data_buf.eps.data.battery_average_current & 0xFF;
        beacon_pl.payload[58] = sat_data_buf.eps.data.battery_acc_current >> 8;
        beacon_pl.payload[59] = sat_data_buf.eps.data.battery_acc_current & 0xFF;
        beacon_pl.payload[60] = sat_data_buf.eps.data.battery_charge >> 8;
        beacon_pl.payload[61] = sat_data_buf.eps.data.battery_charge & 0xFF;
        beacon_pl.payload[62] = sat_data_buf.eps.data.battery_monitor_temperature >> 8;
        beacon_pl.payload[63] = sat_data_buf.eps.data.battery_monitor_temperature & 0xFF;
        beacon_pl.payload[64] = sat_data_buf.eps.data.battery_heater_1_duty_cycle;
        beacon_pl.payload[65] = sat_data_buf.eps.data.battery_heater_2_duty_cycle;
        beacon_pl.payload[66] = sat_data_buf.obdh.data.main_edc;
        beacon_pl.payload[67] = sat_data_buf.state.active_payload[0];
        beacon_pl.payload[68] = sat_data_buf.state.active_payload[1];

        beacon_pl.length = 69U;

        uint8_t beacon_pl_raw[220] = {0};
        uint16_t beacon_pl_raw_len = 0;

        fsat_pkt_encode(&beacon_pl, beacon_pl_raw, &beacon_pl_raw_len);

        if (sat_data_buf.obdh.data.mode != OBDH_MODE_HIBERNATION)
        {
            if (ttc_send(TTC_1, beacon_pl_raw, beacon_pl_raw_len) != 0)
            {
                sys_log_print_event_from_module(SYS_LOG_ERROR, TASK_BEACON_NAME, "Error transmiting the beacon packet!");
                sys_log_new_line();
            }
        }

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_BEACON_PERIOD_MS));
    }
}

/** \} End of beacon group */
