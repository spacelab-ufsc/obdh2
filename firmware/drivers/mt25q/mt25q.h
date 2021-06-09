/*
 * mt25q.h
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
 * \version 0.6.35
 * 
 * \date 2019/11/15
 * 
 * \defgroup mt25q MT25Q
 * \ingroup drivers
 * \{
 */

#ifndef MT25Q_H_
#define MT25Q_H_

#include <stdint.h>
#include <stdbool.h>

#define MT25Q_MODULE_NAME                   "MT25Q"

#define MT25Q_MANUFACTURER_ID               0x20        /**< Manufacturer ID. */

/* Memory type */
#define MT25Q_MEMORY_TYPE_3V                0xBA        /**< 3 V. */
#define MT25Q_MEMORY_TYPE_1V8               0xBB        /**< 1.8 V. */

/* MT25Q memory capacity */
#define MT25Q_MEMORY_CAPACITY_2GB           0x22        /**< 2 Gb. */
#define MT25Q_MEMORY_CAPACITY_1GB           0x21        /**< 1 Gb. */
#define MT25Q_MEMORY_CAPACITY_512MB         0x20        /**< 512 Mb. */
#define MT25Q_MEMORY_CAPACITY_256MB         0x19        /**< 256 Mb. */
#define MT25Q_MEMORY_CAPACITY_128MB         0x18        /**< 128 Mb. */
#define MT25Q_MEMORY_CAPACITY_64MB          0x17        /**< 64 Mb. */

/* Write in progress status */
#define MT25Q_READY                         0           /**< The memory is ready to write data. */
#define MT25Q_BUSY                          1           /**< The memoty is busy writing data. */

/* Write protection */
#define MT25Q_WRITE_DISABLED                0           /**< Write operations disabled. */
#define MT25Q_WRITE_ENABLED                 1           /**< Write operations enabled. */

#define MT25Q_DISCOVERY_TABLE_1             0x0C        /**< Discovery table 1. */
#define MT25Q_DTABLE_1_SECTOR_DESCRIPTOR    0x1C        /**< Discovery table 1 sector descriptor. */
#define MT25Q_DTABLE_1_FLASH_SIZE           0x04        /**< Discovery table 1 flash size. */

#define MT25Q_SIZE_16MB                     0x01000000  /**< 16 MB in bytes. */
#define MT25Q_SIZE_64MB                     0x04000000  /**< 64 MB in bytes. */
#define MT25Q_SIZE_128MB                    0x08000000  /**< 128 MB in bytes. */
#define MT25Q_SIZE_256MB                    0x10000000  /**< 256 MB in bytes. */

#define MT25Q_SECTOR_ERASE_TIMEOUT_MS       3000        /**< Sector erase timeout in milliseconds. */
#define MT25Q_PROGRAM_TIMEOUT_MS            1000        /**< Program timeout in milliseconds. */

/**
 * \brief Sector type.
 */
typedef uint16_t mt25q_sector_t;

/**
 * \brief Device ID structure.
 */
typedef struct
{
    uint8_t manufacturer_id;
    uint8_t memory_type;
    uint8_t memory_capacity;
} mt25q_dev_id_t;

/**
 * \brief Flash description structure.
 */
typedef struct
{
    uint32_t id;
    uint32_t type;
    uint32_t starting_address;
    uint32_t address_mask;
    uint32_t size;
    uint32_t otp_size;
    uint32_t die_count;
    uint32_t die_size;
    uint32_t die_size_bit;

    uint32_t sector_size;
    uint32_t sector_size_bit;
    uint32_t sector_count;
    uint32_t sector_erase_cmd;

    uint32_t sub_sector_size;
    uint32_t sub_sector_size_bit;
    uint32_t sub_sector_count;
    uint32_t sub_sector_erase_cmd;

    uint32_t page_size;
    uint32_t page_count;

    uint32_t buffer_size;
    uint32_t data_width;

    uint8_t num_adr_byte;
} flash_description_t;

/**
 * \brief Status structure.
 */
typedef struct
{
    uint8_t write_enable;                       /**< Write enable/disable of the status register. */
    uint8_t top_bottom;                         /**< Determines whether the protected memory area defined by the block protect bits starts from the top or bottom of the memory array. */
    uint8_t bp;                                 /**< Defines memory to be software protected against PROGRAM or ERASE operations. */
    uint8_t write_enable_latch;                 /**< The device always powers up with this bit cleared to prevent inadvertent WRITE, PROGRAM, or ERASE operation. */
    uint8_t write_in_progress;                  /**< Indicates a command cycle is in progress. */
} mt25q_status_t;

