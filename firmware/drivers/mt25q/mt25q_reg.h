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
 * \version 0.7.42
 * 
 * \date 2020/03/13
 * 
 * \defgroup mt25q
 * \ingroup mt25q
 * \{
 */

#ifndef MT25Q_REG_H_
#define MT25Q_REG_H_

/* Software Reset Operations */
#define MT25Q_RESET_ENABLE                                  0x66U   /**< Enables the software reset. */
#define MT25Q_RESET_MEMORY                                  0x99U   /**< Reset memory command. */

/* Read ID Operations */
#define MT25Q_READ_ID_REG                                   0x9EU   /**< Read ID register. */
#define MT25Q_MULTIPLE_IO_READ_ID_REG                       0xAFU   /**< Multiple I/O read ID register. */
#define MT25Q_READ_SERIAL_FLASH_DISCOVERY_PARAMETER_REG     0x5AU   /**< Read serial flash discovery parameter. */

/* Read Memory Operations */
#define MT25Q_READ                                          0x03U   /**< Read operation. */
#define MT25Q_FAST_READ                                     0x0BU   /**< Fast read operation. */
#define MT25Q_DUAL_OUTPUT_FAST_READ                         0x3BU   /**< Dual output fast read. */
#define MT25Q_DUAL_INPUT_OUTPUT_FAST_READ                   0xBBU   /**< Dual input/output fast read. */
#define MT25Q_QUAD_OUTPUT_FAST_READ                         0x3BU   /**< Quad output fast read. */
#define MT25Q_QUAD_INPUT_OUTPUT_FAST_READ                   0xEBU   /**< Quad intput/output fast read. */
#define MT25Q_DTR_FAST_READ                                 0x0DU   /**< DTR fast read operation. */
#define MT25Q_DTR_DUAL_OUTPUT_FAST_READ                     0x3DU   /**< DTR dual output fast read. */
#define MT25Q_DTR_DUAL_INPUT_OUTPUT_FAST_READ               0xBDU   /**< DTR dual input/output fast read. */
#define MT25Q_DTR_QUAD_OUTPUT_FAST_READ                     0x6DU   /**< DTR quad output fast read. */
#define MT25Q_DTR_QUAD_INPUT_OUTPUT_FAST_READ               0xEDU   /**< DTR input/output fast read. */
#define MT25Q_QUAD_INPUT_OUTPUT_WORD_READ                   0xE7U   /**< Quad input/output word read. */

/* Read Memory Operations with 4-Byte Address */
#define MT25Q_4_BYTE_READ                                   0x13U   /**< 4-byte read. */
#define MT25Q_4_BYTE_FAST_READ                              0x0CU   /**< 4-byte fast read. */
#define MT25Q_4_BYTE_DUAL_OUTPUT_FAST_READ                  0x3CU   /**< 4-byte dual output fast read. */
#define MT25Q_4_BYTE_DUAL_INPUT_OUTPUT_FAST_READ            0xBCU   /**< 4-byte dual input/output fast read. */
#define MT25Q_4_BYTE_QUAD_OUTPUT_FAST_READ                  0x6CU   /**< 4-byte quad input/output fast read. */
#define MT25Q_4_BYTE_QUAD_INPUT_OUTPUT_FAST_READ            0xECU   /**< 4-byte quad input/output fast read. */
#define MT25Q_4_BYTE_DTR_FAST_READ                          0x0EU   /**< 4-byte DTR fast read. */
#define MT25Q_4_BYTE_DTR_DUAL_INPUT_OUTPUT_FAST_READ        0xBEU   /**< 4-byte DTR dual input/output fast read. */
#define MT25Q_4_BYTE_DTR_QUAD_INPUT_OUTPUT_FAST_READ        0xEEU   /**< 4-byte DTR quad input/output fast read. */

/* Write Operations */
#define MT25Q_WRITE_ENABLE                                  0x06U   /**< Write enable. */
#define MT25Q_WRITE_DISABLE                                 0x04U   /**< Write disable. */

/* Read Register Operations */
#define MT25Q_READ_STATUS_REGISTER                          0x05U   /**< Read status register. */
#define MT25Q_READ_FLAG_STATUS_REGISTER                     0x70U   /**< Read flag status register. */
#define MT25Q_READ_NONVOLATILE_CONFIG_REGISTER              0xB5U   /**< Read nonvolatile configuration register. */
#define MT25Q_READ_VOLATILE_CONFIG_REGISTER                 0x85U   /**< Read volatile configuration register. */
#define MT25Q_READ_ENHANCED_VOLATILE_CONFIG_REGISTER        0x65U   /**< Read enhanced volatile configuration register. */
#define MT25Q_READ_EXTENDED_ADDRESS_REGISTER                0xC8U   /**< Read extended address register. */
#define MT25Q_READ_GENERAL_PURPOSE_READ_REGISTER            0x96U   /**< General purpose read register. */

/* Write Register Operations */
#define MT25Q_WRITE_STATUS_REGISTER                         0x01U   /**< Write status register. */
#define MT25Q_WRITE_NONVOLATILE_CONFIG_REGISTER             0xB1U   /**< Write nonvolatile configuration register. */
#define MT25Q_WRITE_VOLATILE_CONFIG_REGISTER                0x81U   /**< Write volatile configuration register. */
#define MT25Q_WRITE_ENHANCED_VOLATILE_CONFIG_REGISTER       0x61U   /**< Write enhanced volatile configuration register. */
#define MT25Q_WRITE_EXTENDED_ADDRESS_REGISTER               0xC5U   /**< Write extended address register. */

/* Clear Flag Status Register Operation */
#define MT25Q_CLEAR_FLAG_STATUS_REGISTER                    0x50U   /**< Clear flag status register. */

/* Program Operations */
#define MT25Q_PAGE_PROGRAM                                  0x02U   /**< Page program. */
#define MT25Q_DUAL_INPUT_FAST_PROGRAM                       0xA2U   /**< Dual input fast program. */
#define MT25Q_EXTENDED_DUAL_INPUT_FAST_PROGRAM              0xD2U   /**< Extended dual input fast program. */
#define MT25Q_QUAD_INPUT_FAST_PROGRAM                       0x32U   /**< Quad input fast program. */
#define MT25Q_EXTENDED_QUAD_INPUT_FAST_PROGRAM              0x38U   /**< Extended quad input fast program. */

/* Program Operations With 4-Byte Address */
#define MT25Q_4_BYTE_PAGE_PROGRAM                           0x12U   /**< 4-byte page program. */
#define MT25Q_4_BYTE_QUAD_INPUT_FAST_PROGRAM                0x34U   /**< 4-byte quad input fast program. */
#define MT25Q_4_BYTE_QUAD_INPUT_EXTENDED_FAST_PROGRAM       0x3EU   /**< 4-byte quad input extended fast program. */

/* Erase Operations */
#define MT25Q_32KB_SUBSECTOR_ERASE                          0x52U   /**< 32 kB subsector erase. */
#define MT25Q_4KB_SUBSECTION_ERASE                          0x20U   /**< 4 kB subsection erase. */
#define MT25Q_SECTOR_ERASE                                  0xD8U   /**< Sector erase. */
#define MT25Q_DIE_ERASE                                     0xC4U   /**< Die erase. */

/* Erase Operations With 4-Byte Address */
#define MT25Q_4_BYTE_SECTOR_ERASE                           0xDCU   /**< 4-byte sector erase. */
#define MT25Q_4_BYTE_4KB_SUBSECTOR_ERASE                    0x21U   /**< 4-byte 4 kB subsector erase. */
#define MT25Q_4_BYTE_32KB_SUBSECTOR_ERASE                   0x5CU   /**< 4-byte 32 kB subsector erase. */

/* Suspend/Resume Operations */
#define MT25Q_PROGRAM_ERASE_SUSPEND                         0x75U   /**< Program erase suspend. */
#define MT25Q_PROGRAM_ERASE_RESUME                          0x7AU   /**< Program erase resume. */

/* One-Time Programmable (OTP) Operations */
#define MT25Q_READ_OTP_ARRAY                                0x4BU   /**< Read OTP array. */
#define MT25Q_PROGRAM_OTP_ARRAY                             0x42U   /**< Program OTP array. */

/* 4-Byte Address Mode Operations */
#define MT25Q_ENTER_4_BYTE_ADDRESS_MODE                     0xB7U   /**< Enter 4-byte address mode. */
#define MT25Q_EXIT_4_BYTE_ADDRESS_MODE                      0xE9U   /**< Exit 4-byte address mode. */

/* Deep Power-Down Operations */
#define MT25Q_ENTER_DEEP_POWER_DOWN                         0xB9U   /**< Enter deep power-down. */
#define MT25Q_RELEASE_FROM_DEEP_POWER_DOWN                  0xABU   /**< Release the device from the deep power-down. */

/* Quad Protocol Operations */
#define MT25Q_ENTER_QUAD_INPUT_OUTPUT_MODE                  0x35U   /**< Enter quad input/output mode. */
#define MT25Q_RESET_QUAD_INPUT_OUTPUT_MODE                  0xF5U   /**< Reset quad input/output mode. */

/* Advanced Sector Protection Operations */
#define MT25Q_READ_SECTOR_PROTECTION                        0x2DU   /**< Read sector protection. */
#define MT25Q_PROGRAM_SECTOR_PROTECTION                     0x2CU   /**< Program sector protection. */
#define MT25Q_READ_VOLATILE_LOCK_BITS                       0xE8U   /**< Read volatile lock bits. */
#define MT25Q_WRITE_VOLATILE_LOCK_BITS                      0xE5U   /**< Write volatile lock bits. */
#define MT25Q_READ_NONVOLATILE_LOCK_BITS                    0xE2U   /**< Read nonvolatile lock bits. */
#define MT25Q_WRITE_NONVOLATILE_LOCK_BITS                   0xE3U   /**< Write nonvolatile lock bits. */
#define MT25Q_ERASE_NONVOLATILE_LOCK_BITS                   0xE4U   /**< Erase nonvolatile lock bits. */
#define MT25Q_READ_GLOBAL_FREEZE_BIT                        0xA7U   /**< Read global freeze bit. */
#define MT25Q_WRITE_GLOBAL_FREEZE_BIT                       0xA6U   /**< Write global freeze bit. */
#define MT25Q_READ_PASSWORD                                 0x27U   /**< Read password. */
#define MT25Q_WRITE_PASSWORD                                0x28U   /**< Write password. */
#define MT25Q_UNLOCK_PASSWORD                               0x29U   /**< Unlock password. */

/* Advanced Sector Protection Operations With 4-Byte Address */
#define MT25Q_4_BYTE_READ_VOLATILE_LOCK_BITS                0xE0U   /**< 4-byte read volatile lock bits. */
#define MT25Q_4_BYTE_WRITE_VOLATILE_LOCK_BITS               0xE1U   /**< 4-byte write volatile lock bits. */

/* Advanced Function Interface Operations */
#define MT25Q_INTERFACE_ACTIVATION                          0x9BU   /**< Interface activation. */
#define MT25Q_CYCLIC_REDUNDANCY_CHECK                       0x27U   /**< Cyclic redundancy check. */


/* Status Register Bits */
#define MT25Q_REG_STATUS_WRITE_ENABLE                       (1U << 7)
#define MT25Q_REG_STATUS_TOP_BOTTOM                         (1U << 5)
#define MT25Q_REG_STATUS_BP                                 ((1U << 6) | (1U << 4) | (1U << 3) | (1U << 2))
#define MT25Q_REG_STATUS_WRITE_ENABLE_LATCH                 (1U << 1)
#define MT25Q_REG_STATUS_WRITE_IN_PROGRESS                  (1U << 0)

/* Flag Status Register Bits */
#define MT25Q_REG_FLAG_STATUS_PROGRAM_ERASE_CONTROLLER      (1U << 7)
#define MT25Q_REG_FLAG_STATUS_ERASE_SUSPEND                 (1U << 6)
#define MT25Q_REG_FLAG_STATUS_ERASE                         (1U << 5)
#define MT25Q_REG_FLAG_STATUS_PROGRAM                       (1U << 4)
#define MT25Q_REG_FLAG_STATUS_PROGRAM_SUSPEND               (1U << 2)
#define MT25Q_REG_FLAG_STATUS_PROTECTION                    (1U << 1)
#define MT25Q_REG_FLAG_STATUS_ADDRESSING                    (1U << 0)

/* Nonvolatile Configuration Register Bits */
#define MT25Q_REG_NVM_CONFIG_DUMMY_CLOCK_CYCLES             0xF000U
#define MT25Q_REG_NVM_CONFIG_XIP_MODE_AT_POR                0x0E00U
#define MT25Q_REG_NVM_CONFIG_OUTPUT_DRV_STRENGTH            0x01C0U
#define MT25Q_REG_NVM_CONFIG_2X_TRANSGER_PROTOCOL           (1U << 5)
#define MT25Q_REG_NVM_CONFIG_RESET_HOLD                     (1U << 4)
#define MT25Q_REG_NVM_CONFIG_QUAD_IO_PROTOCOL               (1U << 3)
#define MT25Q_REG_NVM_CONFIG_DUAL_IO_PROTOCOL               (1U << 2)
#define MT25Q_REG_NVM_CONFIG_128MB_SEGMENT_SEL              (1U << 1)
#define MT25Q_REG_NVM_CONFIG_ADR_BYTES_DURING_CMD_ENTRY     (1U << 0)

/* Volatile Configuration Register Bits */
#define MT25Q_REG_VM_CONFIG_DUMMY_CLOCK_CYCLES              0x01F0U
#define MT25Q_REG_VM_CONFIG_XIP                             (1U << 3)
#define MT25Q_REG_VM_CONFIG_RESERVED                        (1U << 2)
#define MT25Q_REG_VM_CONFIG_WRAP                            ((1U << 1) | (1U << 0))

/* Enhanced Volatile Configuration Register Bits */
#define MT25Q_REG_EVC_QUAD_IO_PROTOCOL                      (1U << 7)
#define MT25Q_REG_EVC_DUAL_IO_PROTOCOL                      (1U << 6)
#define MT25Q_REG_EVC_DOUBLE_TRANSGER_RATE_PROTOCOL         (1U << 5)
#define MT25Q_REG_EVC_RESET_HOLD                            (1U << 4)
#define MT25Q_REG_EVC_OUTPUT_DRIVER_STRENGTH                ((1U << 2) | (1U << 1) | (1U << 0))

/* Sector Protection Register Bits */
#define MT25Q_REG_SEC_PROTEC_PASSWORD_PROTECTION_LOCK       (1U << 2)
#define MT25Q_REG_SEC_PROTEC_SECTOR_PROTECTION_LOCK         (1U << 1)

#endif /* MT25Q_REG_H_ */

/** \} End of mt25q_reg group */
