/** @file
 * typedefs.h
 *
 *  Sept 6, 2022
 *      Author: MHG
 */

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#include <stdint.h>

/**
 * - Parameter access using Standard lookup table (lut_std) are seen as int16, index is 0 - (std_size-1)
 * - Access using the Extended table (lut_ext) are seen as 32bits with big endian slave, index >256
 *
 */

/// Application data is 32bits, uint32, int32 and float
union p32_t {
  float   *f32;
  int32_t *i32;
  uint32_t *u32;
};


/// LUT Standard 16bit access
typedef struct parameter_std_tag {
//  enum access_t {
//    ReadOnlyLow,
//    ReadOnlyHigh,
//    ReadOnlyScaledInt,
//    ReadOnlyScaledUint,
//    ReadOnlyScaledFloat,
//
//    ReadWriteLow,
//    ReadWriteHigh,
//    ReadWriteScaledInt,
//    ReadWriteScaledUint,
//    ReadWriteScaledFloat,
//
//    ReadWriteProtected,
//
//  };
  enum type_t {
    i32, u32, f32
  };
  enum access_t {
    RO, RW, RWP
  };
  type_t type;
  p32_t data;  //  v32_t data;
  access_t access;
  float *scale;  // use float * to scale on the fly and also to allow exponent scale be load from nvm
  float *min;
  float *max;
} std_t;

/// LUT Extended 32bit access
typedef struct parameter_ext_tag {
  enum type_t {
    none, i32, u32, f32
  };
  enum access_t {
    RO, RW, RWP
  };
  type_t type;
  p32_t data;
  access_t access;
} ext_t;


// Structures to communicate with application
typedef struct modbus_read_tag {
  bool received;
  uint8_t self_address;
  uint8_t function_code;
  uint8_t quantity;
  uint16_t address;
} modbus_read_t;

typedef struct modbus_write_tag {
  bool received;
  uint8_t self_address;
  uint8_t function_code;
  uint8_t quantity;
  uint16_t address;
  int16_t *data;
} modbus_write_t;

typedef struct modbus_write_read_tag {
  bool received;
  uint8_t self_address;
  uint8_t function_code;
  uint16_t rd_address;
  uint16_t rd_quantity;
  uint16_t wr_address;
  uint16_t wr_quantity;
  uint8_t *wr_data;
} modbus_write_read_t;


#endif

