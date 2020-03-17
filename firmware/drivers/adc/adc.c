/*
 * adc.c
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
 * \brief ADC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.3.1
 * 
 * \date 03/03/2020
 * 
 * \addtogroup adc
 * \{
 */

#include <hal/adc10_a.h>
#include <hal/adc12_a.h>

#include "adc.h"

int adc_init(adc_port_t port, adc_config_t config)
{
    /* Reset REFMSTR to hand over control to ADC12_A ref control registers */
    REFCTL0 &= ~REFMSTR;

    /* Vref+ = 3.0 V, Vref- = 0 V */
    ADC12CTL0 = ADC12MSC | ADC12SHT0_15 | ADC12REFON | ADC12ON;

    /* Enable sample timer */
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_1;

    P6SEL |= (1 << 3) | (1 << 4);

    ADC12MCTL3 = ADC12SREF_2 | ADC12INCH_3;                 /* OBDH current-sense */
    ADC12MCTL4 = ADC12SREF_2 | ADC12INCH_4;                 /* OBDH Voltage-sense */
    ADC12MCTL5 = ADC12EOS | ADC12SREF_2 | ADC12INCH_10;     /* Temperature sensor */

    /* Allow ~100us (at default UCS settings) for REF to settle */
    adc_delay_ms(1);

    ADC12CTL0 |= ADC12ENC;

    return 0;
}

int adc_read(adc_port_t port, uint16_t *val)
{
    while(ADC12CTL1 & ADC12BUSY);

    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

    switch(port)
    {
        case ADC_PORT_0:
            while(!(ADC12IFG & (1 << 0)));

            *val = ADC12MEM0;

            break;
        case ADC_PORT_1:
            while(!(ADC12IFG & (1 << 1)));

            *val = ADC12MEM1;

            break;
        case ADC_PORT_2:
            while(!(ADC12IFG & (1 << 2)));

            *val = ADC12MEM2;

            break;
        case ADC_PORT_3:
            while(!(ADC12IFG & (1 << 3)));

            *val = ADC12MEM3;

            break;
        case ADC_PORT_4:
            while(!(ADC12IFG & (1 << 4)));

            *val = ADC12MEM4;

            break;
        case ADC_PORT_5:
            while(!(ADC12IFG & (1 << 5)));

            *val = ADC12MEM5;

            break;
        case ADC_PORT_6:
            while(!(ADC12IFG & (1 << 6)));

            *val = ADC12MEM6;

            break;
        case ADC_PORT_7:
            while(!(ADC12IFG & (1 << 7)));

            *val = ADC12MEM7;

            break;
        case ADC_PORT_8:
            while(!(ADC12IFG & (1 << 8)));

            *val = ADC12MEM8;

            break;
        case ADC_PORT_9:
            while(!(ADC12IFG & (1 << 9)));

            *val = ADC12MEM9;

            break;
        case ADC_PORT_10:
            while(!(ADC12IFG & (1 << 10)));

            *val = ADC12MEM10;

            break;
        case ADC_PORT_11:
            while(!(ADC12IFG & (1 << 11)));

            *val = ADC12MEM11;

            break;
        case ADC_PORT_12:
            while(!(ADC12IFG & (1 << 12)));

            *val = ADC12MEM12;

            break;
        case ADC_PORT_13:
            while(!(ADC12IFG & (1 << 13)));

            *val = ADC12MEM13;

            break;
        case ADC_PORT_14:
            while(!(ADC12IFG & (1 << 14)));

            *val = ADC12MEM14;

            break;
        case ADC_PORT_15:
            while(!(ADC12IFG & (1 << 15)));

            *val = ADC12MEM15;

            break;
        default:
            *val = UINT16_MAX;

            return -1;
    }

    return 0;
}

/** \} End of adc group */
