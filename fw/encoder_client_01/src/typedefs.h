/*
 * typedefs.h
 *
 *  Created on: Sep 26, 2024
 *      Author: marco
 */

#ifndef _SRC_TYPEDEFS_H_
#define _SRC_TYPEDEFS_H_
#include <cstdint>

typedef struct node_tag {
  int32_t node_address;
  uint32_t time_base;

  bool connected;
  const char *connect_status;
  uint32_t counts;
  float speed_hz;
} node_t;

typedef struct config_tab {
  std::string serial_port;
  uint32_t baud;
  uint32_t poll_rate;
  uint32_t timeout;
} config_t;

typedef struct line_data_tag {
  uint32_t status;
  uint32_t run_time;
  int32_t total_counts;
  int32_t delta_counts;
//  uint32_t encoder;
} line_data_t;

#endif /* TYPEDEFS_H_ */
