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
 * \version 0.5.33
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
#define MT25Q_RESET_ENABLE                                  0x66    /**< Enables the software reset. */
#define MT25Q_RESET_MEMORY                                  0x99    /**< Reset memory command. */

/* Read ID Operations */
#define MT25Q_READ_ID_REG                                   0x9E    /**< Read ID register. */
#define MT25Q_MULTIPLE_IO_READ_ID_REG                       0xAF    /**< Multiple I/O read ID register. */
#define MT25Q_READ_SERIAL_FLASH_DISCOVERY_PARAMETER_REG     0x5A    /**< Read serial flash discovery parameter. */

/* Read Memory Operations */
#define MT25Q_READ                                          0x03    /**< Read operation. */
#define MT25Q_FAST_READ                                     0x0B    /**< Fast read operation. */
#define MT25Q_DUAL_OUTPUT_FAST_READ                         0x3B    /**< Dual output fast read. */
#define MT25Q_DUAL_INPUT_OUTPUT_FAST_READ                   0xBB    /**< Dual input/output fast read. */
#define MT25Q_QUAD_OUTPUT_FAST_READ                         0x3B    /**< Quad output fast read. */
#define MT25Q_QUAD_INPUT_OUTPUT_FAST_READ                   0xEB    /**< Quad intput/output fast read. */
#define MT25Q_DTR_FAST_READ                                 0x0D    /**< DTR fast read operation. */
#define MT25Q_DTR_DUAL_OUTPUT_FAST_READ                     0x3D    /**< DTR dual output fast read. */
#define MT25Q_DTR_DUAL_INPUT_OUTPUT_FAST_READ               0xBD    /**< DTR dual input/output fast read. */
#define MT25Q_DTR_QUAD_OUTPUT_FAST_READ                     0x6D    /**< DTR quad output fast read. */
#define MT25Q_DTR_QUAD_INPUT_OUTPUT_FAST_READ               0xED    /**< DTR input/output fast read. */
#define MT25Q_QUAD_INPUT_OUTPUT_WORD_READ                   0xE7    /**< Quad input/output word read. */

/* Read Memory Operations with 4-Byte Address */
#define MT25Q_4_BYTE_READ                                   0x13    /**< 4-byte read. */
#define MT25Q_4_BYTE_FAST_READ                              0x0C    /**< 4-byte fast read. */
#define MT25Q_4_BYTE_DUAL_OUTPUT_FAST_READ                  0x3C    /**< 4-byte dual output fast read. */
#define MT25Q_4_BYTE_DUAL_INPUT_OUTPUT_FAST_READ            0xBC    /**< 4-byte dual input/output fast read. */
#define MT25Q_4_BYTE_QUAD_OUTPUT_FAST_READ                  0x6C    /**< 4-byte quad input/output fast read. */
#define MT25Q_4_BYTE_QUAD_INPUT_OUTPUT_FAST_READ            0xEC    /**< 4-byte quad input/output fast read. */
#define MT25Q_4_BYTE_DTR_FAST_READ                          0x0E    /**< 4-byte DTR fast read. */
#define MT25Q_4_BYTE_DTR_DUAL_INPUT_OUTPUT_FAST_READ        0xBE    /**< 4-byte DTR dual input/output fast read. */
#define MT25Q_4_BYTE_DTR_QUAD_INPUT_OUTPUT_FAST_READ        0xEE    /**< 4-byte DTR quad input/output fast read. */

/* Write Operations */
#define MT25Q_WRITE_ENABLE                                  0x06    /**< Write enable. */
#define MT25Q_WRITE_DISABLE                                 0x04    /**< Write disable. */

/* Read Register Operations */
#define MT25Q_READ_STATUS_REGISTER                          0x05    /**< Read status register. */
#define MT25Q_READ_FLAG_STATUS_REGISTER                     0x70    /**< Read flag status register. */
#define MT25Q_READ_NONVOLATILE_CONFIG_REGISTER              0xB5    /**< Read nonvolatile configuration register. */
#define MT25Q_READ_VOLATILE_CONFIG_REGISTER                 0x85    /**< Read volatile configuration register. */
#define MT25Q_READ_ENHANCED_VOLATILE_CONFIG_REGISTER        0x65    /**< Read enhanced volatile configuration register. */
#define MT25Q_READ_EXTENDED_ADDRESS_REGISTER                0xC8    /**< Read extended address register. */
#define MT25Q_READ_GENERAL_PURPOSE_READ_REGISTER            0x96    /**< General purpose read register. */

/* Write Register Operations */
#define MT25Q_WRITE_STATUS_REGISTER                         0x01    /**< Write status register. */
#define MT25Q_WRITE_NONVOLATILE_CONFIG_REGISTER             0xB1    /**< Write nonvolatile configuration register. */
#define MT25Q_WRITE_VOLATILE_CONFIG_REGISTER                0x81    /**< Write volatile configuration register. */
#define MT25Q_WRITE_ENHANCED_VOLATILE_CONFIG_REGISTER       0x61    /**< Write enhanced volatile configuration register. */
#define MT25Q_WRITE_EXTENDED_ADDRESS_REGISTER               0xC5    /**< Write extended address register. */

/* Clear Flag Status Register Operation */
#define MT25Q_CLEAR_FLAG_STATUS_REGISTER                    0x50    /**< Clear flag status register. */

