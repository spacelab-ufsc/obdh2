/*
 * redconf.c
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
 * \brief Configuration parameters definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.6.30
 * 
 * \date 2021/05/16
 * 
 * \defgroup config Configuration
 * \{
 */

#include <redconf.h>
#include <redtypes.h>
#include <redmacs.h>
#include <redvolume.h>

const VOLCONF gaRedVolConf[REDCONF_VOLUME_COUNT] =
{
    {512U, 65536U, 0U, false, 10U, 3U},
};

/** \} End of redconf group */
