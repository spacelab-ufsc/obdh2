/*
 * mt25ql01gbbb_reg.h
 * 
 * Copyright (C) 2020, SpaceLab.
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
 * \brief MT25QL01GBBB driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.1
 * 
 * \date 13/03/2020
 * 
 * \defgroup mt25ql01gbbb
 * \ingroup mt25ql01gbbb
 * \{
 */

#ifndef MT25QL01GBBB_REG_H_
#define MT25QL01GBBB_REG_H_

/* Read Register Operations */
#define MT25QL01GBBB_READ_STATUS_REG                            0x05    /**< Read status register. */
#define MT25QL01GBBB_READ_FLAG_STATUS_REG                       0x70    /**< Read flag status register. */
#define MT25QL01GBBB_READ_NONVOLATILE_CONFIG_REG                0xB5    /**< Read nonvolatile configuration register. */
#define MT25QL01GBBB_READ_VOLATILE_CONFIG_REG                   0x85    /**< Read volatile configuration register. */
#define MT25QL01GBBB_READ_ENHANCED_VOLATILE_CONFIG_REG          0x65    /**< Read enhanced volatile configuration register. */
#define MT25QL01GBBB_READ_EXTENDED_ADDRESS_REG                  0xC8    /**< Read extended address register. */

/* Write Register Operations */
#define MT25QL01GBBB_WRITE_STATUS_REG                           0x01    /**< Write status register. */
#define MT25QL01GBBB_WRITE_NONVOLATILE_CONFIG_REG               0xB1    /**< Write nonvolatile configuration register. */
#define MT25QL01GBBB_WRITE_VOLATILE_CONFIG_REG                  0x81    /**< Write volatile configuration register. */
#define MT25QL01GBBB_WRITE_ENHANCED_VOLATILE_CONFIG_REG         0x61    /**< Write enhanced volatile configuration register. */
#define MT25QL01GBBB_WRITE_EXTENDED_ADDRESS_REG                 0xC5    /**< Write extended address register. */


#define MT25QL01GBBB_

/* Status Register Bits */
#define MT25QL01GBBB_REG_STATUS_WRITE_ENABLE                    (1 << 7)
#define MT25QL01GBBB_REG_STATUS_TOP_BOTTOM                      (1 << 5)
#define MT25QL01GBBB_REG_STATUS_BP                              ((1 << 6) | (1 << 4) | (1 << 3) | (1 << 2))
#define MT25QL01GBBB_REG_STATUS_WRITE_ENABLE_LATCH              (1 << 1)
#define MT25QL01GBBB_REG_STATUS_WRITE_IN_PROGRESS               (1 << 0)

/* Nonvolatile Configuration Register */
#define MT25QL01GBBB_REG_NVM_CONFIG_DUMMY_CLOCK_CYCLES          0xF000
#define MT25QL01GBBB_REG_NVM_CONFIG_XIP_MODE_AT_POR             0x0E00
#define MT25QL01GBBB_REG_NVM_CONFIG_OUTPUT_DRV_STRENGTH         0x01C0
#define MT25QL01GBBB_REG_NVM_CONFIG_2X_TRANSGER_PROTOCOL        (1 << 5)
#define MT25QL01GBBB_REG_NVM_CONFIG_RESET_HOLD                  (1 << 4)
#define MT25QL01GBBB_REG_NVM_CONFIG_QUAD_IO_PROTOCOL            (1 << 3)
#define MT25QL01GBBB_REG_NVM_CONFIG_DUAL_IO_PROTOCOL            (1 << 2)
#define MT25QL01GBBB_REG_NVM_CONFIG_128MB_SEGMENT_SEL           (1 << 1)
#define MT25QL01GBBB_REG_NVM_CONFIG_ADR_BYTES_DURING_CMD_ENTRY  (1 << 0)
#define MT25QL01GBBB_REG_

/* Volatile Configuration Register */
#define MT25QL01GBBB_REG_VM_CONFIG_DUMMY_CLOCK_CYCLES           0x01F0
#define MT25QL01GBBB_REG_VM_CONFIG_XIP                          (1 << 3)
#define MT25QL01GBBB_REG_VM_CONFIG_RESERVED                     (1 << 2)
#define MT25QL01GBBB_REG_VM_CONFIG_WRAP                         ((1 << 1) | (1 << 0))



#endif /* MT25QL01GBBB_REG_H_ */

/** \} End of mt25ql01gbbb_reg group */
