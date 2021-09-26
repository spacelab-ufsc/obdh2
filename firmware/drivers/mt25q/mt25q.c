/*
 * mt25q.c
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
 * \brief MT25Q driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.21
 * 
 * \date 2019/11/15
 * 
 * \addtogroup mt25q
 * \{
 */

#include <string.h>

#include "mt25q.h"
#include "mt25q_reg.h"

#define MT25Q_DUMMY_BYTE        0x00

flash_description_t mt25q_fdo = {0};

/**
 * \brief Writes data into the memory.
 *
 * \param[in] adr is the address to write.
 *
 * \param[in] data is an array of bytes to write.
 *
 * \param[in] len is the number of bytes to write.
 *
 * \param[in] instr is the SPI instruction.
 *
 * \return The status/error code.
 */
int mt25q_gen_program(uint32_t adr, uint8_t *data, uint32_t len, uint8_t instr);

int mt25q_init(void)
{
    if (mt25q_spi_init() != 0)
    {
        return -1;
    }

    if (mt25q_gpio_init() != 0)
    {
        return -1;
    }

    mt25q_delay_ms(10);

    if (mt25q_read_flash_description(&mt25q_fdo) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_read_device_id(mt25q_dev_id_t *dev_id)
{
    uint8_t cmd[4] = {MT25Q_READ_ID_REG, MT25Q_DUMMY_BYTE, MT25Q_DUMMY_BYTE, MT25Q_DUMMY_BYTE};
    uint8_t ans[4] = {MT25Q_DUMMY_BYTE};

    if (mt25q_spi_transfer(cmd, ans, 4) != 0)
    {
        return -1;
    }

    dev_id->manufacturer_id  = ans[1];
    dev_id->memory_type      = ans[2];
    dev_id->memory_capacity  = ans[3];

    return 0;
}

int mt25q_read_flash_description(flash_description_t *fdo)
{
    /* Read device ID */
    mt25q_dev_id_t dev_id = {0};

    if (mt25q_read_device_id(&dev_id) != 0)
    {
        return -1;
    }

    /* Check the manufacturer */
    if (dev_id.manufacturer_id != MT25Q_MANUFACTURER_ID)
    {
        return -1;  /* Unexpected manufacturer! */
    }

    uint32_t device = ((uint32_t)dev_id.manufacturer_id << 16) |
                      ((uint32_t)dev_id.memory_type << 8) |
                      ((uint32_t)dev_id.memory_capacity);

    fdo->id = device;

    device &= 0xFF;
    if (device >= 0x20)
    {
        device -= 6;    /* WTF?? */
    }

    fdo->size = 1 << device;
    fdo->num_adr_byte = 3;

    /* Read the discovery parameter signature */
    uint8_t cmd[350] = {MT25Q_DUMMY_BYTE};
    uint8_t ans[350] = {MT25Q_DUMMY_BYTE};

    cmd[0] = MT25Q_READ_SERIAL_FLASH_DISCOVERY_PARAMETER_REG;

    if (mt25q_spi_transfer(cmd, ans, 350) != 0)
    {
        return -1;
    }

    /* Remove the SPI command, address and dummy clocks */
    memcpy(ans, ans+5, 350);    /* 5 = SPI command + address + dummy clocksy */

    /* Check if the read data is valid */
    if (memcmp(ans, "SFDP", 4))
    {
        return -1;  /* Not flash description data */
    }

    /* The parameter table pointer is at MT25Q_DISCOVERY_TABLE_1 */
    uint32_t table_address = (uint32_t)ans[MT25Q_DISCOVERY_TABLE_1] |
                             ((uint32_t)ans[MT25Q_DISCOVERY_TABLE_1 + 1] << 8) |
                             ((uint32_t)ans[MT25Q_DISCOVERY_TABLE_1 + 2] << 16);

    /* Get the official device size in bytes */
    fdo->size = (*((uint32_t*)&ans[table_address + MT25Q_DTABLE_1_FLASH_SIZE]) + 1)/8;

    /* Get the largest sector size and the sector count, and take one sub-sector size and sub-sector count. */
    /* The first two sector definitions have the definitions that we use - usually 4K and 64K. */
    uint32_t t_offset = table_address + MT25Q_DTABLE_1_SECTOR_DESCRIPTOR + 2;

    if (ans[t_offset] != 0)
    {
        fdo->sector_size_bit        = ans[t_offset];
        /* fdo->sector_size            = (1 << (ans[t_offset])); */
        fdo->sector_size            = 1;
        uint8_t i = 0;
        for(i=0; i<ans[t_offset]; i++)
        {
            fdo->sector_size        = (fdo->sector_size << 1);
        }
        fdo->sector_count           = fdo->size / fdo->sector_size;
        fdo->sector_erase_cmd       = ans[t_offset + 1];

        fdo->sub_sector_size_bit    = ans[t_offset - 2];
        /* fdo->sub_sector_size        = (1 << (ans[t_offset - 2])); */
        fdo->sub_sector_size        = 1;
        for(i=0; i<ans[t_offset-2]; i++)
        {
            fdo->sub_sector_size    = (fdo->sub_sector_size << 1);
        }
        fdo->sub_sector_count       = fdo->size / fdo->sub_sector_size;
        fdo->sub_sector_erase_cmd   = ans[t_offset - 1];
    }

    /* Hard-coded flash parameters */
    fdo->page_size      = 0x100;
    fdo->page_count     = fdo->size / fdo->page_size;
    fdo->address_mask   = 0xFF;

    fdo->otp_size = 0x40;

    /* Initial Die information */
    if (fdo->size > MT25Q_SIZE_64MB)
    {
        fdo->die_count      = fdo->size / MT25Q_SIZE_64MB;
        fdo->die_size       = MT25Q_SIZE_64MB;
        fdo->die_size_bit   = 26;
    }

    /* Auto detect address mode */
    if (fdo->size > MT25Q_SIZE_16MB)
    {
        uint8_t flag = 0;

        if (mt25q_enter_4_byte_address_mode() != 0)
        {
            return -1;
        }

        /* Verify current address mode */
        if (mt25q_read_flag_status_register(&flag) != 0)
        {
            return -1;
        }

        if (flag & 1)   /* Test addressing bit of flag status reg (bit 0) */
        {
            fdo->num_adr_byte = MT25Q_ADDRESS_MODE_4_BYTE;
        }
    }
    else
    {
        fdo->num_adr_byte = MT25Q_ADDRESS_MODE_3_BYTE;
    }

    return 0;
}

int mt25q_read_status(mt25q_status_t *status)
{
    uint8_t cmd[2] = {MT25Q_READ_STATUS_REGISTER, MT25Q_DUMMY_BYTE};
    uint8_t ans[2] = {MT25Q_DUMMY_BYTE};

    if (mt25q_spi_transfer(cmd, ans, 2) != 0)
    {
        return -1;
    }

    status->write_enable        = (ans[1] & MT25Q_REG_STATUS_WRITE_ENABLE) >> 7;
    status->top_bottom          = (ans[1] & MT25Q_REG_STATUS_TOP_BOTTOM) >> 5;
    status->bp                  = (ans[1] & MT25Q_REG_STATUS_BP) >> 2;
    status->bp                  = (((status->bp >> 1) & 0x08) | status->bp) & 0x0F;
    status->write_enable_latch  = (ans[1] & MT25Q_REG_STATUS_WRITE_ENABLE_LATCH) >> 1;
    status->write_in_progress   = (ans[1] & MT25Q_REG_STATUS_WRITE_IN_PROGRESS);

    return 0;
}

int mt25q_reset(void)
{
    uint8_t cmd = MT25Q_RESET_ENABLE;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    cmd = MT25Q_RESET_MEMORY;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_enter_deep_power_down(void)
{
    uint8_t cmd = MT25Q_ENTER_DEEP_POWER_DOWN;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_release_from_deep_power_down(void)
{
    uint8_t cmd = MT25Q_RELEASE_FROM_DEEP_POWER_DOWN;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

int mt25q_write_enable(void)
{
    uint8_t cmd = MT25Q_WRITE_ENABLE;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        mt25q_status_t status = {0};

        mt25q_read_status(&status);

        if (status.write_enable_latch == MT25Q_WRITE_ENABLED)
        {
            return 0;
        }
    }

    return -1;
}

int mt25q_write_disable(void)
{
    uint8_t cmd = MT25Q_WRITE_DISABLE;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    uint8_t i = 0;
    for(i=0; i<UINT8_MAX; i++)
    {
        mt25q_status_t status = {0};

        mt25q_read_status(&status);

        if (status.write_enable_latch == MT25Q_WRITE_DISABLED)
        {
            return 0;
        }
    }

    return -1;
}

bool mt25q_is_busy(void)
{
    mt25q_status_t status = {0};

    if (mt25q_read_status(&status) != 0)
    {
        /* Error reading the status register */
        return true;
    }

    if (status.write_in_progress == MT25Q_READY)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int mt25q_die_erase(mt25q_sector_t die)
{
    /* Validate the sector number input */
    if (die >= mt25q_fdo.die_count)
    {
        return -1;
    }

    uint32_t die_adr = die;
    uint16_t i = 0;
    for(i=0; i<mt25q_fdo.die_size_bit; i++)
    {
        die_adr <<= 1;
    }

	/* Check whether any previous Write, Program or Erase cycle is on-going */
    if (mt25q_is_busy())
    {
        return -1;
    }

    /* Disable Write protection */
    if (mt25q_write_enable() != 0)
    {
        return -1;
    }

    uint8_t cmd = MT25Q_DIE_ERASE;
    uint8_t adr_arr[4] = {0};

    if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
    {
        adr_arr[0] = (uint8_t)((die_adr >> 16) & 0xFF);
        adr_arr[1] = (uint8_t)((die_adr >> 8) & 0xFF);
        adr_arr[2] = (uint8_t)((die_adr >> 0) & 0xFF);
    }
    else
    {
        adr_arr[0] = (uint8_t)((die_adr >> 24) & 0xFF);
        adr_arr[1] = (uint8_t)((die_adr >> 16) & 0xFF);
        adr_arr[2] = (uint8_t)((die_adr >> 8) & 0xFF);
        adr_arr[3] = (uint8_t)((die_adr >> 0) & 0xFF);
    }

    if (mt25q_spi_select() != 0)
    {
        return -1;
    }

    /* Write the erase command */
    if (mt25q_spi_write_only(&cmd, 1) != 0)
    {
        return -1;
    }

    /* Write the address */
    if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) != 0)
    {
        return -1;
    }

    if (mt25q_spi_unselect() != 0)
    {
        return -1;
    }

    /* Wait till complete */
    for(i=0; i<MT25Q_SECTOR_ERASE_TIMEOUT_MS; i++)
    {
        if (!mt25q_is_busy())
        {
            break;
        }

        mt25q_delay_ms(1);
    }

    uint8_t flag = 0;
    if (mt25q_read_flag_status_register(&flag) != 0)
    {
        return -1;
    }

    if (mt25q_clear_flag_status_register() != 0)
    {
        return -1;
    }

    if (i >= MT25Q_SECTOR_ERASE_TIMEOUT_MS)
    {
        return -1;  /* Timeout reached */
    }

    return 0;
}

int mt25q_sector_erase(mt25q_sector_t sector)
{
    /* Validate the sector number input */
    if (mt25q_fdo.sector_count < sector)
    {
        return -1;  /* The sector does not exist! */
    }

    uint32_t sector_adr = sector;
    uint16_t i = 0;
    for(i=0; i<mt25q_fdo.sector_size_bit; i++)
    {
        sector_adr <<= 1;
    }

	/* Check whether any previous Write, Program or Erase cycle is on-going */
    if (mt25q_is_busy())
    {
        return -1;
    }

    /* Disable Write protection */
    if (mt25q_write_enable() != 0)
    {
        return -1;
    }

    uint8_t cmd = mt25q_fdo.sector_erase_cmd;
    uint8_t adr_arr[4] = {0};

    if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
    {
        adr_arr[0] = (uint8_t)((sector_adr >> 16) & 0xFF);
        adr_arr[1] = (uint8_t)((sector_adr >> 8) & 0xFF);
        adr_arr[2] = (uint8_t)((sector_adr >> 0) & 0xFF);
    }
    else
    {
        adr_arr[0] = (uint8_t)((sector_adr >> 24) & 0xFF);
        adr_arr[1] = (uint8_t)((sector_adr >> 16) & 0xFF);
        adr_arr[2] = (uint8_t)((sector_adr >> 8) & 0xFF);
        adr_arr[3] = (uint8_t)((sector_adr >> 0) & 0xFF);
    }

    if (mt25q_spi_select() != 0)
    {
        return -1;
    }

    /* Write the erase command */
    if (mt25q_spi_write_only(&cmd, 1) != 0)
    {
        return -1;
    }

    /* Write the address */
    if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) != 0)
    {
        return -1;
    }

    if (mt25q_spi_unselect() != 0)
    {
        return -1;
    }

    /* Wait till complete */
    for(i=0; i<MT25Q_SECTOR_ERASE_TIMEOUT_MS; i++)
    {
        if (!mt25q_is_busy())
        {
            break;
        }

        mt25q_delay_ms(1);
    }

    uint8_t flag = 0;
    if (mt25q_read_flag_status_register(&flag) != 0)
    {
        return -1;
    }

    if (mt25q_clear_flag_status_register() != 0)
    {
        return -1;
    }

    if (i >= MT25Q_SECTOR_ERASE_TIMEOUT_MS)
    {
        return -1;  /* Timeout reached */
    }

    return 0;
}

int mt25q_sub_sector_erase(mt25q_sector_t sub)
{
    /* Validate the sector number input */
    if(sub >= mt25q_fdo.sub_sector_count)
    {
        return -1;  /* The sector does not exist! */
    }

    uint32_t sub_sector_adr = sub;
    uint16_t i = 0;
    for(i=0; i<mt25q_fdo.sub_sector_size_bit; i++)
    {
        sub_sector_adr <<= 1;
    }

	/* Check whether any previous Write, Program or Erase cycle is on-going */
    if (mt25q_is_busy())
    {
        return -1;
    }

    /* Disable Write protection */
    if (mt25q_write_enable() != 0)
    {
        return -1;
    }

    uint8_t cmd = mt25q_fdo.sub_sector_erase_cmd;
    uint8_t adr_arr[4] = {0};

    if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
    {
        adr_arr[0] = (uint8_t)((sub_sector_adr >> 16) & 0xFF);
        adr_arr[1] = (uint8_t)((sub_sector_adr >> 8) & 0xFF);
        adr_arr[2] = (uint8_t)((sub_sector_adr >> 0) & 0xFF);
    }
    else
    {
        adr_arr[0] = (uint8_t)((sub_sector_adr >> 24) & 0xFF);
        adr_arr[1] = (uint8_t)((sub_sector_adr >> 16) & 0xFF);
        adr_arr[2] = (uint8_t)((sub_sector_adr >> 8) & 0xFF);
        adr_arr[3] = (uint8_t)((sub_sector_adr >> 0) & 0xFF);
    }

    if (mt25q_spi_select() != 0)
    {
        return -1;
    }

    /* Write the erase command */
    if (mt25q_spi_write_only(&cmd, 1) != 0)
    {
        return -1;
    }

    /* Write the address */
    if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) != 0)
    {
        return -1;
    }

    if (mt25q_spi_unselect() != 0)
    {
        return -1;
    }

    /* Wait till complete */
    for(i=0; i<MT25Q_SECTOR_ERASE_TIMEOUT_MS; i++)
    {
        if (!mt25q_is_busy())
        {
            break;
        }

        mt25q_delay_ms(1);
    }

    uint8_t flag = 0;
    if (mt25q_read_flag_status_register(&flag) != 0)
    {
        return -1;
    }

    if (mt25q_clear_flag_status_register() != 0)
    {
        return -1;
    }

    if (i >= MT25Q_SECTOR_ERASE_TIMEOUT_MS)
    {
        return -1;  /* Timeout reached */
    }

    return 0;
}

