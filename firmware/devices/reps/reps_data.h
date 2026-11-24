/*
 * reps_data.h
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
 * \brief REPS data structure definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Pedro Ferrari Barbosa <pedro.ferraribarbosa2007@gmail.com>
 * 
 * \version 0.7.22
 * 
 * \date 2026/11/12
 * 
 * \defgroup reps_data REPS Data
 * \ingroup reps
 * \{
 */

#ifndef REPS_DATA_H_
#define REPS_DATA_H_

#include <stdint.h>

#include <drivers/sl_eps2/sl_eps2.h>

typedef enum reps_hk_current_e{
    REPS_HK_CURRENT_VBUS_BK5V0 = 0,
    REPS_HK_CURRENT_VBUS_BK3V3,
    REPS_HK_CURRENT_VPANELS,
    REPS_HK_CURRENT_VBAT,
    REPS_HK_CURRENT_VIN,
    REPS_HK_CURRENT_3V3_OBDH,
    REPS_HK_CURRENT_5V0_PAYLOAD,
    REPS_HK_CURRENT_5V0_RADIOBEACON,
    REPS_HK_CURRENT_3V3_BEACON,
    REPS_HK_CURRENT_5V0_RADIOMAIN,
    REPS_HK_CURRENT_3V3_ANTDEP,
}reps_hk_current_e;


typedef enum reps_hk_voltage_e{
    REPS_HK_VOLTAGE_VPANELS = 0,
    REPS_HK_VOLTAGE_VIN,
    REPS_HK_VOLTAGE_VBUS,
    REPS_HK_VOLTAGE_5V0,
    REPS_HK_VOLTAGE_3V3,
}reps_hk_voltage_e;

#endif /* REPS_DATA_H_ */

/** \} End of reps_data group */
