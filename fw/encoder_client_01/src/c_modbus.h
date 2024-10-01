/*
 * c_modbus.h
 *
 *  Created on: Sep 25, 2024
 *      Author: mhg
 */

#ifndef C_MODBUS_H_
#define C_MODBUS_H_

#include <modbus.h>
#include "typedefs.h"

class c_modbus {
 public:
  c_modbus();
  bool GetData(uint8_t n, line_data_t &d);
  static bool memory_error;
  static const char *modbus_error;

 private:
  static modbus_t *ctx;

};

#endif /* C_ENCODER_H_ */
