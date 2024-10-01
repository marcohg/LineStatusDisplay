#!/usr/bin/bash
# read_write_std

export SERIAL=/dev/ttyUSB0
if [ $# -gt 0 ]; then  
export SERIAL=$1
fi
export ARGS="-a 2 -0 -p none -b 115200"

# // LUT to access 32bit value - first part is read only
# const ext_t ext_lut[] = {
#     { .type = ext_t::u32, .data = { .u32 = &status        } },  // 16
#     { .type = ext_t::u32, .data = { .u32 = &run_time      } },  // 18
#     { .type = ext_t::i32, .data = { .i32 = &total_counts  } },  // 20
#     { .type = ext_t::u32, .data = { .i32 = &delta_counts  } },  // 22
#     { .type = ext_t::f32, .data = { .f32 = &speed_monitor } },  // 24
#     { .type = ext_t::u32, { .u32 = &firmware_version      } },  // 26     // Information, implicit access=RO
#     { .type = ext_t::u32, .data = { .u32 = encoder        } },  // 28 point to FTM2->CNT
#     GAP,                                                        // 30
#     GAP,                                                        // 32
#     GAP,                                                        // 34
#     GAP,                                                        // 36
#     GAP,                                                        // 38
#     { .type = ext_t::u32, .data = { .u32 = &serial_command}, .access = ext_t::RW}, //  40 - RW
#     { .type = ext_t::u32, .data = { .u32 = &time_base_ms  }, .access = ext_t::RW}, //  42 - RW
#     { .type = ext_t::f32, .data = { .f32 = &pulse_length  }, .access = ext_t::RW}, //  44  RW
# };

echo "Read-Only status, runtime, total counts, delta"
modpoll -t 4:i32 -r 16 -c 4 -1 $ARGS $SERIAL | grep "\["
echo -------------
# echo "Read speed monitor"
# modpoll -t 4:f32 -r 30 -c 1 $ARGS $SERIAL | grep "\["
# echo -------------
# echo "Read GAP (undefined)"
# modpoll -t 4 -r 32 $ARGS $SERIAL 
# echo -------------

# for D in 25000 33000; do 
# echo "Write to scaled [3] mydata $D"
# modpoll -t 4 -r 3 $ARGS $SERIAL $D | grep -e 'Written' && modpoll -t 4 -r 2 -c 1 $ARGS $SERIAL | grep "\["
# echo -------------
# done
 