int mt25q_write(uint32_t adr, uint8_t *data, uint16_t len)
{
    if (mt25q_write_enable() != 0)
    {
        return -1;
    }

    /* Computing the starting alignment, i.e. the distance from the page boundary */
    uint16_t data_offset = (mt25q_fdo.page_size - (adr & mt25q_fdo.address_mask) ) & mt25q_fdo.address_mask;

    if (data_offset > len)
    {
        data_offset = len;
    }

    uint8_t prog_instr = 0xFF;
    if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
    {
        prog_instr = MT25Q_PAGE_PROGRAM;
    }
    else
    {
        prog_instr = MT25Q_4_BYTE_PAGE_PROGRAM;
    }

    if (data_offset > 0)
    {
        if (mt25q_gen_program(adr, data, data_offset, prog_instr) != 0)
        {
            return -1;
        }
    }

    for(; (data_offset + mt25q_fdo.page_size) < len; data_offset+=mt25q_fdo.page_size)
    {
        if (mt25q_gen_program(adr + data_offset, data + data_offset, mt25q_fdo.page_size, prog_instr) != 0)
        {
            return -1;
        }
    }

    if (len > data_offset)
    {
        return mt25q_gen_program(adr + data_offset, data + data_offset, (len - data_offset), prog_instr);
    }

    return 0;
}

