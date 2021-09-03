/*
 * cy15x102qn.h
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
 * \brief CY15x102QN driver definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.1
 * 
 * \date 2021/06/04
 * 
 * \defgroup cy15x102qn CY15x102QN
 * \ingroup drivers
 * \{
 */

#ifndef CY15X102QN_H_
#define CY15X102QN_H_

#include <stdint.h>

#include <drivers/spi/spi.h>
#include <drivers/gpio/gpio.h>

#define CY15X102QN_MODULE_NAME              "CY15X102QN"

/* Commands */
#define CY15X102QN_OPCODE_WREN              0x06    /**< Set write enable latch. */
#define CY15X102QN_OPCODE_WRDI              0x04    /**< Reset write enable latch. */
#define CY15X102QN_OPCODE_RDSR              0x05    /**< Read Status Register. */
#define CY15X102QN_OPCODE_WRSR              0x01    /**< Write Status Register. */
#define CY15X102QN_OPCODE_WRITE             0x02    /**< Write memory data. */
#define CY15X102QN_OPCODE_READ              0x03    /**< Read memory data. */
#define CY15X102QN_OPCODE_FSTRD             0x0B    /**< Fast read memory data. */
#define CY15X102QN_OPCODE_SSWR              0x42    /**< Special Sector Write. */
#define CY15X102QN_OPCODE_SSRD              0x4B    /**< Special Sector Read. */
#define CY15X102QN_OPCODE_RDID              0x9F    /**< Read device ID. */
#define CY15X102QN_OPCODE_RUID              0x4C    /**< Read Unique ID. */
#define CY15X102QN_OPCODE_WRSN              0xC2    /**< Write Serial Number. */
#define CY15X102QN_OPCODE_SNR               0xC3    /**< Read Serial Number. */
#define CY15X102QN_OPCODE_DPD               0xBA    /**< Enter Deep Power-Down. */
#define CY15X102QN_OPCODE_HBN               0xB9    /**< Enter Hibernate Mode. */


/**
 * \brief Status type.
 */
typedef uint8_t cy15x102qn_status_t;

/**
 * \brief UID type.
 */
typedef uint64_t cy15x102qn_uid_t;

/**
 * \brief Device ID type.
 */
typedef struct
{
    uint64_t manufacturer_id:56;                    /**< Manufacturer ID. */
    uint8_t family:3;                               /**< Family. */
    uint8_t density:4;                              /**< Density. */
    uint8_t inrush:1;                               /**< Inrush. */
    uint8_t sub_type:3;                             /**< Sub Type. */
    uint8_t revision:2;                             /**< Revision. */
    uint8_t voltage:1;                              /**< Voltage. */
    uint8_t frequency:2;                            /**< Frequency. */
} cy15x102qn_device_id_t;

/**
 * \brief Serial number type.
 */
typedef struct
{
    uint16_t customer_id;                           /**< 16-bit Customer Identifier. */
    uint64_t unique_number:40;                      /**< 40-bit Unique Number. */
    uint8_t crc;                                    /**< 8-bit CRC. */
} cy15x102qn_serial_number_t;

/**
 * \brief Configuration parameters.
 */
typedef struct
{
    spi_port_t port;                                /**< SPI port. */
    spi_cs_t cs_pin;                                /**< Chip-Select pin. */
    uint32_t clock_hz;                              /**< SPI clock in Hertz. */
    gpio_pin_t wp_pin;                              /**< Write protection GPIO pin. */
} cy15x102qn_config_t;

