/*
 * reps.c
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
 * \brief REPS device implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2026/11/17
 * 
 * \addtogroup reps
 * \{
 */

#include <stdbool.h>
#include <system/sys_log/sys_log.h>
#include <drivers/sl_reps/sl_reps.h>

#include "reps.h"

static sl_reps_config_t reps_config = {0};
static bool reps_is_open = false;

int reps_init(void){
    int err = 0;

    if(reps_is_open){    
        return err;
    }else{
        sys_log_print_event_from_module(SYS_LOG_INFO, REPS_MODULE_NAME, "Initializing REPS device...");
        sys_log_new_line();

        // TODO: configure reps_config based on the available GPIOs
    }
}

int reps_get_voltage(reps_hk_voltage_e which, sl_reps_voltage_t *data){
    switch(which){
        case REPS_HK_VOLTAGE_VPANELS:   sl_reps_read_vol_vpanels(reps_config, data);    break;
        case REPS_HK_VOLTAGE_VBUS:      sl_reps_read_vol_vbus(reps_config, data);       break;
        case REPS_HK_VOLTAGE_VIN:       sl_reps_read_vol_vin(reps_config, data);        break;
        case REPS_HK_VOLTAGE_3V3:       sl_reps_read_vol_3v3(reps_config, data);        break;
        case REPS_HK_VOLTAGE_5V0:       sl_reps_read_vol_5v0(reps_config, data);        break;
        default: 
            sys_log_print_event_from_module(SYS_LOG_ERROR, REPS_MODULE_NAME, "Error while trying to read housekeeping voltage: 'which' variable to read out of range.");
            sys_log_new_line();

            return -1;
    }

    return 0;
}

int reps_get_current(reps_hk_current_e which, sl_reps_current_t *data){
    switch(which){
        case REPS_HK_CURRENT_VBUS_BK5V0:        sl_reps_read_cur_vbus_bk5v0(reps_config, data);         break;
        case REPS_HK_CURRENT_VBUS_BK3V3:        sl_reps_read_cur_vbus_bk3v3(reps_config, data);         break;
        case REPS_HK_CURRENT_VPANELS:           sl_reps_read_cur_vpanels(reps_config, data);            break;
        case REPS_HK_CURRENT_VBAT:              sl_reps_read_cur_vbat(reps_config, data);               break;
        case REPS_HK_CURRENT_VIN:               sl_reps_read_cur_vin(reps_config, data);                break;
        case REPS_HK_CURRENT_3V3_OBDH:          sl_reps_read_cur_3v3_obdh(reps_config, data);           break;
        case REPS_HK_CURRENT_5V0_PAYLOAD:       sl_reps_read_cur_5v0_payload(reps_config, data);        break;
        case REPS_HK_CURRENT_5V0_RADIOBEACON:   sl_reps_read_cur_5v0_radio_beacon(reps_config, data);   break;
        case REPS_HK_CURRENT_3V3_BEACON:        sl_reps_read_cur_3v3_beacon(reps_config, data);         break;
        case REPS_HK_CURRENT_5V0_RADIOMAIN:     sl_reps_read_cur_5v0_radio_main(reps_config, data);     break;
        case REPS_HK_CURRENT_3V3_ANTDEP:        sl_reps_read_cur_3v3_antdep(reps_config, data);         break;
        default: 
            sys_log_print_event_from_module(SYS_LOG_ERROR, REPS_MODULE_NAME, "Error while trying to read housekeeping current: 'which' variable to read out of range.");
            sys_log_new_line();

            return -1;
    }
    
    return 0;
}   

/** \} End of reps group */