int mt25q_read(uint32_t adr, uint8_t *data, uint16_t len)
{
	/* Validate address input */
    if (adr > mt25q_get_max_address())
    {
        return -1;
    }

    uint8_t cmd = MT25Q_READ;
    uint8_t adr_arr[4] = {0};

    if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
    {
        adr_arr[0] = (uint8_t)((adr >> 16) & 0xFF);
        adr_arr[1] = (uint8_t)((adr >> 8) & 0xFF);
        adr_arr[2] = (uint8_t)((adr >> 0) & 0xFF);
    }
    else
    {
        adr_arr[0] = (uint8_t)((adr >> 24) & 0xFF);
        adr_arr[1] = (uint8_t)((adr >> 16) & 0xFF);
        adr_arr[2] = (uint8_t)((adr >> 8) & 0xFF);
        adr_arr[3] = (uint8_t)((adr >> 0) & 0xFF);
    }

    if (mt25q_spi_select() != 0)
    {
        return -1;
    }

    /* Write the READ command */
    if (mt25q_spi_write_only(&cmd, 1) != 0)
    {
        return -1;
    }

    /* Write the address */
    if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) != 0)
    {
        return -1;
    }

    /* Read the data */
    if (mt25q_spi_read_only(data, len) != 0)
    {
        return -1;
    }

    if (mt25q_spi_unselect() != 0)
    {
        return -1;
    }

    return 0;
}

