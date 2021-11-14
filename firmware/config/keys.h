/*
 * keys.h
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
 * \brief Keys definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.34
 * 
 * \date 2021/10/20
 * 
 * \defgroup keys Keys
 * \{
 */

#ifndef KEYS_H_
#define KEYS_H_

/* NOTE: These keys are just for tests, as this code is hosted in a public repository, */
/*       all of them should be replaced by the real ones before the launch of the satellite!!*/

/* Telecommands keys */
#define CONFIG_TC_KEY_DATA_REQUEST                      ":*gRc9h.Mhwzl1PW"
#define CONFIG_TC_KEY_ENTER_HIBERNATION                 "#7@rsQVL$Pxf(5eU"
#define CONFIG_TC_KEY_LEAVE_HIBERNATION                 "5d#QvdYzK59`Du>z"
#define CONFIG_TC_KEY_ACTIVATE_MODULE                   "6mP=VA33u/:m)b+z"
#define CONFIG_TC_KEY_DEACTIVATE_MODULE                 ":jvDVD_)jR@HWT{_"
#define CONFIG_TC_KEY_ACTIVATE_PAYLOAD_EDC              "`Uyp)]^n,3q[/zs["
#define CONFIG_TC_KEY_ACTIVATE_PAYLOAD_PAYLOAD_X        "d0bG$pIpdPzuSVV^"
#define CONFIG_TC_KEY_ACTIVATE_PAYLOAD_HARSH            "zhD)GQ7rxD;:U7s$"
#define CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_EDC            "Bdks$V>JzK*3*A}2"
#define CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_PAYLOAD_X      "fna%&wM|Y(wIF32J"
#define CONFIG_TC_KEY_DEACTIVATE_PAYLOAD_HARSH          "=jG,3r:bxiN'I;cK"
#define CONFIG_TC_KEY_ERASE_MEMORY                      "C:8RkJE${^>%wYVR"
#define CONFIG_TC_KEY_FORCE_RESET                       "@A)Z-t(x+@nDHpKF"
#define CONFIG_TC_KEY_GET_PAYLOAD_DATA                  "BkN&a):^fr(@(5x?"

#endif /* KEYS_H_ */

/** \} End of keys group */