/* Program Operations */
#define MT25Q_PAGE_PROGRAM                                  0x02    /**< Page program. */
#define MT25Q_DUAL_INPUT_FAST_PROGRAM                       0xA2    /**< Dual input fast program. */
#define MT25Q_EXTENDED_DUAL_INPUT_FAST_PROGRAM              0xD2    /**< Extended dual input fast program. */
#define MT25Q_QUAD_INPUT_FAST_PROGRAM                       0x32    /**< Quad input fast program. */
#define MT25Q_EXTENDED_QUAD_INPUT_FAST_PROGRAM              0x38    /**< Extended quad input fast program. */

/* Program Operations With 4-Byte Address */
#define MT25Q_4_BYTE_PAGE_PROGRAM                           0x12    /**< 4-byte page program. */
#define MT25Q_4_BYTE_QUAD_INPUT_FAST_PROGRAM                0x34    /**< 4-byte quad input fast program. */
#define MT25Q_4_BYTE_QUAD_INPUT_EXTENDED_FAST_PROGRAM       0x3E    /**< 4-byte quad input extended fast program. */

/* Erase Operations */
#define MT25Q_32KB_SUBSECTOR_ERASE                          0x52    /**< 32 kB subsector erase. */
#define MT25Q_4KB_SUBSECTION_ERASE                          0x20    /**< 4 kB subsection erase. */
#define MT25Q_SECTOR_ERASE                                  0xD8    /**< Sector erase. */
#define MT25Q_DIE_ERASE                                     0xC4    /**< Die erase. */

/* Erase Operations With 4-Byte Address */
#define MT25Q_4_BYTE_SECTOR_ERASE                           0xDC    /**< 4-byte sector erase. */
#define MT25Q_4_BYTE_4KB_SUBSECTOR_ERASE                    0x21    /**< 4-byte 4 kB subsector erase. */
#define MT25Q_4_BYTE_32KB_SUBSECTOR_ERASE                   0x5C    /**< 4-byte 32 kB subsector erase. */

/* Suspend/Resume Operations */
#define MT25Q_PROGRAM_ERASE_SUSPEND                         0x75    /**< Program erase suspend. */
#define MT25Q_PROGRAM_ERASE_RESUME                          0x7A    /**< Program erase resume. */

/* One-Time Programmable (OTP) Operations */
#define MT25Q_READ_OTP_ARRAY                                0x4B    /**< Read OTP array. */
#define MT25Q_PROGRAM_OTP_ARRAY                             0x42    /**< Program OTP array. */

/* 4-Byte Address Mode Operations */
#define MT25Q_ENTER_4_BYTE_ADDRESS_MODE                     0xB7    /**< Enter 4-byte address mode. */
#define MT25Q_EXIT_4_BYTE_ADDRESS_MODE                      0xE9    /**< Exit 4-byte address mode. */

/* Deep Power-Down Operations */
#define MT25Q_ENTER_DEEP_POWER_DOWN                         0xB9    /**< Enter deep power-down. */
#define MT25Q_RELEASE_FROM_DEEP_POWER_DOWN                  0xAB    /**< Release the device from the deep power-down. */

/* Quad Protocol Operations */
#define MT25Q_ENTER_QUAD_INPUT_OUTPUT_MODE                  0x35    /**< Enter quad input/output mode. */
#define MT25Q_RESET_QUAD_INPUT_OUTPUT_MODE                  0xF5    /**< Reset quad input/output mode. */

/* Advanced Sector Protection Operations */
#define MT25Q_READ_SECTOR_PROTECTION                        0x2D    /**< Read sector protection. */
#define MT25Q_PROGRAM_SECTOR_PROTECTION                     0x2C    /**< Program sector protection. */
#define MT25Q_READ_VOLATILE_LOCK_BITS                       0xE8    /**< Read volatile lock bits. */
#define MT25Q_WRITE_VOLATILE_LOCK_BITS                      0xE5    /**< Write volatile lock bits. */
#define MT25Q_READ_NONVOLATILE_LOCK_BITS                    0xE2    /**< Read nonvolatile lock bits. */
#define MT25Q_WRITE_NONVOLATILE_LOCK_BITS                   0xE3    /**< Write nonvolatile lock bits. */
#define MT25Q_ERASE_NONVOLATILE_LOCK_BITS                   0xE4    /**< Erase nonvolatile lock bits. */
#define MT25Q_READ_GLOBAL_FREEZE_BIT                        0xA7    /**< Read global freeze bit. */
#define MT25Q_WRITE_GLOBAL_FREEZE_BIT                       0xA6    /**< Write global freeze bit. */
#define MT25Q_READ_PASSWORD                                 0x27    /**< Read password. */
#define MT25Q_WRITE_PASSWORD                                0x28    /**< Write password. */
#define MT25Q_UNLOCK_PASSWORD                               0x29    /**< Unlock password. */

/* Advanced Sector Protection Operations With 4-Byte Address */
#define MT25Q_4_BYTE_READ_VOLATILE_LOCK_BITS                0xE0    /**< 4-byte read volatile lock bits. */
#define MT25Q_4_BYTE_WRITE_VOLATILE_LOCK_BITS               0xE1    /**< 4-byte write volatile lock bits. */

/* Advanced Function Interface Operations */
#define MT25Q_INTERFACE_ACTIVATION                          0x9B    /**< Interface activation. */
#define MT25Q_CYCLIC_REDUNDANCY_CHECK                       0x27    /**< Cyclic redundancy check. */


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
