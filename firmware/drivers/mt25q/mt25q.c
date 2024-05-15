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
 * along with OBDH 2.0. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief MT25Q driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.8.9
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

static flash_description_t mt25q_fdo = {0};

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
static int mt25q_gen_program(uint32_t adr, uint8_t *data, uint32_t len, uint8_t instr);

int mt25q_init(void)
{
    int err = -1;

    if (mt25q_spi_init() == 0)
    {
        if (mt25q_gpio_init() == 0)
        {
            mt25q_delay_ms(10);

            if (mt25q_read_flash_description(&mt25q_fdo) == 0)
            {
                err = 0;
            }
        }
    }

    return err;
}

int mt25q_read_device_id(mt25q_dev_id_t *dev_id)
{
    int err = -1;

    uint8_t cmd[4] = {MT25Q_READ_ID_REG, MT25Q_DUMMY_BYTE, MT25Q_DUMMY_BYTE, MT25Q_DUMMY_BYTE};
    uint8_t ans[4] = {0};

    if (mt25q_spi_transfer(cmd, ans, 4) == 0)
    {
        dev_id->manufacturer_id  = ans[1];
        dev_id->memory_type      = ans[2];
        dev_id->memory_capacity  = ans[3];

        err = 0;
    }

    return err;
}

int mt25q_read_flash_description(flash_description_t *fdo)
{
    int err = -1;

    /* Read device ID */
    mt25q_dev_id_t dev_id = {0};

    if (mt25q_read_device_id(&dev_id) == 0)
    {
        /* Check the manufacturer */
        if (dev_id.manufacturer_id == MT25Q_MANUFACTURER_ID)
        {
            uint32_t device = ((uint32_t)dev_id.manufacturer_id << 16) |
                              ((uint32_t)dev_id.memory_type << 8) |
                              ((uint32_t)dev_id.memory_capacity);

            fdo->id = device;

            device &= 0xFFU;
            if (device >= 0x20U)
            {
                device -= 6U;   /* WTF?? */
            }

            fdo->size = 1UL << device;
            fdo->num_adr_byte = 3;

            /* Read the discovery parameter signature */
            uint8_t cmd[350] = {0};
            uint8_t ans[350] = {0};

            cmd[0] = MT25Q_READ_SERIAL_FLASH_DISCOVERY_PARAMETER_REG;

            if (mt25q_spi_transfer(cmd, ans, 350) == 0)
            {
                /* Remove the SPI command, address and dummy clocks */
                if (memmove(ans, &ans[5], 350) == ans)   /* 5 = SPI command + address + dummy clocks */
                {
                    /* Check if the read data is valid */
                    uint8_t sfdp[6] = {'S', 'F', 'D', 'P', 0, 164};
                    if ((ans != NULL) && (memcmp(ans, sfdp, 4) == 0))
                    {
                        /* The parameter table pointer is at MT25Q_DISCOVERY_TABLE_1 */
                        uint32_t table_address = (uint32_t)ans[MT25Q_DISCOVERY_TABLE_1] |
                                                 ((uint32_t)ans[MT25Q_DISCOVERY_TABLE_1 + 1U] << 8) |
                                                 ((uint32_t)ans[MT25Q_DISCOVERY_TABLE_1 + 2U] << 16);

                        /* Get the official device size in bytes */
                        uint32_t size_buf = (uint32_t)ans[table_address + MT25Q_DTABLE_1_FLASH_SIZE] |
                                            ((uint32_t)ans[table_address + MT25Q_DTABLE_1_FLASH_SIZE + 1U] << 8) |
                                            ((uint32_t)ans[table_address + MT25Q_DTABLE_1_FLASH_SIZE + 2U] << 16) |
                                            ((uint32_t)ans[table_address + MT25Q_DTABLE_1_FLASH_SIZE + 3U] << 24);
                        fdo->size = (size_buf + 1U) / 8U;

                        /* Get the largest sector size and the sector count, and take one sub-sector size and sub-sector count. */
                        /* The first two sector definitions have the definitions that we use - usually 4K and 64K. */
                        uint32_t t_offset = table_address + MT25Q_DTABLE_1_SECTOR_DESCRIPTOR + 2U;

                        if (ans[t_offset] != 0U)
                        {
                            fdo->sector_size_bit        = ans[t_offset];
                            /* fdo->sector_size            = (1 << (ans[t_offset])); */
                            fdo->sector_size            = 1;
                            uint8_t i = 0;
                            for(i = 0; i < ans[t_offset]; i++)
                            {
                                fdo->sector_size        = (fdo->sector_size << 1);
                            }
                            fdo->sector_count           = fdo->size / fdo->sector_size;
                            fdo->sector_erase_cmd       = ans[t_offset + 1U];

                            fdo->sub_sector_size_bit    = ans[t_offset - 2U];
                            /* fdo->sub_sector_size        = (1 << (ans[t_offset - 2])); */
                            fdo->sub_sector_size        = 1;
                            for(i = 0; i < ans[t_offset - 2U]; i++)
                            {
                                fdo->sub_sector_size    = (fdo->sub_sector_size << 1);
                            }
                            fdo->sub_sector_count       = fdo->size / fdo->sub_sector_size;
                            fdo->sub_sector_erase_cmd   = ans[t_offset - 1U];
                        }

                        /* Hard-coded flash parameters */
                        fdo->page_size      = 0x100U;
                        fdo->page_count     = fdo->size / fdo->page_size;
                        fdo->address_mask   = 0xFFU;

                        fdo->otp_size = 0x40U;

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

                            if (mt25q_enter_4_byte_address_mode() == 0)
                            {
                                /* Verify current address mode */
                                if (mt25q_read_flag_status_register(&flag) == 0)
                                {
                                    if ((flag & 1U) > 0U)   /* Test addressing bit of flag status reg (bit 0) */
                                    {
                                        fdo->num_adr_byte = MT25Q_ADDRESS_MODE_4_BYTE;
                                    }

                                    err = 0;
                                }
                            }
                        }
                        else
                        {
                            fdo->num_adr_byte = MT25Q_ADDRESS_MODE_3_BYTE;

                            err = 0;
                        }
                    }
                }
            }
        }
    }

    return err;
}