uint32_t mt25q_get_max_address(void)
{
    return mt25q_fdo.size;
}

int mt25q_enter_4_byte_address_mode(void)
{
    uint8_t cmd = MT25Q_ENTER_4_BYTE_ADDRESS_MODE;
    uint8_t ans = MT25Q_DUMMY_BYTE;

    if (mt25q_spi_transfer(&cmd, &ans, 1) != 0)
    {
        return -1;
    }

    /* Verify current addr mode */
    uint8_t flag = 0;
    if (mt25q_read_flag_status_register(&flag) != 0)
    {
        return -1;
    }

    if (flag & 1)
    {
        mt25q_fdo.num_adr_byte = MT25Q_ADDRESS_MODE_4_BYTE;
    }
    else
    {
        mt25q_fdo.num_adr_byte = MT25Q_ADDRESS_MODE_3_BYTE;
    }

    return 0;
}

int mt25q_read_flag_status_register(uint8_t *flag)
{
    uint8_t cmd[2] = {MT25Q_READ_FLAG_STATUS_REGISTER, MT25Q_DUMMY_BYTE};
    uint8_t ans[2] = {MT25Q_DUMMY_BYTE};

    if (mt25q_spi_transfer(cmd, ans, 2) != 0)
    {
        return -1;
    }

    *flag = ans[1];

    return 0;
}

