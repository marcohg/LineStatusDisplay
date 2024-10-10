#include <iostream>
#include <fstream>
#include "globals.h"
#include "c_modbus.h"


void ReadConfiguration(const char *fname, node_t *line, config_t *cfg, const int nn);
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
//#include <unistd.h>




int main(int argc, char *argv[]) {
  line_data_t line1;
  ReadConfiguration("/mnt/tmp/line_encoder.cfg", nodes, &cfg, max_nodes); // wait for file

  c_modbus line_encoder;  // create the object
  if (line_encoder.memory_error) {
    std::cout << "Error " << line_encoder.modbus_error << "\n";
    return 2;
  }

  std::ofstream out;
  std::string status_file("/mnt/tmp/encoder_client.status");
  out.open(status_file, std::ios_base::app);  // overwrite here
  out << "Hello Marco!!\n";
  out.close();

  while (1) {
    //  - run: communicate all connected (active) nodes
    //  - service: connect, configure
    if (line_encoder.GetData(0, line1)) {
      float hz = (float) line1.delta_counts * 1000 / 4 / nodes[0].time_base;
      // std::cout
      std::cout << line1.status << "," << line1.run_time << "," << line1.total_counts << "," << line1.delta_counts
          << "," << hz << "\n";
      out.open(status_file, std::ios_base::app);
            out << line1.status << "," << line1.run_time << "," << line1.total_counts << "," << line1.delta_counts
                      << "," << hz << "\n";
      out.close();

    }
    else
      std::cout << ".\r\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    sleep(0.5);
  }
//
////    TestModbusEncoder(cfg.serial_port.c_str());
//  }
//  else {
//    return read_config;
//  }
//  // test c_modbus destructor
}