int mt25q_read_status(mt25q_status_t *status)
{
    int err = -1;

    uint8_t cmd[2] = {MT25Q_READ_STATUS_REGISTER, MT25Q_DUMMY_BYTE};
    uint8_t ans[2] = {0};

    if (mt25q_spi_transfer(cmd, ans, 2) == 0)
    {
        status->write_enable        = (ans[1] & MT25Q_REG_STATUS_WRITE_ENABLE) >> 7;
        status->top_bottom          = (ans[1] & MT25Q_REG_STATUS_TOP_BOTTOM) >> 5;
        status->bp                  = (ans[1] & MT25Q_REG_STATUS_BP) >> 2;
        status->bp                  = (((status->bp >> 1) & 0x08U) | status->bp) & 0x0FU;
        status->write_enable_latch  = (ans[1] & MT25Q_REG_STATUS_WRITE_ENABLE_LATCH) >> 1;
        status->write_in_progress   = (ans[1] & MT25Q_REG_STATUS_WRITE_IN_PROGRESS);

        err = 0;
    }

    return err;
}

int mt25q_reset(void)
{
    int err = -1;

    uint8_t cmd = MT25Q_RESET_ENABLE;

    if (mt25q_spi_write(&cmd, 1) == 0)
    {
        cmd = MT25Q_RESET_MEMORY;

        if (mt25q_spi_write(&cmd, 1) == 0)
        {
            err = 0;
        }
    }

    return err;
}

int mt25q_enter_deep_power_down(void)
{
    uint8_t cmd = MT25Q_ENTER_DEEP_POWER_DOWN;

    return mt25q_spi_write(&cmd, 1);
}

