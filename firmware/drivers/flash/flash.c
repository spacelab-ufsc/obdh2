/*
 * flash.c
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
 * \brief Flash driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Carlos Augusto Porto Freitas <carlos.portof@hotmail.com>
 *
 * \version 0.10.18
 * 
 * \date 2020/03/17
 * 
 * \addtogroup flash
 * \{
 */

#include <msp430.h>

#include "flash.h"

static long *current_flash_ptr;

int flash_init(void)
{
    return 0;
}

void flash_write(uint8_t *data, uint16_t len)
{
    static char *flash_ptr;

    uint16_t i;

    if ((FCTL3 & LOCKA) > 0)
    {
        FCTL3 = FWKEY | LOCKA;              /* Clear Lock bit and LockA */
    }
    else
    {
        FCTL3 = FWKEY;                      /* Clear Lock bit */
    }

    FCTL1 = FWKEY | WRT;                    /* Set WRT bit for write operation */
    for(i=0; i<len; i++)
    {
        flash_ptr[i] = data[i];             /* Write value to flash */

        while((FCTL3 & BUSY) == 1)          /* Check if Flash being used */
        {
            ;
        }
    }

    FCTL1 = FWKEY;                          /* Clear WRT bit */
    FCTL3 = FWKEY | LOCK | LOCKA;           /* Set LOCK bit */
}

void flash_write_single(uint8_t data, uint8_t *addr)
{
    if ((FCTL3 & LOCKA) > 0)
    {
        FCTL3 = FWKEY | LOCKA;              /* Clear Lock bit and LockA */
    }
    else
    {
        FCTL3 = FWKEY;                      /* Clear Lock bit */
    }

    FCTL1 = FWKEY | WRT;                    /* Set WRT bit for write operation */
    *addr = data;                           /* Write value to flash */

    while((FCTL3 & BUSY) == 1)              /* Check if Flash being used */
    {
        ;
    }

    FCTL1 = FWKEY;                          /* Clear WRT bit */
    FCTL3 = FWKEY | LOCK | LOCKA;           /* Set LOCK bit */
}

uint8_t flash_read_single(uint8_t *addr)
{
    return *addr;
}

void flash_write_long(uint32_t data, uint32_t *addr)
{
    if ((FCTL3 & LOCKA) > 0)
    {
        FCTL3 = FWKEY | LOCKA;              /* Clear Lock bit and LockA */
    }
    else
    {
        FCTL3 = FWKEY;                      /* Clear Lock bit */
    }

    FCTL1 = FWKEY | BLKWRT;                 /* Set WRT bit for write operation */
    *addr = data;                           /* Write value to flash */

    while((FCTL3 & BUSY) == 1)              /* Check if Flash being used */
    {
        ;
    }

    FCTL1 = FWKEY;                          /* Clear WRT bit */
    FCTL3 = FWKEY | LOCK | LOCKA;           /* Set LOCK bit */
}

uint32_t flash_read_long(uint32_t *addr)
{
    return *addr;
}

void flash_erase(uint32_t *region)
{
    uint32_t *erase_ptr = region;
    uintptr_t seg = region;

    if ((FCTL3 & LOCKA) > 0)
    {
        FCTL3 = FWKEY | LOCKA;              /* Clear Lock bit and LockA */
    }
    else
    {
        FCTL3 = FWKEY;                      /* Clear Lock bit */
    }

    switch(seg)
    {
        case FLASH_BANK_0_ADR:  FCTL1 = FWKEY | MERAS;          break;
        case FLASH_BANK_1_ADR:  FCTL1 = FWKEY | MERAS;          break;
        case FLASH_BANK_2_ADR:  FCTL1 = FWKEY | MERAS;          break;
        case FLASH_BANK_3_ADR:  FCTL1 = FWKEY | MERAS;          break;
        case FLASH_SEG_A_ADR:   FCTL1 = FWKEY | ERASE;          break;
        case FLASH_SEG_B_ADR:   FCTL1 = FWKEY | ERASE;          break;
        case FLASH_SEG_C_ADR:   FCTL1 = FWKEY | ERASE;          break;
        case FLASH_SEG_D_ADR:   FCTL1 = FWKEY | ERASE;          break;
        case FLASH_MASS_ERASE:  FCTL1 = FWKEY | MERAS | ERASE;  break;
        default:                                                break;
    }

    *erase_ptr = 0;

    while((FCTL3 & BUSY) == 1)
    {
        ;
    }

    FCTL1 = FWKEY;                          /* Clear WRT bit */
    FCTL3 = FWKEY | LOCK | LOCKA;           /* Set LOCK bit */
}

/** \} End of flash group */
