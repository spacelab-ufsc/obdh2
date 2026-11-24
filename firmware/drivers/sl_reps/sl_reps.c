/*
 * sl_eps2.c
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
 * \brief SpaceLab REPS driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/11/17
 * 
 * \addtogroup sl_reps
 * \{
 */

#include <stdbool.h>
#include <stdint.h>

#include "sl_reps.h"

int sl_reps_read_hk_data(const sl_reps_config_t config, sl_reps_hk_data_t *data){
    uint32_t err_counter = 0;

    if(sl_reps_read_cur_vbus_bk3v3(config, &data->current_vbus_bk3v3) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_vbus_bk5v0(config, &data->current_vbus_bk5v0) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_vpanels(config, &data->current_vpanels) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_vbat(config, &data->current_vbat) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_vin(config, &data->current_vin) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_3v3_obdh(config, &data->current_3v3_obdh) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_3v3_antdep(config, &data->current_3v3_antdep) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_3v3_beacon(config, &data->current_3v3_beacon) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_5v0_payload(config, &data->current_5v0_payload) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_5v0_radio_beacon(config, &data->current_5v0_radio_beacon) != 0){
        err_counter++;
    }

    if(sl_reps_read_cur_5v0_radio_main(config, &data->current_5v0_radio_main) != 0){
        err_counter++;
    }

    if(sl_reps_read_vol_vpanels(config, &data->voltage_vpanels) != 0){
        err_counter++;
    }

    if(sl_reps_read_vol_vbus(config, &data->voltage_vbus) != 0){
        err_counter++;
    }

    if(sl_reps_read_vol_vin(config, &data->voltage_vin) != 0){
        err_counter++;
    }

    if(sl_reps_read_vol_3v3(config, &data->voltage_3v3) != 0){
        err_counter++;
    }

    if(sl_reps_read_vol_5v0(config, &data->voltage_5v0) != 0){
        err_counter++;
    }

    return err_counter;
}

int sl_reps_read_cur_vbus_bk3v3(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_VBUS_BK3V3;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_vbus_bk5v0(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_VBUS_BK5V0;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_vpanels(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_VPANELS;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_vbat(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_VBAT;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_vin(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_VIN;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_3v3_obdh(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_3v3_OBDH;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_3v3_antdep(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_3v3_AntDep;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_3v3_beacon(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_3v3_BEACON;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_5v0_payload(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_5v0_PAYLOAD;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_5v0_radio_beacon(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_5v0_RADIO_BEACON;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_cur_5v0_radio_main(const sl_reps_config_t config, sl_reps_current_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_CUR_5v0_RADIO_MAIN;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_vol_vpanels(const sl_reps_config_t config, sl_reps_voltage_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_VOL_VPANELS;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_vol_vin(const sl_reps_config_t config, sl_reps_voltage_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_VOL_VBUS;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_vol_vbus(const sl_reps_config_t config, sl_reps_voltage_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_VOL_VIN;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_vol_3v3(const sl_reps_config_t config, sl_reps_voltage_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_VOL_3v3;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

int sl_reps_read_vol_5v0(const sl_reps_config_t config, sl_reps_voltage_t *data){
    static const uint8_t sel_signal = REPS_SELSIG_VOL_5v0;

    for(uint8_t i = 0; i < 4; i++){
        gpio_set_state(config.hkmux_sel_pins[i], ((sel_signal >>  i) & 1));
    }

    /* give time to signal to stabilize. Probably can be removed*/
    sl_reps_delay_ms(REPS_MUXOUT_SIG_WAIT_MS);

    (*data) = adc_read(config.hkmux_out_adc_port, data);

    return 0;
}

/** \} End of sl_reps group */
