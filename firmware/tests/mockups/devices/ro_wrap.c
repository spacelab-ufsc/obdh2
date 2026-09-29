/*
 * ro_wrap.h
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
 * \brief Radio Occultation device wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Renato Augusto Schenkel Meneghin Marchiori <renato.meneghin@posgrad.ufsc.br>
 * 
 * \version 0.0.1
 * 
 * \date 2026/09/29
 * 
 * \addtogroup ro_wrap
 * \{
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <setjmp.h>
#include <float.h>
#include <cmocka.h>

#include "ro_wrap.h"

int __wrap_payload_init(ro_type_t ro)
{
    check_expected(ro);

    return mock_type(int);
}

int __wrap_payload_enable(ro_type_t ro)
{
    check_expected(ro);

    return mock_type(int);
}

int __wrap_payload_disable(ro_type_t ro)
{
    check_expected(ro);

    return mock_type(int);
}

int __wrap_payload_write_cmd(ro_type_t ro, ro_cmd_t cmd)
{
    check_expected(ro);
    check_expected(cmd);

    return mock_type(int);
}

int __wrap_payload_get_data(ro_type_t ro, ro_data_id_t id, uint8_t *data, uint32_t *len)
{
    check_expected(ro);
    check_expected(id);

    if (data != NULL)
    {
        *data = mock_type_ptr(uint8_t*);
    }

    if (len != NULL)
    {
        *len = mock_type(len);
    }

    return mock_type(int);
}

/** \} End of payload_wrap group */
