/*
 * adc.c
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
 * \brief ADC driver implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.7.28
 * 
 * \date 2020/03/03
 * 
 * \addtogroup adc
 * \{
 */

#include <stdbool.h>

#include <hal/gpio.h>
#include <hal/adc10_a.h>
#include <hal/adc12_a.h>
#include <hal/ref.h>
#include <hal/tlv.h>

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "adc.h"

bool adc_is_ready = false;

float adc_mref = 0;
float adc_nref = 0;

uint8_t adc_cal_bytes;
struct s_TLV_ADC_Cal_Data *adc_cal_data;

int adc_init(void)
{
    if (!adc_is_ready)
    {
        /* Daughterboard, current and voltage sensor pins */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4);

        ADC12_A_init(ADC12_A_BASE, ADC12_A_SAMPLEHOLDSOURCE_SC, ADC12_A_CLOCKSOURCE_ADC12OSC, ADC12_A_CLOCKDIVIDER_1);

        ADC12_A_enable(ADC12_A_BASE);

        ADC12_A_setupSamplingTimer(ADC12_A_BASE, ADC12_A_CYCLEHOLD_768_CYCLES, ADC12_A_CYCLEHOLD_4_CYCLES, ADC12_A_MULTIPLESAMPLESDISABLE);

        /* Temperature sensor */
        ADC12_A_configureMemoryParam param = {0};

        param.memoryBufferControlIndex          = ADC12_A_MEMORY_0;
        param.inputSourceSelect                 = ADC12_A_INPUT_TEMPSENSOR;
        param.positiveRefVoltageSourceSelect    = ADC12_A_VREFPOS_INT;
        param.negativeRefVoltageSourceSelect    = ADC12_A_VREFNEG_AVSS;
        param.endOfSequence                     = ADC12_A_NOTENDOFSEQUENCE;

        ADC12_A_configureMemory(ADC12_A_BASE, &param);

        /* Current sensor */
        param.memoryBufferControlIndex          = ADC12_A_MEMORY_8;
        param.inputSourceSelect                 = ADC12_A_INPUT_A3;
        param.positiveRefVoltageSourceSelect    = ADC12_A_VREFPOS_EXT;
        param.negativeRefVoltageSourceSelect    = ADC12_A_VREFNEG_AVSS;
        param.endOfSequence                     = ADC12_A_NOTENDOFSEQUENCE;

        ADC12_A_configureMemory(ADC12_A_BASE, &param);

        /* Voltage sensor */
        param.memoryBufferControlIndex          = ADC12_A_MEMORY_9;
        param.inputSourceSelect                 = ADC12_A_INPUT_A4;
        param.positiveRefVoltageSourceSelect    = ADC12_A_VREFPOS_EXT;
        param.negativeRefVoltageSourceSelect    = ADC12_A_VREFNEG_AVSS;
        param.endOfSequence                     = ADC12_A_NOTENDOFSEQUENCE;

        ADC12_A_configureMemory(ADC12_A_BASE, &param);

        /* Daughterboard ADC 0 */
        param.memoryBufferControlIndex          = ADC12_A_MEMORY_10;
        param.inputSourceSelect                 = ADC12_A_INPUT_A0;
        param.positiveRefVoltageSourceSelect    = ADC12_A_VREFPOS_EXT;
        param.negativeRefVoltageSourceSelect    = ADC12_A_VREFNEG_AVSS;
        param.endOfSequence                     = ADC12_A_NOTENDOFSEQUENCE;

        ADC12_A_configureMemory(ADC12_A_BASE, &param);

        /* Daughterboard ADC 1 */
        param.memoryBufferControlIndex          = ADC12_A_MEMORY_11;
        param.inputSourceSelect                 = ADC12_A_INPUT_A1;
        param.positiveRefVoltageSourceSelect    = ADC12_A_VREFPOS_EXT;
        param.negativeRefVoltageSourceSelect    = ADC12_A_VREFNEG_AVSS;
        param.endOfSequence                     = ADC12_A_NOTENDOFSEQUENCE;

        ADC12_A_configureMemory(ADC12_A_BASE, &param);

        /* Daughterboard ADC 2 */
        param.memoryBufferControlIndex          = ADC12_A_MEMORY_12;
        param.inputSourceSelect                 = ADC12_A_INPUT_A2;
        param.positiveRefVoltageSourceSelect    = ADC12_A_VREFPOS_EXT;
        param.negativeRefVoltageSourceSelect    = ADC12_A_VREFNEG_AVSS;
        param.endOfSequence                     = ADC12_A_NOTENDOFSEQUENCE;

        ADC12_A_configureMemory(ADC12_A_BASE, &param);

        ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12_A_IFG0 | ADC12_A_IFG8 | ADC12_A_IFG9 | ADC12_A_IFG10 | ADC12_A_IFG11 | ADC12_A_IFG12);

        uint8_t i = 0;
        for(i = 0; i < ADC_TIMEOUT_MS; i++)
        {
            if (REF_ACTIVE != Ref_isRefGenBusy(REF_BASE))
            {
                break;
            }

            adc_delay_ms(1);
        }

        Ref_setReferenceVoltage(REF_BASE, REF_VREF1_5V);

        Ref_enableReferenceVoltage(REF_BASE);

        Ref_enableTempSensor(REF_BASE);

        adc_delay_ms(10);

        /* Temperature sensor calibration data */
        TLV_getInfo(TLV_TAG_ADCCAL, 0, &adc_cal_bytes, &adc_cal_data);

        adc_mref = ((float)(adc_cal_data->adc_ref15_85_temp - adc_cal_data->adc_ref15_30_temp)) / (85 - 30);
        adc_nref = adc_cal_data->adc_ref15_85_temp - (adc_mref * 85.0);

        adc_is_ready = true;
    }
    else
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_WARNING, ADC_MODULE_NAME, "ADC driver already initialized!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
    }

    return 0;
}

