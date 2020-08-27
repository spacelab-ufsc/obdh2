/*
 * eps_data.h
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
 * \brief EPS data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.4.17
 * 
 * \date 2020/07/16
 * 
 * \defgroup eps_data EPS Data
 * \ingroup eps
 * \{
 */

#ifndef EPS_DATA_H_
#define EPS_DATA_H_

#include <stdint.h>

/**
 * \brief Battery cell data.
 */
typedef struct
{
    uint16_t voltage;               /**< Raw voltage. */
    uint32_t temperature;           /**< Raw temperature. */
    uint8_t heater_duty_cycle;      /**< Heater raw duty cycle. */
} eps_bat_cell_t;

/**
 * \brief Battery data.
 */
typedef struct
{
    eps_bat_cell_t cell_0;          /**< Cell 0 data. */
    eps_bat_cell_t cell_1;          /**< Cell 1 data. */
    uint16_t current;               /**< Raw current. */
    uint16_t average_current;       /**< Raw average current. */
    uint16_t accumulated_current;   /**< Raw accumulated current. */
} eps_bat_t;

/**
 * \brief Battery monitor data.
 */
typedef struct
{
    uint16_t temperature;           /**< Raw temperature. */
    uint16_t current;               /**< Raw current. */
    uint16_t charge;                /**< Raw battery charge. */
} eps_bat_monitor_t;

/**
 * \brief Solar panel data.
 */
typedef struct
{
    uint16_t pos_x_current;         /**< Raw +x current. */
    uint16_t neg_x_current;         /**< Raw -x current. */
    uint16_t pos_y_current;         /**< Raw +y current. */
    uint16_t neg_y_current;         /**< Raw -y current. */
    uint16_t pos_z_current;         /**< Raw +z current. */
    uint16_t neg_z_current;         /**< Raw -z current. */
    uint16_t neg_y_pos_x_voltage;   /**< Raw -y +x voltage. */
    uint16_t neg_x_pos_z_voltage;   /**< Raw -x +z voltage. */
    uint16_t neg_z_pos_y_voltage;   /**< Raw -z +y voltage. */
} eps_solar_panel_t;

/**
 * \brief EPS data.
 */
typedef struct
{
    uint32_t timestamp;             /**< Data timestamp (system ticks). */
    uint16_t temperature;           /**< uC raw temperature. */
    eps_bat_t battery;              /**< Battery data. */
    eps_bat_monitor_t bat_monitor;  /**< Battery monitor data. */
    eps_solar_panel_t solar_panel;  /**< Solar panel data. */
    uint16_t main_bus_voltage;      /**< Main bus raw voltage. */
    uint16_t vpanels_voltage;       /**< vpanels raw voltage. */
} eps_data_t;

#endif /* EPS_DATA_H_ */

/** \} End of eps_data group */
