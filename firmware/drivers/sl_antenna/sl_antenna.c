/*
 * sl_antenna.c
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
 * \brief SpaceLab Antenna Driver implementation.
 *
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.13
 *
 * \date 2024/03/06
 *
 * \addtogroup sl_antenna
 * \{
 */

#include <drivers/gpio/gpio.h>

#include "sl_antenna.h"

/* Antenna gpio pin */
#define SL_ANTENNA_DEPLOY_PIN GPIO_PIN_61

int sl_antenna_init(void)
{
    sl_antenna_config_t ant_config = {0};
    int err = -1;

    ant_config.mode = GPIO_MODE_OUTPUT;

    if (gpio_init(SL_ANTENNA_DEPLOY_PIN, ant_config) == 0)
    {
        if (gpio_set_state(SL_ANTENNA_DEPLOY_PIN, GPIO_STATE_LOW) == 0)
        {
            err = 0;
        }
    }

    return err;
}

int sl_antenna_start_sequential_deploy(void)
{
    int err = -1;

    if (gpio_set_state(SL_ANTENNA_DEPLOY_PIN, SL_ANTENNA_FIRST_STAGE_STATE) == 0)
    {
        sl_antenna_delay_s(SL_ANTENNA_STAGE_DURATION_SEC);

        sl_antenna_delay_s(SL_ANTENNA_HALF_TRANSITION_DURATION_SEC);

        if (gpio_set_state(SL_ANTENNA_DEPLOY_PIN, SL_ANTENNA_SECOND_STAGE_STATE) == 0)
        {
            sl_antenna_delay_s(SL_ANTENNA_HALF_TRANSITION_DURATION_SEC);

            sl_antenna_delay_s(SL_ANTENNA_STAGE_DURATION_SEC);

            sl_antenna_delay_s(SL_ANTENNA_HALF_TRANSITION_DURATION_SEC);

            if (gpio_set_state(SL_ANTENNA_DEPLOY_PIN, SL_ANTENNA_THIRD_STAGE_STATE) == 0)
            {
                sl_antenna_delay_s(SL_ANTENNA_HALF_TRANSITION_DURATION_SEC);

                sl_antenna_delay_s(SL_ANTENNA_STAGE_DURATION_SEC);

                err = 0;
            }
        }
    }

    (void)gpio_set_state(SL_ANTENNA_DEPLOY_PIN, GPIO_STATE_LOW);

    return err;
}

/** \} End of sl_antenna group */