int adc_read(adc_port_t port, uint16_t *val)
{
    uint8_t i = 0;
    for(i=0; i<ADC_TIMEOUT_MS; i++)
    {
        if (!ADC12_A_isBusy(ADC12_A_BASE))
        {
            break;
        }

        adc_delay_ms(1);
    }

    int err = -1;

    *val = UINT16_MAX;

    if (i != ADC_TIMEOUT_MS)
    {
        switch(port)
        {
            case ADC_PORT_0:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_0, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG0) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_0);

                    err = 0;
                }

                break;
            case ADC_PORT_1:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_1, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG1) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_1);

                    err = 0;
                }

                break;
            case ADC_PORT_2:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_2, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG2) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_2);

                    err = 0;
                }

                break;
            case ADC_PORT_3:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_3, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG3) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_3);

                    err = 0;
                }

                break;
            case ADC_PORT_4:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_4, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG4) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_4);

                    err = 0;
                }

                break;
            case ADC_PORT_5:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_5, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG5) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_5);

                    err = 0;
                }

                break;
            case ADC_PORT_6:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_6, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG6) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_6);

                    err = 0;
                }

                break;
            case ADC_PORT_7:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_7, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG7) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_7);

                    err = 0;
                }

                break;
            case ADC_PORT_8:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_8, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG8) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_8);

                    err = 0;
                }

                break;
            case ADC_PORT_9:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_9, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG9) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_9);

                    err = 0;
                }

                break;
            case ADC_PORT_10:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_10, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG10) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_10);

                    err = 0;
                }

                break;
            case ADC_PORT_11:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_11, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG11) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_11);

                    err = 0;
                }

                break;
            case ADC_PORT_12:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_12, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG12) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_12);

                    err = 0;
                }

                break;
            case ADC_PORT_13:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_13, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG13) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_13);

                    err = 0;
                }

                break;
            case ADC_PORT_14:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_14, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG14) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_14);

                    err = 0;
                }

                break;
            case ADC_PORT_15:
                ADC12_A_startConversion(ADC12_A_BASE, ADC12_A_MEMORY_15, ADC12_A_SINGLECHANNEL);

                for(i=0; i<ADC_TIMEOUT_MS; i++)
                {
                    if (ADC12_A_getInterruptStatus(ADC12_A_BASE, ADC12_A_IFG15) > 0)
                    {
                        break;
                    }

                    adc_delay_ms(1);
                }

                if (i != ADC_TIMEOUT_MS)
                {
                    *val = ADC12_A_getResults(ADC12_A_BASE, ADC12_A_MEMORY_15);

                    err = 0;
                }

                break;
            default:
            #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
                sys_log_print_event_from_module(SYS_LOG_ERROR, ADC_MODULE_NAME, "Error reading the ADC port ");
                sys_log_print_uint(port);
                sys_log_print_msg("! Invalid port!");
                sys_log_new_line();
            #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
                break;
        }

        if (err == 0)
        {
            ADC12_A_clearInterrupt(ADC12_A_BASE, ADC12_A_IFG0 | ADC12_A_IFG8 | ADC12_A_IFG9);
        }
    }

    return err;
}

float adc_temp_get_mref(void)
{
    return adc_mref;
}

float adc_temp_get_nref(void)
{
    return adc_nref;
}

/** \} End of adc group */