/**
 * \brief Driver initialization.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_init(cy15x102qn_config_t *conf);

/**
 * \brief Set Write Enable Latch (WREN, 06h).
 *
 * The CY15X102QN will power up with writes disabled. The WREN command must be issued before any write operation.
 * Sending the WREN opcode allows the user to issue subsequent opcodes for write operations. These include
 * writing to the Status Register (WRSR), the memory (WRITE), Special Sector (SSWR), and Write Serial Number
 * (WRSN).
 *
 * Sending the WREN opcode causes the internal Write Enable Latch to be set. A flag bit in the Status Register,
 * called WEL, indicates the state of the latch. WEL = '1' indicates that writes are permitted. Attempting to
 * write the WEL bit in the Status Register has no effect on the state of this bit - only the WREN opcode can
 * set this bit. The WEL bit will be automatically cleared on the rising edge of CS following a WRDI, a WRSR, a
 * WRITE, a SSWR, or a WRSN operation. This prevents further writes to the Status Register or the F-RAM array
 * without another WREN command.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_set_write_enable(cy15x102qn_config_t *conf);

/**
 * \brief Reset Write Enable Latch (WRDI, 04h).
 *
 * The WRDI command disables all write activity by clearing the Write Enable Latch. Verify that the writes are
 * disabled by reading the WEL bit in the Status Register and verify that WEL is equal to '0'.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_reset_write_enable(cy15x102qn_config_t *conf);

/**
 * \brief Read Status Register (RDSR, 05h).
 *
 * The RDSR command allows the bus master to verify the contents of the Status Register. Reading the status
 * register provides information about the current state of the write-protection features. Following the RDSR
 * opcode, the CY15X102QN will return one byte with the contents of the Status Register.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in,out] status is a pointer to store the status bits of the device.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_read_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t *status);

/**
 * \brief Write Status Register (WRSR, 01h).
 *
 * The WRSR command allows the SPI bus master to write into the Status Register and change the write protect
 * configuration by setting the WPEN, BP0, and BP1 bits as required. Before issuing a WRSR command, the WP pin
 * must be HIGH or inactive. Note that on the CY15X102QN, WP only prevents writing to the Status Register, not
 * the memory array. Before sending the WRSR command, the user must send a WREN command to enable writes.
 * Executing a WRSR command is a write operation and therefore, clears the Write Enable Latch.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] status is the new status bits value.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_write_status_reg(cy15x102qn_config_t *conf, cy15x102qn_status_t status);

/**
 * \brief Write Operation (WRITE, 02h).
 *
 * All writes to the memory begin with a WREN opcode with CS being asserted and deasserted. The next opcode is
 * WRITE. The WRITE opcode is followed by a three-byte address containing the 18-bit address (A17–A0) of the
 * first data byte to be written into the memory. The upper six bits of the three-byte address are ignored.
 * Subsequent bytes are data bytes, which are written sequentially. Addresses are incremented internally as
 * long as the bus master continues to issue clocks and keeps CS LOW. If the last address of 3FFFFh is reached,
 * the internal address counter will roll over to 00000h. Data is written on MSb first. The rising edge of CS
 * terminates a write operation.
 *
 * \note When a burst write reaches a protected block address, the automatic address increment stops and all
 * the subsequent data bytes received for write will be ignored by the device. EEPROMs use page buffers to
 * increase their write throughput. This compensates for the technology's inherently slow write operations.
 * F-RAM memories do not have page buffers because each byte is written to the F-RAM array immediately after it
 * is clocked in (after the eighth clock). This allows any number of bytes to be written without page buffer
 * delays.
 *
 * \note If power is lost in the middle of the write operation, only the last completed byte will be written.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] adr is the address to write the given data.
 *
 * \param[in] data is the data to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_write(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief Read Operation (READ, 03h).
 *
 * After the falling edge of CS, the bus master can issue a READ opcode. Following the READ command is a
 * three-byte address containing the 18-bit address (A17–A0) of the first byte of the read operation. The upper
 * six bits of the address are ignored. After the opcode and address are issued, the device drives out the read
 * data on the next eight clocks. The SI input is ignored during read data bytes. Subsequent bytes are data
 * bytes, which are read out sequentially. Addresses are incremented internally as long as the bus master
 * continues to issue clocks and CS is LOW. If the last address of 3FFFFh is reached, the internal address
 * counter will roll over to 00000h. Data is read on MSb first. The rising edge of CS terminates a read
 * operation and tristates the SO pin.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] adr is the address to read.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief Fast Read Operation (FAST_READ, 0Bh).
 *
 * The CY15X102QN supports a FAST READ opcode (0Bh) that is provided for opcode compatibility with serial flash
 * devices. The FAST READ opcode is followed by a three-byte address containing the 18-bit address (A17–A0) of
 * the first byte of the read operation and then a dummy byte. The dummy byte inserts a read latency of 8-clock
 * cycle. The fast read operation is otherwise the same as an ordinary read operation except that it requires
 * an additional dummy byte. After receiving the opcode, address, and a dummy byte, the CY15X102QN starts
 * driving its SO line with data bytes, with MSB first, and continues transmitting as long as the device is
 * selected and the clock is available. In case of bulk read, the internal address counter is incremented
 * automatically, and after the last address 3FFFFh is reached, the counter rolls over to 00000h. When the
 * device is driving data on its SO line, any transition on its SI line is ignored. The rising edge of CS
 * terminates a fast read operation and tristates the SO pin. 
 *
 * \note The dummy byte can be any 8-bit value but Axh (8'b1010xxxx). The lower 4 bits of Axh are don't care
 * bits. Hence, Axh essentially represents 16 different 8-bit values which shouldn't be transmitted as the
 * dummy byte. 00h is typically used as the dummy byte in most use cases.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] adr is the address to read.
 *
 * \param[in,out] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_fast_read(cy15x102qn_config_t *conf, uint32_t adr, uint8_t *data, uint32_t len);

/**
 * \brief Special Sector Write (SSWR, 42h).
 *
 * All writes to the 256-byte special begin with a WREN opcode with CS being asserted and deasserted. The next
 * opcode is SSWR. The SSWR opcode is followed by a three-byte address containing the 8-bit sector address
 * (A7–A0) of the first data byte to be written into the special sector memory. The upper 16 bits of the
 * three-byte address are ignored. Subsequent bytes are data bytes, which are written sequentially. Addresses
 * are incremented internally as long as the bus master continues to issue clocks and keeps CS LOW. Once the
 * internal address counter auto increments to XXXFFh, CS should toggle HIGH to terminate the ongoing SSWR
 * operation. Data is written on MSb first. The rising edge of CS terminates a write operation.
 *
 * \note If power is lost in the middle of the write operation, only the last completed byte will be written.
 *
 * \note The special sector F-RAM memory guarantees to retain data integrity up to three cycles of standard
 * reflow soldering.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] adr is the special sector address to write (8-bits).
 *
 * \param[in] data is the data to write in the special sector.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_special_sector_write(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len);

/**
 * \brief Special Sector Read (SSRD, 4Bh).
 *
 * After the falling edge of CS, the bus master can issue an SSRD opcode. Following the SSRD command is a
 * three-byte address containing the 8-bit address (A7–A0) of the first byte of the special sector read
 * operation. The upper 16 bits of the address are ignored. After the opcode and address are issued, the device
 * drives out the read data on the next eight clocks. The SI input is ignored during read data bytes. Subsequent
 * bytes are data bytes, which are read out sequentially. Addresses are incremented internally as long as the
 * bus master continues to issue clocks and CS is LOW. Once the internal address counter auto increments to
 * XXXFFh, CS should toggle HIGH to terminate the ongoing SSRD operation. Data is read on MSb first. The rising
 * edge of CS terminates a special sector read operation and tristates the SO pin.
 *
 * \note The special sector F-RAM memory guarantees to retain data integrity up to three cycles of standard
 * reflow soldering.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] is the special sector address to read (8-bits).
 *
 * \param[in] data is a pointer to store the read data.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_special_sector_read(cy15x102qn_config_t *conf, uint8_t adr, uint8_t *data, uint16_t len);

/**
 * \brief Read Device ID (RDID, 9Fh).
 *
 * The CY15X102QN device can be interrogated for its manufacturer, product identification, and die revision.
 * The RDID opcode 9Fh allows the user to read the 9-byte manufacturer ID and product ID, both of which are
 * read-only bytes. The JEDEC-assigned manufacturer ID places the Cypress (Ramtron) identifier in bank 7;
 * therefore, there are six bytes of the continuation code 7Fh followed by the single byte C2h. There are two
 * bytes of product ID, which includes a family code, a density code, a sub code, and the product revision code.
 * Table 6 shows 9-Byte Device ID field description. Refer to Ordering Information on page 24 for 9-Byte device
 * ID of an individual part.
 *
 * \note The least significant data byte (Byte 0) shifts out first and the most significant data byte (Byte 8)
 * shifts out last.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in,out] id is a pointer to store the read device ID.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_read_device_id(cy15x102qn_config_t *conf, cy15x102qn_device_id_t *id);

/**
 * \brief Read Unique ID (RUID, 4Ch).
 *
 * The CY15X102QN device can be interrogated for unique ID which is a factory programmed, 64-bit number unique
 * to each device. The RUID opcode, 4Ch allows to read the 8-byte, read only unique ID.
 *
 * \note The least significant data byte (Byte 0) shifts out first and the most significant data byte (Byte 7)
 * shifts out last.
 *
 * \note The unique ID registers are guaranteed to retain data integrity of up to three cycles of the standard
 * reflow soldering.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] uid is a pointer to store the read unique ID.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_read_unique_id(cy15x102qn_config_t *conf, cy15x102qn_uid_t *uid);

/**
 * \brief Write Serial Number (WRSN, C2h).
 *
 * The serial number is an 8-byte one-time programmable memory space provided to the user to uniquely identify
 * a PC board or a system. A serial number typically consists of a two-byte Customer ID, followed by five bytes
 * of a unique serial number and one byte of CRC check. However, the end application can define its own format
 * for the 8-byte serial number. All writes to the Serial Number Register begin with a WREN opcode with CS being
 * asserted and deasserted. The next opcode is WRSN. The WRSN instruction can be used in burst mode to write all
 * the 8 bytes of serial number. After the last byte of the serial number is shifted in, CS must be driven HIGH
 * to complete the WRSN operation.
 *
 * \note The CRC checksum is not calculated by the device. The system firmware must calculate the CRC checksum
 * on the 7-byte content and append the checksum to the 7-byte user-defined serial number before programming the
 * 8-byte serial number into the serial number register. The factory default value for the 8-byte Serial Number
 * is '0000000000000000h'.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] s_num is the serial number to write.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_write_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t s_num);

/**
 * \brief Read Serial Number (RDSN, C3h).
 *
 * The CY15X102QN device incorporates an 8-byte serial space provided to the user to uniquely identify the
 * device. The serial number is read using the RDSN instruction. A serial number read may be performed in burst
 * mode to read all the eight bytes at once. After the last byte of the serial number is read, the device loops
 * back to the first (MSB) byte of the serial number. An RDSN instruction can be issued by shifting the opcode
 * for RDSN after CS goes LOW.
 *
 * \note The least significant data byte (Byte 0) shifts out first and the most significant data byte (Byte 7)
 * shifts out last.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in,out] s_num is a pointer to store the read serial number.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_read_serial_number(cy15x102qn_config_t *conf, cy15x102qn_serial_number_t *s_num);

/**
 * \brief Deep Power-Down Mode (DPD, BAh).
 *
 * A power-saving Deep Power-Down mode is implemented on the CY15X102QN device. The device enters the Deep
 * Power-Down mode after tENTDPD time after the DPD opcode BAh is clocked in and a rising edge of CS is applied.
 * When in Deep-Power-Down mode, the SCK and SI pins are ignored and SO will be Hi-Z, but the device continues
 * to monitor the CS pin.
 *
 * A CS pulse-width of tCSDPD exits the DPD mode after tEXTDPD time. The CS pulse-width can be generated either
 * by sending a dummy command cycle or toggling CS alone while SCK and I/Os are don’t care. The I/Os remain in
 * hi-Z state during the wakeup from deep power-down.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_deep_power_down_mode(cy15x102qn_config_t *conf);

/**
 * \brief Hibernate Mode (HBN, B9h).
 *
 * A lowest power Hibernate mode is implemented on the CY15X102QN device. The device enters Hibernate mode after
 * tENTHIB time after the HBN opcode B9h is clocked in and a rising edge of CS is applied. When in Hibernate
 * mode, the SCK and SI pins are ignored and SO will be Hi-Z, but the device continues to monitor the CS pin.
 * On the next falling edge of CS, the device will return to normal operation within tEXTHIB time. The SO pin
 * remains in a Hi-Z state during the wakeup from hibernate period. The device does not necessarily respond to
 * an opcode within the wakeup period. To exit the Hibernate mode, the controller may send a "dummy" read, for
 * example, and wait for the remaining tEXTHIB time.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \see Excelon-Auto 2-Mbit (256K x 8) Automotive-E Serial (SPI) F-RAM.
 *
 * \return The stauts/error code
 */