int mt25q_release_from_deep_power_down(void)
{
    uint8_t cmd = MT25Q_RELEASE_FROM_DEEP_POWER_DOWN;

    return mt25q_spi_write(&cmd, 1);
}

int mt25q_write_enable(void)
{
    int err = -1;

    uint8_t cmd = MT25Q_WRITE_ENABLE;

    if (mt25q_spi_write(&cmd, 1) == 0)
    {
        uint8_t i = 0;
        for(i = 0; i < UINT8_MAX; i++)
        {
            mt25q_status_t status = {0};

            if (mt25q_read_status(&status) == 0)
            {
                if (status.write_enable_latch == MT25Q_WRITE_ENABLED)
                {
                    err = 0;
                    break;
                }
            }
        }
    }

    return err;
}

int mt25q_write_disable(void)
{
    int err = -1;

    uint8_t cmd = MT25Q_WRITE_DISABLE;

    if (mt25q_spi_write(&cmd, 1) == 0)
    {
        uint8_t i = 0;
        for(i = 0; i < UINT8_MAX; i++)
        {
            mt25q_status_t status = {0};

            if (mt25q_read_status(&status) == 0)
            {
                if (status.write_enable_latch == MT25Q_WRITE_DISABLED)
                {
                    err = 0;
                    break;
                }
            }
        }
    }

    return err;
}

bool mt25q_is_busy(void)
{
    bool res = true;

    mt25q_status_t status = {0};

    if (mt25q_read_status(&status) == 0)
    {
        if (status.write_in_progress == MT25Q_READY)
        {
            res = false;
        }
    }

    return res;
}

