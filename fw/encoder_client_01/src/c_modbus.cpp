/*
 * c_encoder.cpp
 *
 *  Created on: Sep 25, 2024
 *      Author: mhg
 */
#include <iostream>
#include "globals.h"
#include "c_modbus.h"
#include "typedefs.h"
// modbus
#include <stdio.h>
#include <modbus.h>
#include <errno.h>

modbus_t *c_modbus::ctx;
const char *c_modbus::modbus_error;
bool c_modbus::memory_error = false;
const char *connect_success = "good";

c_modbus::c_modbus() {
  ctx = modbus_new_rtu(cfg.serial_port.c_str(), cfg.baud, 'N', 8, 1);
  if (ctx != NULL) {
    // Scan nodes
    for (int i = 0; i < max_nodes; ++i) {
      if (nodes[i].node_address > 0) {
        modbus_set_slave(ctx, nodes[i].node_address);
        if (modbus_connect(ctx) == -1) {
          nodes[i].connected = false;
          nodes[i].connect_status = modbus_strerror(errno);
        } else {
          nodes[i].connected = true;
          nodes[i].connect_status = connect_success;
        }
      }
    }
    // int modbus_set_response_timeout(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec);
    if(cfg.timeout_ms > 999) {
      uint32_t secs = cfg.timeout_ms/1000; // ms-sec
      modbus_set_response_timeout(ctx, secs, 0);
    }
    else {
      uint32_t us = cfg.timeout_ms * 1000;
      modbus_set_response_timeout(ctx, 0, us);
    }
  } else {
    modbus_error = modbus_strerror(errno);
    modbus_free(ctx);
    memory_error = true;
  }
}

bool c_modbus::GetData(uint8_t n, line_data_t &d) {
  modbus_set_slave(ctx, nodes[n].node_address);
  uint16_t tab_reg[8];
  int rc = modbus_read_registers(ctx, 16, 8, tab_reg);
  if (rc == -1) {
    printf("Failed man!\n");
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    return false;
  }
  d.status = *((uint32_t*) &tab_reg[0]);
  d.run_time = *((uint32_t*) &tab_reg[2]);
  d.total_counts = *((int32_t*) &tab_reg[4]);
  d.delta_counts = *((int32_t*) &tab_reg[6]);

  return true;
}

