/*
 * sl_reps.h
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
 * \brief SpaceLab REPS driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/11/12
 * 
 * \defgroup sl_reps SpaceLab REPS
 * \ingroup drivers
 * \{
 */

#ifndef SL_REPS_H_
#define SL_REPS_H_

#include <stdint.h>
#include <drivers/gpio/gpio.h>
#include <drivers/adc/adc.h>

#define SL_REPS_MODULE_NAME                     "SpaceLab REPS"

/**
 * \brief Selection signal for the AMUX_OUT housekeeping MUX. Same signal is used to place the error bit in the error array in function sl_reps_read_hk_data.
 */
#define REPS_SELSIG_CUR_VBUS_BK5V0          (0b0000)
#define REPS_SELSIG_CUR_VPANELS             (0b0001)
#define REPS_SELSIG_VOL_VPANELS             (0b0010)
#define REPS_SELSIG_VOL_VIN                 (0b0011)
#define REPS_SELSIG_CUR_VBAT                (0b0100)
#define REPS_SELSIG_CUR_VBUS_BK3V3          (0b0101)
#define REPS_SELSIG_CUR_VIN                 (0b0110)
#define REPS_SELSIG_VOL_VBUS                (0b0111)
#define REPS_SELSIG_CUR_3v3_OBDH            (0b1000)
#define REPS_SELSIG_CUR_5v0_PAYLOAD         (0b1001)
#define REPS_SELSIG_CUR_5v0_RADIO_BEACON    (0b1010)
#define REPS_SELSIG_CUR_3v3_BEACON          (0b1011)
#define REPS_SELSIG_CUR_5v0_RADIO_MAIN      (0b1100)
#define REPS_SELSIG_VOL_5v0                 (0b1101)
#define REPS_SELSIG_CUR_3v3_AntDep          (0b1110)
#define REPS_SELSIG_VOL_3v3                 (0b1111)

#define REPS_MUXOUT_SIG_WAIT_MS             (0x01)

/**
 * \brief Reps current type.
 */
typedef uint16_t sl_reps_current_t;

/**
 * \brief Reps voltage type.
 */
typedef uint16_t sl_reps_voltage_t;

typedef struct sl_reps_config_t{
    gpio_pin_t radio_main_en_pin;       /**< radio main enable GPIO. */
    gpio_pin_t radio_beacon_en_pin;     /**< radio beacon enable GPIO. */
    gpio_pin_t payload_en_pin;          /**< payload enable GPIO. */
    gpio_pin_t antdep_en_pin;           /**< antenna deploy enable GPIO. */
    gpio_pin_t beacon_en_pin;           /**< beacon enable GPIO. */
    gpio_pin_t obdh_en_pin;             /**< OBDH enable GPIO (???). */
    gpio_pin_t bc_vfb;                  /**< ??? */
    gpio_pin_t bc_a_ts;                 /**< ??? */
    gpio_pin_t bc_b_ts;                 /**< ??? */
    gpio_pin_t en_3v3_a_pin;            /**< 3v3 a enable GPIO. */
    gpio_pin_t en_3v3_b_pin;            /**< 3v3 b enable GPIO. */
    gpio_pin_t en_5v0_a_pin;            /**< 5v0 a enable GPIO. */
    gpio_pin_t en_5v0_b_pin;            /**< 5v0 b enable GPIO. */
    gpio_pin_t bc_a_en;                 /**< ??? */
    gpio_pin_t bc_b_en;                 /**< ??? */
    gpio_pin_t hkmux_sel_pins[4];       /**< mux selection signals for housekeeping data. */
    adc_port_t hkmux_out_adc_port;      /**< mux output for housekeeping data. */
}sl_reps_config_t;

typedef struct sl_reps_hk_data_t{
    sl_reps_current_t   current_vbus_bk3v3;         /**< 3v3 current on vbus. */   
    sl_reps_current_t   current_vbus_bk5v0;         /**< 5v5 current on vbus. */
    sl_reps_current_t   current_vpanels;            /**< current on solar panels. */
    sl_reps_current_t   current_vbat;               /**< current on batteries. */
    sl_reps_current_t   current_vin;                /**< current on 'in' channel. */
    sl_reps_current_t   current_3v3_obdh;           /**< 3v3 current on obdh. */
    sl_reps_current_t   current_3v3_antdep;         /**< 3v3 current on antenna deploy. */
    sl_reps_current_t   current_3v3_beacon;         /**< 3v3 current on beacon. */
    sl_reps_current_t   current_5v0_payload;        /**< 5v0 current on payload. */
    sl_reps_current_t   current_5v0_radio_beacon;   /**< 5v0 current on radio beacon. */
    sl_reps_current_t   current_5v0_radio_main;     /**< 5v0 current on radio main. */

    sl_reps_voltage_t   voltage_vpanels;            /**< voltage om solar pannels. */
    sl_reps_voltage_t   voltage_vin;                /**< voltage on 'in' channel. */
    sl_reps_voltage_t   voltage_vbus;               /**< voltage on vbus. */
    sl_reps_voltage_t   voltage_3v3;                /**< 3v3 voltage. */
    sl_reps_voltage_t   voltage_5v0;                /**< 5v0 voltage. */
}sl_reps_hk_data_t;


/**
* \brief Populates the sl_reps_hk_data with housekeeping data.
*
* \param config configuration data.
*
* \param data buffer to be written to
*
* \returns error code 
*/
int sl_reps_read_hk_data(const sl_reps_config_t config, sl_reps_hk_data_t *data);

int sl_reps_read_cur_vbus_bk3v3(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_vbus_bk5v0(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_vpanels(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_vbat(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_vin(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_3v3_obdh(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_3v3_antdep(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_3v3_beacon(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_5v0_payload(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_5v0_radio_beacon(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_cur_5v0_radio_main(const sl_reps_config_t config, sl_reps_current_t *data);

int sl_reps_read_vol_vpanels(const sl_reps_config_t config, sl_reps_voltage_t *data);

int sl_reps_read_vol_vin(const sl_reps_config_t config, sl_reps_voltage_t *data);

int sl_reps_read_vol_vbus(const sl_reps_config_t config, sl_reps_voltage_t *data);

int sl_reps_read_vol_3v3(const sl_reps_config_t config, sl_reps_voltage_t *data);

int sl_reps_read_vol_5v0(const sl_reps_config_t config, sl_reps_voltage_t *data);

void sl_reps_delay_ms(uint32_t ms);

#endif /* SL_REPS_H_ */

/** \} End of sl_eeps group */
