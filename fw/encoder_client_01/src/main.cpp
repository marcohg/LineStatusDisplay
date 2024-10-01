#include <iostream>
#include "globals.h"
#include "c_modbus.h"


int ReadConfiguration(const char *fname, node_t *line, config_t *cfg, const int nn);
extern int TestModbusEncoder(const char *serial_port);

// Default values
const int max_nodes = 4;
node_t nodes[max_nodes] = {
    { .node_address = 2, .time_base = 1000 }
};
config_t cfg = {
    .serial_port = "/dev/ttyUSB0",
    .baud = 115200,
    .poll_rate = 1000,
    .timeout  = 100
};

#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {
  line_data_t line1;
  if( int read_config = ReadConfiguration("config.txt", nodes, &cfg, max_nodes) == 0) {
    c_modbus line_encoder;  // create the object
    if(line_encoder.memory_error) {
      std::cout << "Error " << line_encoder.modbus_error << "\n";
      return 2;
    }
    // Logic:
    //  - run: communicate all connected (active) nodes
    //  - service: connect, configure
    while (1) {
      if (line_encoder.GetData(0, line1)) {
        float hz = (float) line1.delta_counts * 1000 / 4 / nodes[0].time_base;
        std::cout << line1.status << "," << line1.run_time << "," << line1.total_counts << "," << line1.delta_counts
                  << "," << hz << "\n";
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
//    TestModbusEncoder(cfg.serial_port.c_str());
  }
  else {
    return read_config;
  }
  // test c_modbus destructor
}
