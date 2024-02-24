/*
 * mem_mng.c
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
 * along with OBDH 2.0. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Memory management routines implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.10.9
 * 
 * \date 2024/02/24
 * 
 * \addtogroup mem_mng
 * \{
 */

#include "mem_mng.h"

int mem_mng_check_fram(void)
{
    return -1;
}

int mem_mng_init_fram(void)
{
    return -1;
}

int mem_mng_reset_fram(void)
{
    return -1;
}

int mem_mng_save_obdh_data_to_fram(obdh_data_t data, sys_time_t ts)
{
    return -1;
}

int mem_mng_load_obdh_data_from_fram(obdh_data_t *data, sys_time_t *ts)
{
    return -1;
}

/** \} End of mem_mng group */
