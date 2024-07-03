#include <stdint.h>
#include <string.h>

#include <system/sys_log/sys_log.h>
#include <devices/media/media.h>
#include <config/config.h>
#include <FreeRTOS.h>
#include <task.h>

#include "mem_dump.h"

#define NOR_ADDR(page, page_size) (((uint32_t) page * (uint32_t) page_size));

static void obdh_dump_param(uint8_t* raw_data, const char* const dev)
{
    /* Checking values of obdh param */
     obdh_telemetry_t* p = (obdh_telemetry_t*) raw_data;

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param time stamp: ");
     sys_log_print_uint(p->timestamp);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param temperature: ");
     sys_log_print_uint((uint32_t)p->data.temperature);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param current: ");
     sys_log_print_uint((uint32_t)p->data.current);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param voltage: ");
     sys_log_print_uint((uint32_t)p->data.voltage);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param last reset cause: ");
     sys_log_print_hex((uint32_t)p->data.last_reset_cause);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param reset counter: ");
     sys_log_print_uint((uint32_t)p->data.reset_counter);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param last valid tc: ");
     sys_log_print_uint((uint32_t)p->data.last_valid_tc);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param hw version: ");
     sys_log_print_uint((uint32_t)p->data.hw_version);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param fw version: ");
     sys_log_print_uint((uint32_t)p->data.fw_version);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param mode: ");
     sys_log_print_uint((uint32_t)p->data.mode);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param ts last change: ");
     sys_log_print_uint((uint32_t)p->data.ts_last_mode_change);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param mode duration: ");
     sys_log_print_uint((uint32_t)p->data.mode_duration);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param initial hib exec: ");
     sys_log_print_uint((uint32_t)p->data.initial_hib_executed);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param initial hib time count: ");
     sys_log_print_uint((uint32_t)p->data.initial_hib_time_count);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param ant deployed exec: ");
     sys_log_print_uint((uint32_t)p->data.ant_deployment_executed);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param ant deployed counter: ");
     sys_log_print_uint((uint32_t)p->data.ant_deployment_counter);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param position struct dump: ");
     sys_log_dump_hex(((uint8_t*)&(p->data.position)), sizeof(position_data_t));
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page obdh: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_obdh_data);
     sys_log_new_line();


     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page eps: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_eps_data);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page ttc0: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_ttc_0_data);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page ttc1: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_ttc_1_data);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page ant: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_ant_data);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page edc: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_edc_data);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page px: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_px_data);
     sys_log_new_line();

     vTaskDelay(pdMS_TO_TICKS(10U));

     sys_log_print_event_from_module(SYS_LOG_INFO, dev, "Param media page sbcd: ");
     sys_log_print_uint((uint32_t)p->data.media.last_page_sbcd_pkts);
     sys_log_new_line();
}


static void fram_dump(void)
{
    uint32_t addr;
    uint8_t init_word[8];
    uint8_t sys_time[6];
    uint8_t obdh_param[256];

    addr = CONFIG_MEM_ADR_INIT_WORD;

    /* Read init word and log it*/
    if (media_read(MEDIA_FRAM, addr, init_word, 8U) == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, "FRAM DUMP", "Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_dump_hex(init_word, 8U);
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, "FRAM DUMP", "Error reading Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_new_line();
    }

    vTaskDelay(pdMS_TO_TICKS(100U));

    addr = CONFIG_MEM_ADR_SYS_TIME;

    /* Read sys time and log it*/
    if (media_read(MEDIA_FRAM, addr, sys_time, 6U) == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, "FRAM DUMP", "Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_dump_hex(sys_time, 6U);
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, "FRAM DUMP", "Error reading Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_new_line();
    }

    vTaskDelay(pdMS_TO_TICKS(100U));

    addr = CONFIG_MEM_ADR_SYS_PARAM;

    /* Read OBDH params and log it*/
    if (media_read(MEDIA_FRAM, addr, obdh_param, 256U) == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, "FRAM DUMP", "Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_dump_hex(obdh_param, 256U);
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, "FRAM DUMP", "Error reading Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_new_line();
    }

    vTaskDelay(pdMS_TO_TICKS(100U));
    
    obdh_dump_param(obdh_param, "FRAM OBDH PARAMS");

}

static inline uint32_t check_final_page(uint32_t page)
{
    if (page < CONFIG_MEM_OBDH_DATA_END_PAGE)
        return CONFIG_MEM_OBDH_DATA_END_PAGE;

    return CONFIG_MEM_SBCD_PKTS_END_PAGE;
}

static bool nor_page_read(uint32_t addr, uint32_t final_page, uint8_t* data)
{
    uint32_t final_addr = NOR_ADDR(final_page, 256UL);

    if (addr > final_addr)
        return false;

    if (media_read(MEDIA_NOR, addr, data, 256U) == 0)
    {
        sys_log_print_event_from_module(SYS_LOG_INFO, "NOR DUMP", "Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_dump_hex(data, 256U);
        sys_log_new_line();
    }
    else
    {
        sys_log_print_event_from_module(SYS_LOG_ERROR, "NOR DUMP", "Error reading Addr(");
        sys_log_print_hex(addr);
        sys_log_print_msg("): ");
        sys_log_new_line();

        return false;
    }

    return true;
}

static void nor_dump(void)
{
    uint8_t buf[256] = {0};
    uint32_t addr = 0UL;
    bool reading = true;
    uint32_t page = 0UL;
    uint32_t final_page = 0UL;

    while (reading)
    {
        addr = NOR_ADDR(page, 256UL);
        final_page = check_final_page(page);

        reading = nor_page_read(addr, final_page, buf);

        if (reading && (final_page == CONFIG_MEM_OBDH_DATA_END_PAGE))
        {
            sys_log_print_event_from_module(SYS_LOG_INFO, "OBDH PARAMS", "PAGE: ");
            sys_log_print_uint(page);
            sys_log_new_line();

            obdh_dump_param(buf, "NOR OBDH PARAMS");
        }

        page++;
    }
}

void vTaskMemDump(void* param)
{
    vTaskDelay(pdMS_TO_TICKS(1000U));

    while(1)
    {
        fram_dump();

        nor_dump();

        vTaskSuspend(NULL);
    }
}
