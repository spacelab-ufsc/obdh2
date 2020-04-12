/*
 * beacon.c
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
 * \brief Beacon task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.10
 * 
 * \date 27/10/2019
 * 
 * \addtogroup beacon
 * \{
 */

#include <fsat_pkt/fsat_pkt.h>
#include <ngham/ngham.h>
#include <config/config.h>

#include "beacon.h"
#include "startup.h"

xTaskHandle xTaskBeaconHandle;

void vTaskBeacon(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_BEACON_INIT_TIMEOUT_MS));

    /* Delay before the first cycle */
    vTaskDelay(pdMS_TO_TICKS(TASK_BEACON_INITIAL_DELAY_MS));

    while(1)
    {
        TickType_t last_cycle = xTaskGetTickCount();

        /* Beacon data */
        uint8_t data[30] = {0};
        uint16_t data_len = 1;

        fsat_pkt_pl_t beacon_pl;

        /* Packet ID */
        fsat_pkt_add_id(&beacon_pl, CONFIG_PKT_ID_BEACON);

        /* Source callsign */
        fsat_pkt_add_callsign(&beacon_pl, CONFIG_SATELLITE_CALLSIGN);

        /* Payload data */
        fsat_pkt_add_payload(&beacon_pl, data, data_len);

        tx_pkt_t beacon_pkt;

        fsat_pkt_encode(beacon_pl, beacon_pkt.pl, &beacon_pkt.pl_len);

        beacon_pkt.ngham_flags = 0;
        beacon_pkt.priority = PKT_PRIORITY_NORMAL;

        ngham_encode(&beacon_pkt);

        vTaskDelayUntil(&last_cycle, pdMS_TO_TICKS(TASK_BEACON_PERIOD_MS));
    }
}

/** \} End of beacon group */
