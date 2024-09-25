#!/usr/bin/bash
# read_write_std

export SERIAL=/dev/ttyUSB0
if [ $# -gt 0 ]; then  
export SERIAL=$1
fi
export ARGS="-a 2 -0 -1 -p none -b 115200"

# const ext_t ext_lut[] = {
#     { .type = ext_t::u32, { .u32 = &firmware_version      } },  // 16     // Information
#     { .type = ext_t::u32, .data = { .u32 = &status        } },  // 18
#     { .type = ext_t::u32, .data = { .u32 = &fault         } },  // 20
#     { .type = ext_t::i32, .data = { .i32 = &total_counts  } },  // 22
#     { .type = ext_t::u32, .data = { .u32 = encoder        } },  // 24 point to FTM2->CNT
#     { .type = ext_t::u32, .data = { .i32 = &delta_counts  } },  // 26
#     { .type = ext_t::u32, .data = { .u32 = &run_time      } },  // 28
#     { .type = ext_t::f32, .data = { .f32 = &speed_monitor } },  // 30
#     GAP,                                                        // 32
#     { .type = ext_t::u32, .data = { .u32 = &time_base_ms  }, .access = ext_t::RW}, //  34 - RW
#     { .type = ext_t::f32, .data = { .f32 = &pulse_length  }, .access = ext_t::RW}, //  36 RW
# };

echo "Firmware Version - speed monitor"
modpoll -t 4:i32 -r 16 -c 7 $ARGS $SERIAL | grep "\["
echo -------------
echo "Read speed monitor"
modpoll -t 4:f32 -r 30 -c 1 $ARGS $SERIAL | grep "\["
echo -------------
echo "Read GAP (undefined)"
modpoll -t 4 -r 32 $ARGS $SERIAL 
echo -------------
# for D in 25000 33000; do 
# echo "Write to scaled [3] mydata $D"
# modpoll -t 4 -r 3 $ARGS $SERIAL $D | grep -e 'Written' && modpoll -t 4 -r 2 -c 1 $ARGS $SERIAL | grep "\["
# echo -------------
# done
 
