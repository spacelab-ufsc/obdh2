/*
 * mt25q_reg.h
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
 * \brief MT25Q driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.5.26
 * 
 * \date 2020/03/13
 * 
 * \defgroup mt25q
 * \ingroup mt25q
 * \{
 */

#ifndef MT25Q_REG_H_
#define MT25Q_REG_H_

/* Read ID Operations */
#define MT25Q_READ_ID_REG                                   0x9E    /**< Read ID register. */
#define MT25Q_MULTIPLE_IO_READ_ID_REG                       0xAF    /**< Multiple I/O read ID register. */
#define MT25Q_READ_SERIAL_FLASH_DISCOVERY_PARAMETER_REG     0x5A    /**< Read serial flash discovery parameter. */

/* Read Register Operations */
#define MT25Q_READ_STATUS_REG                               0x05    /**< Read status register. */
#define MT25Q_READ_FLAG_STATUS_REG                          0x70    /**< Read flag status register. */
#define MT25Q_READ_NONVOLATILE_CONFIG_REG                   0xB5    /**< Read nonvolatile configuration register. */
#define MT25Q_READ_VOLATILE_CONFIG_REG                      0x85    /**< Read volatile configuration register. */
#define MT25Q_READ_ENHANCED_VOLATILE_CONFIG_REG             0x65    /**< Read enhanced volatile configuration register. */
#define MT25Q_READ_EXTENDED_ADDRESS_REG                     0xC8    /**< Read extended address register. */

/* Write Register Operations */
#define MT25Q_WRITE_STATUS_REG                              0x01    /**< Write status register. */
#define MT25Q_WRITE_NONVOLATILE_CONFIG_REG                  0xB1    /**< Write nonvolatile configuration register. */
#define MT25Q_WRITE_VOLATILE_CONFIG_REG                     0x81    /**< Write volatile configuration register. */
#define MT25Q_WRITE_ENHANCED_VOLATILE_CONFIG_REG            0x61    /**< Write enhanced volatile configuration register. */
#define MT25Q_WRITE_EXTENDED_ADDRESS_REG                    0xC5    /**< Write extended address register. */

/* Status Register Bits */
#define MT25Q_REG_STATUS_WRITE_ENABLE                       (1 << 7)
#define MT25Q_REG_STATUS_TOP_BOTTOM                         (1 << 5)
#define MT25Q_REG_STATUS_BP                                 ((1 << 6) | (1 << 4) | (1 << 3) | (1 << 2))
#define MT25Q_REG_STATUS_WRITE_ENABLE_LATCH                 (1 << 1)
#define MT25Q_REG_STATUS_WRITE_IN_PROGRESS                  (1 << 0)

/* Flag Status Register Bits */
#define MT25Q_REG_FLAG_STATUS_PROGRAM_ERASE_CONTROLLER      (1 << 7)
#define MT25Q_REG_FLAG_STATUS_ERASE_SUSPEND                 (1 << 6)
#define MT25Q_REG_FLAG_STATUS_ERASE                         (1 << 5)
#define MT25Q_REG_FLAG_STATUS_PROGRAM                       (1 << 4)
#define MT25Q_REG_FLAG_STATUS_PROGRAM_SUSPEND               (1 << 2)
#define MT25Q_REG_FLAG_STATUS_PROTECTION                    (1 << 1)
#define MT25Q_REG_FLAG_STATUS_ADDRESSING                    (1 << 0)

/* Nonvolatile Configuration Register Bits */
#define MT25Q_REG_NVM_CONFIG_DUMMY_CLOCK_CYCLES             0xF000
#define MT25Q_REG_NVM_CONFIG_XIP_MODE_AT_POR                0x0E00
#define MT25Q_REG_NVM_CONFIG_OUTPUT_DRV_STRENGTH            0x01C0
#define MT25Q_REG_NVM_CONFIG_2X_TRANSGER_PROTOCOL           (1 << 5)
#define MT25Q_REG_NVM_CONFIG_RESET_HOLD                     (1 << 4)
#define MT25Q_REG_NVM_CONFIG_QUAD_IO_PROTOCOL               (1 << 3)
#define MT25Q_REG_NVM_CONFIG_DUAL_IO_PROTOCOL               (1 << 2)
#define MT25Q_REG_NVM_CONFIG_128MB_SEGMENT_SEL              (1 << 1)
#define MT25Q_REG_NVM_CONFIG_ADR_BYTES_DURING_CMD_ENTRY     (1 << 0)

/* Volatile Configuration Register Bits */
#define MT25Q_REG_VM_CONFIG_DUMMY_CLOCK_CYCLES              0x01F0
#define MT25Q_REG_VM_CONFIG_XIP                             (1 << 3)
#define MT25Q_REG_VM_CONFIG_RESERVED                        (1 << 2)
#define MT25Q_REG_VM_CONFIG_WRAP                            ((1 << 1) | (1 << 0))

/* Enhanced Volatile Configuration Register Bits */
#define MT25Q_REG_EVC_QUAD_IO_PROTOCOL                      (1 << 7)
#define MT25Q_REG_EVC_DUAL_IO_PROTOCOL                      (1 << 6)
#define MT25Q_REG_EVC_DOUBLE_TRANSGER_RATE_PROTOCOL         (1 << 5)
#define MT25Q_REG_EVC_RESET_HOLD                            (1 << 4)
#define MT25Q_REG_EVC_OUTPUT_DRIVER_STRENGTH                ((1 << 2) | (1 << 1) | (1 << 0))

/* Sector Protection Register Bits */
#define MT25Q_REG_SEC_PROTEC_PASSWORD_PROTECTION_LOCK       (1 << 2)
#define MT25Q_REG_SEC_PROTEC_SECTOR_PROTECTION_LOCK         (1 << 1)

#endif /* MT25Q_REG_H_ */

/** \} End of mt25q_reg group */