/**
 * \brief Address modes.
 */
typedef enum
{
    MT25Q_ADDRESS_MODE_3_BYTE=3,                /**< 3 byte address mode. */
    MT25Q_ADDRESS_MODE_4_BYTE                   /**< 4 byte address mode. */
} mt25q_address_mode_e;

/**
 * \brief Driver initialization.
 *
 * \return The status/error code.
 */
int mt25q_init(void);

/**
 * \brief Resets the device by software.
 *
 * To reset the device, the RESET ENABLE command must be followed by the RESET MEMORY
 * command. When the two commands are executed, the device enters a power-on reset condition.
 * It is recommended to exit XIP mode before executing these two commands.
 *
 * All volatile lock bits, the volatile configuration register, the enhanced volatile configuration
 * register, and the extended address register are reset to the power-on reset default
 * condition according to nonvolatile configuration register settings.
 *
 * If a reset is initiated while a WRITE, PROGRAM, or ERASE operation is in progress or suspended,
 * the operation is aborted and data may be corrupted.
 *
 * Reset is effective after the flag status register bit 7 outputs 1 with at least one byte output.
 *
 * A RESET ENABLE command is not accepted during WRITE STATUS REGISTER and WRITE
 * NONVOLATILE CONFIGURATION REGISTER operations.
 *
 * \return The status/error code.
 */
int mt25q_reset(void);

/**
 * \brief Reads the device ID information.
 *
 * \param[in,out] dev_id is a pointer to store the read device ID.
 *
 * \return The status/error code.
 */
int mt25q_read_device_id(mt25q_dev_id_t *dev_id);

/**
 * \brief Reads the description data of the flash memory.
 *
 * \param[in,out] fdo is a pointer to store the read data into a flash description structure.
 *
 * \return The status/error code.
 */
int mt25q_read_flash_description(flash_description_t *fdo);

/**
 * \brief Clears the flag status register.
 *
 * \return The status/error code.
 */
int mt25q_clear_flag_status_register(void);

/**
 * \brief Reads the status register.
 *
 * Can be read continuously and at any time, including during a PROGRAM, ERASE, or WRITE operation.
 * If one of these operations is in progress, checking the write in progress bit or P/E controller bit is
 * recommended before executing the command.
 *
 * \param[in,out] status is a pointer to the status structure to store the read data.
 *
 * \return The status/error code.
 */
int mt25q_read_status(mt25q_status_t *status);

/**
 * \brief Put the device in deep power-down.
 *
 * The command is used to place the device in deep power-down mode for the lowest device
 * power consumption, with device current reduced to I_CC2. This command can also be used as
 * a software protection mechanism while the device is not in active use.
 *
 * \return The status/error code.
 */
int mt25q_enter_deep_power_down(void);

/**
 * \brief Release the device from the deep power-down.
 *
 * The command is used to exit from deep power-down mode. The device also exits deep
 * power-down mode upon:
 * \parblock
 *      - A power-down, entering standby mode with the next power-up.
 *      - A hardware or software reset operation, entering standby mode with a recovery time as
 *      specified in the AC Reset Specifications.
 *      .
 * \endparblock
 *
 * \return The status/error code.
 */
int mt25q_release_from_deep_power_down(void);

/**
 * \brief Sets the write enable latch bit before each PROGRAM, ERASE, and WRITE command.
 *
 * \return The status/error code.
 */
int mt25q_write_enable(void);

/**
 * \brief Clears the write enable latch bit.
 *
 * In case of a protection error, WRITE DISABLE will not clear the bit. Instead, a
 * CLEAR FLAG STATUS REGISTER command must be issued to clear both flags.
 *
 * \return The status/error code.
 */
int mt25q_write_disable(void);

/**
 * \brief Checks if the memory is busy or not.
 *
 * This function checks the Write In Progress (WIP) bit to determine whether the Flash memory is busy
 * with a Write, Program or Erase cycle.
 *
 * \return TRUE/FALSE if the memory is busy or not.
 */
bool mt25q_is_busy(void);

/**
 * \brief Erases the whole die.
 *
 * \param[in] die is the die number to erase.
 *
 * \return The status/error code.
 */
