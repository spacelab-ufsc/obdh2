/*
 * phj.h
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

/*
 * \brief PHJ driver definition.
 * 
 * \author João Cláudio Elsen Barcellos <joaoclaudiobarcellos@gmail.com>
 * 
 * \date 2020/03/30
 * 
 * \defgroup phj Payload Joinville
 * \ingroup drivers
 * \{
 */

#ifndef PHJ_H_
#define PHJ_H_

#include <stdint.h>
#include <stdbool.h>
#include <drivers/i2c/i2c.h>
#include <drivers/gpio/gpio.h>

#define PHJ_SLAVE_ADDRESS           0x48

#define PHJ_FRAME_STATE_LEN         13

#define PHJ_FRAME_ID_STATE          0x20

typedef struct
{
    i2c_port_t port;
    uint32_t bitrate;
} phj_config_i2c;

typedef struct
{
    gpio_pin_t pin;
    uint8_t mode;
} phj_config_gpio;

int phj_init_i2c(phj_config_i2c config);

int phj_init_gpio(phj_config_gpio config);

int phj_read(uint8_t *data, uint16_t len);

int phj_check_converter(void);

int phj_check_message(void);

#endif /* PHJ_H_ */

/** \} End of phj group */