int mt25q_die_erase(mt25q_sector_t die)
{
    int err = -1;

    /* Validate the sector number input */
    if (die < mt25q_fdo.die_count)
    {
        uint32_t die_adr = die;

        uint16_t i = 0;
        for(i = 0; i < mt25q_fdo.die_size_bit; i++)
        {
            die_adr <<= 1;
        }

	    /* Check whether any previous Write, Program or Erase cycle is on-going */
        if (!mt25q_is_busy())
        {
            /* Disable Write protection */
            if (mt25q_write_enable() == 0)
            {
                uint8_t cmd = MT25Q_DIE_ERASE;
                uint8_t adr_arr[4] = {0};

                if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
                {
                    adr_arr[0] = (uint8_t)((die_adr >> 16) & 0xFFU);
                    adr_arr[1] = (uint8_t)((die_adr >> 8) & 0xFFU);
                    adr_arr[2] = (uint8_t)((die_adr >> 0) & 0xFFU);
                }
                else
                {
                    adr_arr[0] = (uint8_t)((die_adr >> 24) & 0xFFU);
                    adr_arr[1] = (uint8_t)((die_adr >> 16) & 0xFFU);
                    adr_arr[2] = (uint8_t)((die_adr >> 8) & 0xFFU);
                    adr_arr[3] = (uint8_t)((die_adr >> 0) & 0xFFU);
                }

                if (mt25q_spi_select() == 0)
                {
                    /* Write the erase command */
                    if (mt25q_spi_write_only(&cmd, 1) == 0)
                    {
                        /* Write the address */
                        if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) == 0)
                        {
                            if (mt25q_spi_unselect() == 0)
                            {
                                /* Wait till complete */
                                for(i = 0; i < MT25Q_SECTOR_ERASE_TIMEOUT_MS; i++)
                                {
                                    if (!mt25q_is_busy())
                                    {
                                        break;
                                    }

                                    mt25q_delay_ms(1);
                                }

                                uint8_t flag = 0;
                                if (mt25q_read_flag_status_register(&flag) == 0)
                                {
                                    if (mt25q_clear_flag_status_register() == 0)
                                    {
                                        if (i < MT25Q_SECTOR_ERASE_TIMEOUT_MS)
                                        {
                                            err = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return err;
}

int mt25q_sector_erase(mt25q_sector_t sector)
{
    int err = -1;

    /* Validate the sector number input */
    if (mt25q_fdo.sector_count >= sector)
    {
        uint32_t sector_adr = sector;

        uint16_t i = 0;
        for(i = 0; i < mt25q_fdo.sector_size_bit; i++)
        {
            sector_adr <<= 1;
        }

        /* Check whether any previous Write, Program or Erase cycle is on-going */
        if (!mt25q_is_busy())
        {
            /* Disable Write protection */
            if (mt25q_write_enable() == 0)
            {
                uint8_t cmd = mt25q_fdo.sector_erase_cmd;
                uint8_t adr_arr[4] = {0};

                if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
                {
                    adr_arr[0] = (uint8_t)((sector_adr >> 16) & 0xFFU);
                    adr_arr[1] = (uint8_t)((sector_adr >> 8) & 0xFFU);
                    adr_arr[2] = (uint8_t)((sector_adr >> 0) & 0xFFU);
                }
                else
                {
                    adr_arr[0] = (uint8_t)((sector_adr >> 24) & 0xFFU);
                    adr_arr[1] = (uint8_t)((sector_adr >> 16) & 0xFFU);
                    adr_arr[2] = (uint8_t)((sector_adr >> 8) & 0xFFU);
                    adr_arr[3] = (uint8_t)((sector_adr >> 0) & 0xFFU);
                }

                if (mt25q_spi_select() == 0)
                {
                    /* Write the erase command */
                    if (mt25q_spi_write_only(&cmd, 1) == 0)
                    {
                        /* Write the address */
                        if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) == 0)
                        {
                            if (mt25q_spi_unselect() == 0)
                            {
                                /* Wait till complete */
                                for(i = 0; i < MT25Q_SECTOR_ERASE_TIMEOUT_MS; i++)
                                {
                                    if (!mt25q_is_busy())
                                    {
                                        break;
                                    }

                                    mt25q_delay_ms(1);
                                }

                                uint8_t flag = 0;
                                if (mt25q_read_flag_status_register(&flag) == 0)
                                {
                                    if (mt25q_clear_flag_status_register() == 0)
                                    {
                                        if (i < MT25Q_SECTOR_ERASE_TIMEOUT_MS)
                                        {
                                            err = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return err;
}

int mt25q_sub_sector_erase(mt25q_sector_t sub)
{
    int err = -1;

    /* Validate the sector number input */
    if(sub < mt25q_fdo.sub_sector_count)
    {
        uint32_t sub_sector_adr = sub;

        uint16_t i = 0;
        for(i = 0; i < mt25q_fdo.sub_sector_size_bit; i++)
        {
            sub_sector_adr <<= 1;
        }

        /* Check whether any previous Write, Program or Erase cycle is on-going */
        if (!mt25q_is_busy())
        {
            /* Disable Write protection */
            if (mt25q_write_enable() == 0)
            {
                uint8_t cmd = mt25q_fdo.sub_sector_erase_cmd;
                uint8_t adr_arr[4] = {0};

                if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
                {
                    adr_arr[0] = (uint8_t)((sub_sector_adr >> 16) & 0xFFU);
                    adr_arr[1] = (uint8_t)((sub_sector_adr >> 8) & 0xFFU);
                    adr_arr[2] = (uint8_t)((sub_sector_adr >> 0) & 0xFFU);
                }
                else
                {
                    adr_arr[0] = (uint8_t)((sub_sector_adr >> 24) & 0xFFU);
                    adr_arr[1] = (uint8_t)((sub_sector_adr >> 16) & 0xFFU);
                    adr_arr[2] = (uint8_t)((sub_sector_adr >> 8) & 0xFFU);
                    adr_arr[3] = (uint8_t)((sub_sector_adr >> 0) & 0xFFU);
                }

                if (mt25q_spi_select() == 0)
                {
                    /* Write the erase command */
                    if (mt25q_spi_write_only(&cmd, 1) == 0)
                    {
                        /* Write the address */
                        if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) == 0)
                        {
                            if (mt25q_spi_unselect() == 0)
                            {
                                /* Wait till complete */
                                for(i = 0; i < MT25Q_SECTOR_ERASE_TIMEOUT_MS; i++)
                                {
                                    if (!mt25q_is_busy())
                                    {
                                        break;
                                    }

                                    mt25q_delay_ms(1);
                                }

                                uint8_t flag = 0;
                                if (mt25q_read_flag_status_register(&flag) == 0)
                                {
                                    if (mt25q_clear_flag_status_register() == 0)
                                    {
                                        if (i < MT25Q_SECTOR_ERASE_TIMEOUT_MS)
                                        {
                                            err = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return err;
}

int mt25q_write(uint32_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

    if (mt25q_write_enable() == 0)
    {
        /* Computing the starting alignment, i.e. the distance from the page boundary */
        uint16_t data_offset = (mt25q_fdo.page_size - (adr & mt25q_fdo.address_mask)) & mt25q_fdo.address_mask;

        if (data_offset > len)
        {
            data_offset = len;
        }

        uint8_t prog_instr = UINT8_MAX;
        if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
        {
            prog_instr = MT25Q_PAGE_PROGRAM;
        }
        else
        {
            prog_instr = MT25Q_4_BYTE_PAGE_PROGRAM;
        }

        err = 0;

        if (data_offset > 0U)
        {
            if (mt25q_gen_program(adr, data, data_offset, prog_instr) != 0)
            {
                err = -1;
            }
        }

        if (err == 0)
        {
            for(; (data_offset + mt25q_fdo.page_size) < len; data_offset += mt25q_fdo.page_size)
            {
                if (mt25q_gen_program(adr + data_offset, &data[data_offset], mt25q_fdo.page_size, prog_instr) != 0)
                {
                    err = -1;

                    break;
                }
            }

            if (err == 0)
            {
                if (len > data_offset)
                {
                    err = mt25q_gen_program(adr + data_offset, &data[data_offset], (len - data_offset), prog_instr);
                }
            }
        }
    }

    return err;
}

int mt25q_read(uint32_t adr, uint8_t *data, uint16_t len)
{
    int err = -1;

	/* Validate address input */
    if (adr <= mt25q_get_max_address())
    {
        uint8_t cmd = MT25Q_READ;
        uint8_t adr_arr[4] = {0};

        if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
        {
            adr_arr[0] = (uint8_t)((adr >> 16) & 0xFFU);
            adr_arr[1] = (uint8_t)((adr >> 8) & 0xFFU);
            adr_arr[2] = (uint8_t)((adr >> 0) & 0xFFU);
        }
        else
        {
            adr_arr[0] = (uint8_t)((adr >> 24) & 0xFFU);
            adr_arr[1] = (uint8_t)((adr >> 16) & 0xFFU);
            adr_arr[2] = (uint8_t)((adr >> 8) & 0xFFU);
            adr_arr[3] = (uint8_t)((adr >> 0) & 0xFFU);
        }

        if (mt25q_spi_select() == 0)
        {
            /* Write the READ command */
            if (mt25q_spi_write_only(&cmd, 1) == 0)
            {
                /* Write the address */
                if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) == 0)
                {
                    /* Read the data */
                    if (mt25q_spi_read_only(data, len) == 0)
                    {
                        if (mt25q_spi_unselect() == 0)
                        {
                            err = 0;
                        }
                    }
                }
            }
        }
    }

    return err;
}

uint32_t mt25q_get_max_address(void)
{
    return mt25q_fdo.size;
}

int mt25q_enter_4_byte_address_mode(void)
{
    int err = -1;

    uint8_t cmd = MT25Q_ENTER_4_BYTE_ADDRESS_MODE;
    uint8_t ans = MT25Q_DUMMY_BYTE;

    if (mt25q_spi_transfer(&cmd, &ans, 1) == 0)
    {
        /* Verify current addr mode */
        uint8_t flag = 0;
        if (mt25q_read_flag_status_register(&flag) == 0)
        {
            if ((flag & 1U) > 0U)
            {
                mt25q_fdo.num_adr_byte = MT25Q_ADDRESS_MODE_4_BYTE;
            }
            else
            {
                mt25q_fdo.num_adr_byte = MT25Q_ADDRESS_MODE_3_BYTE;
            }

            err = 0;
        }
    }

    return err;
}

int mt25q_read_flag_status_register(uint8_t *flag)
{
    int err = -1;

    uint8_t cmd[2] = {MT25Q_READ_FLAG_STATUS_REGISTER, MT25Q_DUMMY_BYTE};
    uint8_t ans[2] = {0};

    if (mt25q_spi_transfer(cmd, ans, 2) == 0)
    {
        *flag = ans[1];

        err = 0;
    }

    return err;
}

int mt25q_clear_flag_status_register(void)
{
    uint8_t cmd = MT25Q_CLEAR_FLAG_STATUS_REGISTER;

    return mt25q_spi_write(&cmd, 1);
}

flash_description_t mt25q_get_flash_description(void)
{
    return mt25q_fdo;
}

static int mt25q_gen_program(uint32_t adr, uint8_t *data, uint32_t len, uint8_t instr)
{
    int err = -1;

    /* Validate address input */
    if (adr <= mt25q_get_max_address())
    {
        /* Check whether any previous Write, Program or Erase cycle is on-going */
        if (!mt25q_is_busy())
        {
            uint8_t cmd = instr;
            uint8_t adr_arr[4] = {0};

            if (mt25q_fdo.num_adr_byte == MT25Q_ADDRESS_MODE_3_BYTE)
            {
                adr_arr[0] = (uint8_t)((adr >> 16) & 0xFFU);
                adr_arr[1] = (uint8_t)((adr >> 8) & 0xFFU);
                adr_arr[2] = (uint8_t)((adr >> 0) & 0xFFU);
            }
            else
            {
                adr_arr[0] = (uint8_t)((adr >> 24) & 0xFFU);
                adr_arr[1] = (uint8_t)((adr >> 16) & 0xFFU);
                adr_arr[2] = (uint8_t)((adr >> 8) & 0xFFU);
                adr_arr[3] = (uint8_t)((adr >> 0) & 0xFFU);
            }

            if (mt25q_spi_select() == 0)
            {
                /* Write the PROGRAM command */
                if (mt25q_spi_write_only(&cmd, 1) == 0)
                {
                    /* Write the address */
                    if (mt25q_spi_write_only(adr_arr, mt25q_fdo.num_adr_byte) == 0)
                    {
                        /* Write the data */
                        if (mt25q_spi_write_only(data, len) == 0)
                        {
                            if (mt25q_spi_unselect() == 0)
                            {
                                /* Wait till complete */
                                uint16_t i = 0;
                                for(i = 0; i < MT25Q_PROGRAM_TIMEOUT_MS; i++)
                                {
                                    if (!mt25q_is_busy())
                                    {
                                        break;
                                    }

                                    mt25q_delay_ms(1);
                                }

                                uint8_t flag = 0;
                                if (mt25q_read_flag_status_register(&flag) == 0)
                                {
                                    if (mt25q_clear_flag_status_register() == 0)
                                    {
                                        if (((flag & MT25Q_REG_FLAG_STATUS_PROGRAM) == 0U) && ((flag & MT25Q_REG_FLAG_STATUS_PROTECTION) == 0U))
                                        {
                                            if (i < MT25Q_PROGRAM_TIMEOUT_MS)
                                            {
                                                err = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return err;
}

/** \} End of mt25q group */