int mt25q_die_erase(mt25q_sector_t die);

/**
 * \brief Erases a given memory sector.
 *
 * \param[in] sector is the memory sector to erase.
 *
 * \return The status/error code.
 */
int mt25q_sector_erase(mt25q_sector_t sector);

/**
 * \brief Erases a given memory sub-sector.
 *
 * \param[in] sub is the memory sub-sector to erase.
 *
 * \return The status/error code.
 */
int mt25q_sub_sector_erase(mt25q_sector_t sub);

/**
 * \brief Writes data to a given address.
 *
 * \param[in] adr is the address to write.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int mt25q_write(uint32_t adr, uint8_t *data, uint16_t len);

/**
 * \brief Reads data from a given address.
 *
 * \param[in] adr is the address to write.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int mt25q_read(uint32_t adr, uint8_t *data, uint16_t len);

/**
 * \brief Gets the maximum address value of the memory.
 *
 * \return The maximum address value of the memory.
 */
uint32_t mt25q_get_max_address(void);

/**
 * \brief Enables the 4-byte address mode.
 *
 * \return The status/error code.
 */
int mt25q_enter_4_byte_address_mode(void);

/**
 * \brief Reads the flag status register.
 *
 * \param[in,out] flag is a pointer to store the read value of the flag status.
 *
 * \return The status/error code.
 */
int mt25q_read_flag_status_register(uint8_t *flag);

/**
 * \brief Gets the flash description structure of the device.
 *
 * \return The flash description structure.
 */
flash_description_t mt25q_get_flash_description(void);

/**
 * \brief SPI interface initialization.
 *
 * \return The status/error code.
 */
int mt25q_spi_init(void);

/**
 * \brief Writes the device using the SPI interface.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int mt25q_spi_write(uint8_t *data, uint16_t len);

/**
 * \brief Reads the device using the SPI interface.
 *
 * \param[in] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int mt25q_spi_read(uint8_t *data, uint16_t len);

/**
 * \brief SPI transfer operation (write and/or read).
 *
 * \param[in,out] wdata is a pointer to the data to be written during the SPI transfer.
 *
 * \param[in,out] rdata is a pointer to store the read data during the SPI transfer.
 *
 * \param[in] len is the number of bytes of the transfer operation.
 *
 * \return The status/error code.
 */
int mt25q_spi_transfer(uint8_t *wdata, uint8_t *rdata, uint16_t len);

/**
 * \brief Selects the device manually (CS pin low).
 *
 * \return The status/error code.
 */
int mt25q_spi_select(void);

/**
 * \brief Unselects the device manually (CS pin high).
 *
 * \return The status/error code.
 */
int mt25q_spi_unselect(void);

/**
 * \brief Writes data without automatically select the device.
 *
 * \param[in] data is a pointer to the bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int mt25q_spi_write_only(uint8_t *data, uint16_t len);

/**
 * \brief Reads data without automatically select the device.
 *
 * \param[in,out] data is a pointer to to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int mt25q_spi_read_only(uint8_t *data, uint16_t len);

/**
 * \brief Transfer data without automatically select the device.
 *
 * \param[in] wdata is a pointer to the bytes to write.
 *
 * \param[in,out] rdata is a pointer to store the read bytes.
 *
 * \param[in] len is the number of bytes to transfer (read and write at the same time).
 *
 * \return The status/error code.
 */
int mt25q_spi_transfer_only(uint8_t *wdata, uint8_t *rdata, uint16_t len);

/**
 * \brief Initialization of the GPIO pins.
 *
 * This function initializes the pins "HOLD" and "RESET".
 *
 * \return The status/error code.
 */
int mt25q_gpio_init(void);

/**
 * \brief Sets the state of the hold pin.
 *
 * \param[in] state is the state to set the hold pin (high/low or true/false).
 *
 * \return The status/error code.
 */
int mt25q_gpio_set_hold(bool state);

/**
 * \brief Sets the state of the reset pin.
 *
 * \param[in] state is the state to set the reset pin (high/low or true/false).
 *
 * \return The status/error code.
 */
int mt25q_gpio_set_reset(bool state);

/**
 * \brief Milliseconds delay.
 *
 * \param[in] ms is the delay period in milliseconds.
 *
 * \return None.
 */
void mt25q_delay_ms(uint32_t ms);

#endif /* MT25Q_H_ */

/** \} End of mt25q group */
