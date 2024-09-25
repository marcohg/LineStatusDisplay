#!/usr/bin/bash
# read_write_std

export SERIAL=/dev/ttyUSB0
if [ $# -gt 0 ]; then  
export SERIAL=$1
fi
export ARGS="-a 2 -0 -1 -p none -b 115200"

# const std_t std_lut[MODBUS_STD_TBL_SIZE] = {
#     { .type = std_t::i32, { .i32 = &my_i32  }, std_t::RO  .scale = &i32_scale },
#     { .type = std_t::u32, { .u32 = &my_u32  }, std_t::RO  .scale = &u32_scale },
#     { .type = std_t::f32, { .f32 = &my_f32  }, std_t::RO  .scale = &f32_scale },
#     { .type = std_t::i32, { .i32 = &my_i32  }, std_t::RW, .scale = &i32_scale,  .min = &i32_min,  .max = &i32_max},
#     { .type = std_t::u32, { .u32 = &my_u32  }, std_t::RW, .scale = &u32_scale,  .min = &u32_min,  .max = &i32_max},
#     { .type = std_t::f32, { .f32 = &my_f32  }, std_t::RW, .scale = &f32_scale,  .min = &i32_min,  .max = &i32_max},
# };

read_all_data() {
echo "Read all six data"
modpoll -t 4 -r 0 -c 6 $ARGS $SERIAL | grep "\["
echo -------------
}

# echo "LUT starting at 0 to 16, vars are 32bits, modbus access is 16b"
# read_all_data

# UA=6
# echo "Read undefined address [$UA]"
# modpoll -t 4 -r $UA -c 3 $ARGS $SERIAL | grep "\["
# echo -------------

# D=17150
# echo "Write $D to read-only [0]"
# modpoll -t 4 -r 0 $ARGS $SERIAL $D | grep -e 'Written' && modpoll -t 4 -r 2 -c 1 $ARGS $SERIAL 
# echo -------------

test_write() {
  echo "write $WRITE to address $1"
  for D in $WRITE; do
  echo "Write to scaled [$1] = $D"
  modpoll -t 4 -r $1 $ARGS $SERIAL $D | grep -e 'Written' && modpoll -t 4 -r $1 -c 1 $ARGS $SERIAL | grep "\["
  echo -------------
  done
} 

# echo i32[3] -500 to 3000, scale 1.0
# WRITE="-500 0 3000 -501 3001"
# test_write 3

# echo u32[4], 250 to 350'000, scale 0.1
# WRITE="250 0x88B8"  
# test_write 4

# float is interpreted as uint or int depending on min >=0 or <0 respectively
echo f32[5], 1.5 to 651, scale 100

WRITE="150 0xFDEB"  # 65000=0xFDEB  
test_write 4
