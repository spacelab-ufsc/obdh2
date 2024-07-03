#ifndef MEM_DUMP_H_
#define MEM_DUMP_H_

#include <devices/media/media.h>
#include <stdint.h>

#include <structs/satellite.h>
#include <structs/obdh_data.h>

void vTaskMemDump(void* param);

#endif
