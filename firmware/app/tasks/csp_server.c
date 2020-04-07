/*
 * csp_server.c
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
 * \brief CSP server task implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.7
 * 
 * \date 04/04/2020
 * 
 * \addtogroup csp_server
 * \{
 */

#include <csp/csp.h>

#include <config/config.h>

#include "csp_server.h"
#include "startup.h"

xTaskHandle xTaskCSPServerHandle;

void vTaskCSPServer(void *pvParameters)
{
    /* Wait startup task to finish */
    xEventGroupWaitBits(task_startup_status, TASK_STARTUP_DONE, pdFALSE, pdTRUE, pdMS_TO_TICKS(TASK_CSP_SERVER_INIT_TIMEOUT_MS));

    /* Create socket without any socket options */
    csp_socket_t *sock = csp_socket(CSP_SO_NONE);

    /* Bind all ports to socket */
    csp_bind(sock, CSP_ANY);

    /* Create SDR_CSP_MAX_CONNECTIONS connections backlog queue */
    csp_listen(sock, CONFIG_CSP_MAX_CONN);

    /* Pointer to current connection and packet */
    csp_conn_t *conn;
    csp_packet_t *packet;

    while(1)
    {
        /* Wait for connection until reach timeout */
        if ((conn = csp_accept(sock, CONFIG_CSP_WAIT_CONN_TIMEOUT_MS)) == NULL)
        {
            continue;
        }

        /* Read packets until reach timeout */
        while((packet = csp_read(conn, CONFIG_CSP_READ_PKT_TIMEOUT_MS)) != NULL)
        {
            switch(csp_conn_dport(conn))
            {
                case CONFIG_CSP_PORT:
                    /* TODO: Process packet here */
                default:
                    /* Let the service handler reply pings, buffer use, etc. */
                    csp_service_handler(conn, packet);
                    break;
            }
        }

        /* Close current connection and handle next */
        csp_close(conn);
    }
}

/** \} End of csp_server group */