int cy15x102qn_hibernate_mode(cy15x102qn_config_t *conf);

/**
 * \brief SPI interface initialization.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_init(cy15x102qn_config_t *conf);

/**
 * \brief Writes the device using the SPI interface.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_write(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

/**
 * \brief Reads the device using the SPI interface.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] data is an array to store the read bytes.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_read(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

/**
 * \brief SPI transfer operation (write and/or read).
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in,out] wdata is a pointer to the data to be written during the SPI transfer.
 *
 * \param[in,out] rdata is a pointer to store the read data during the SPI transfer.
 *
 * \param[in] len is the number of bytes of the transfer operation.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_transfer(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len);

/**
 * \brief Selects the device manually (CS pin low).
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_select(cy15x102qn_config_t *conf);

/**
 * \brief Unselects the device manually (CS pin high).
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_unselect(cy15x102qn_config_t *conf);

/**
 * \brief Writes data without automatically select the device.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] data is a pointer to the bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_write_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

/**
 * \brief Reads data without automatically select the device.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in,out] data is a pointer to to store the read bytes.
 *
 * \param[in] len is the number of bytes to read.
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_read_only(cy15x102qn_config_t *conf, uint8_t *data, uint16_t len);

/**
 * \brief Transfer data without automatically select the device.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \param[in] wdata is a pointer to the bytes to write.
 *
 * \param[in,out] rdata is a pointer to store the read bytes.
 *
 * \param[in] len is the number of bytes to transfer (read and write at the same time).
 *
 * \return The status/error code.
 */
int cy15x102qn_spi_transfer_only(cy15x102qn_config_t *conf, uint8_t *wdata, uint8_t *rdata, uint16_t len);

/**
 * \brief Initialization of the GPIO pins.
 *
 * This function initializes the pin "WP".
 *
 * This Active LOW pin prevents write operation to the Status Register when WPEN bit in the Status Register is
 * set to '1'. This is critical because other write protection features are controlled through the Status
 * Register.
 *
 * \note  This pin has an internal weak pull-up resistor which keeps this pin HIGH if left floating (not
 * connected on the board).
 *
 * \note This pin can also be tied to VDD if not used.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_gpio_init(cy15x102qn_config_t *conf);

/**
 * \brief Sets the state of the hold pin.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_gpio_set_write_protect(cy15x102qn_config_t *conf);

/**
 * \brief Sets the state of the reset pin.
 *
 * \param[in,out] conf is a pointer to the configuration parameters of the device.
 *
 * \return The status/error code.
 */
int cy15x102qn_gpio_clear_write_protect(cy15x102qn_config_t *conf);

#endif /* CY15X102QN_H_ */

/** \} End of cy15x102qn group */