int mt25q_clear_flag_status_register(void)
{
    uint8_t cmd = MT25Q_CLEAR_FLAG_STATUS_REGISTER;

    if (mt25q_spi_write(&cmd, 1) != 0)
    {
        return -1;
    }

    return 0;
}

flash_description_t mt25q_get_flash_description(void)
{
    return mt25q_fdo;
}

int mt25q_gen_program(uint32_t adr, uint8_t *data, uint32_t len, uint8_t instr)
{
	/* Validate address input */
    if (adr > mt25q_get_max_address())
    {
        return -1;
    }

	/* Check whether any previous Write, Program or Erase cycle is on-going */
    if (mt25q_is_busy())
    {
        return -1;
    }

    uint8_t cmd = instr;
    uint8_t adr_arr[4] = {0};

    if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
    {
        adr_arr[0] = (uint8_t)((adr >> 16) & 0xFF);
        adr_arr[1] = (uint8_t)((adr >> 8) & 0xFF);
        adr_arr[2] = (uint8_t)((adr >> 0) & 0xFF);
    }
    else
    {
        adr_arr[0] = (uint8_t)((adr >> 24) & 0xFF);
        adr_arr[1] = (uint8_t)((adr >> 16) & 0xFF);
        adr_arr[2] = (uint8_t)((adr >> 8) & 0xFF);
        adr_arr[3] = (uint8_t)((adr >> 0) & 0xFF);
    }

    if (mt25q_spi_select() != 0)
    {
        return -1;
    }

    /* Write the PROGRAM command */
    if (mt25q_spi_write_only(&cmd, 1) != 0)
    {
        return -1;
    }

    /* Write the address */
    if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) != 0)
    {
        return -1;
    }

    /* Write the data */
    if (mt25q_spi_write_only(data, len) != 0)
    {
        return -1;
    }

    if (mt25q_spi_unselect() != 0)
    {
        return -1;
    }

    /* Wait till complete */
    uint16_t i = 0;
    for(i=0; i<MT25Q_PROGRAM_TIMEOUT_MS; i++)
    {
        if (!mt25q_is_busy())
        {
            break;
        }

        mt25q_delay_ms(1);
    }

    uint8_t flag = 0;
    if (mt25q_read_flag_status_register(&flag) != 0)
    {
        return -1;
    }

    if (mt25q_clear_flag_status_register() != 0)
    {
        return -1;
    }

    if ((flag & MT25Q_REG_FLAG_STATUS_PROGRAM) || (flag & MT25Q_REG_FLAG_STATUS_PROTECTION))
    {
        return -1;  /* Program failure or protection error */
    }

    /* Timeout reached */
    if (i >= MT25Q_PROGRAM_TIMEOUT_MS)
    {
        return -1;
    }

    return 0;
}

/** \} End of mt25q group */
