/*
 * media_wrap.h
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
 * \brief Media device wrap definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.2
 * 
 * \date 2021/08/15
 * 
 * \defgroup media_wrap Media Wrap
 * \ingroup tests
 * \{
 */

#ifndef MEDIA_WRAP_H_
#define MEDIA_WRAP_H_

#include <stdint.h>

#include <devices/media/media.h>

int __wrap_media_init(media_t med);

int __wrap_media_write(media_t med, uint32_t adr, uint8_t *data, uint16_t len);

int __wrap_media_read(media_t med, uint32_t adr, uint8_t *data, uint16_t len);

int __wrap_media_erase(media_t med, media_erase_t type, uint32_t sector);

media_info_t __wrap_media_get_info(media_t med);

#endif /* MEDIA_WRAP_H_ */

/** \} End of media_wrap group */
