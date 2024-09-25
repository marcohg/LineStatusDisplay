#ifndef _SOURCE_GLOBAL_DATA_H
#define _SOURCE_GLOBAL_DATA_H
#include "typedefs.h"
#include <stdint.h>

#define MODBUS_LOCAL_ADDRESS        0x02
#define MODBUS_STD_TBL_SIZE         16

// No gaps between tables makes easier to extend, just define the address of next register out of the LUT_EXT
#define MODBUS_MULTIPLE_RD_MAX      50
#define MODBUS_MULTIPLE_RD_EXTEND   120 // MAX is 125
#define MODBUS_SERIAL_BUFFER_SIZE   256

// Firmware Version
#define MAJOR 0
#define MINOR 5
#define BUILD 0

extern uint32_t status        ;
extern uint32_t fault         ;
extern int32_t  total_counts  ;
extern uint32_t *encoder      ;
extern int32_t 	delta_counts  ;
extern uint32_t run_time      ;
extern float    speed_monitor ;
static float pulse_length = 1.0;
static uint32_t firmware_version = ( MAJOR << 16 ) + ( MINOR << 8 ) + BUILD;
extern uint32_t time_base_ms  ;
#define GAP { .data = { nullptr } } //



static int32_t i32u = 17150;
static float i32u_scale = 1.0f, i32u_min = -500, i32u_max = 30000.0f;

static uint32_t u32u = 343000;
static float u32u_scale = 0.1, u32u_min = 250.0, u32u_max = 350000; // 3500=0x88B8;

static float f32u = 17.150, f32u_scale = 100, f32u_min = 1.5, f32u_max= 651;


const std_t std_lut[MODBUS_STD_TBL_SIZE] = {
    { .type = std_t::i32, { .i32 = &i32u  }, std_t::RO, .scale = &i32u_scale },
    { .type = std_t::u32, { .u32 = &u32u  }, std_t::RO, .scale = &u32u_scale },
    { .type = std_t::f32, { .f32 = &f32u  }, std_t::RO, .scale = &f32u_scale },
    { .type = std_t::i32, { .i32 = &i32u  }, std_t::RW, .scale = &i32u_scale,  .min = &i32u_min,  .max = &i32u_max},
    { .type = std_t::u32, { .u32 = &u32u  }, std_t::RW, .scale = &u32u_scale,  .min = &u32u_min,  .max = &u32u_max},
    { .type = std_t::f32, { .f32 = &f32u },  std_t::RW, .scale = &f32u_scale,  .min = &f32u_min,  .max = &f32u_max},
};

// LUT to access 32bit value - first part is read only
const ext_t ext_lut[] = {
    { .type = ext_t::u32, { .u32 = &firmware_version      } },  // 16     // Information, implicit access=RO
    { .type = ext_t::u32, .data = { .u32 = &status        } },  // 18
    { .type = ext_t::u32, .data = { .u32 = &fault         } },  // 20
    { .type = ext_t::i32, .data = { .i32 = &total_counts  } },  // 22
    { .type = ext_t::u32, .data = { .u32 = encoder        } },  // 24 point to FTM2->CNT
    { .type = ext_t::u32, .data = { .i32 = &delta_counts  } },  // 26
    { .type = ext_t::u32, .data = { .u32 = &run_time      } },  // 28
    { .type = ext_t::f32, .data = { .f32 = &speed_monitor } },  // 30
    GAP,                                                        // 32
    { .type = ext_t::u32, .data = { .u32 = &time_base_ms  }, .access = ext_t::RW}, //  34 - RW
    { .type = ext_t::f32, .data = { .f32 = &pulse_length  }, .access = ext_t::RW}, //  36 RW
};

#endif
