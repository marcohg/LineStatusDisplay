#include <iostream>

extern int TestModbusEncoder(const char *serial_port);
extern int ReadConfiguration(const char *fname);

// These are default values
int slave_address = 2;
std::string serial_port("/dev/ttyUSB0");
int baud = 115200;

int main(int argc, char *argv[]) {

  if( int read_config = ReadConfiguration("config.txt") == 0) {
    TestModbusEncoder(serial_port.c_str());
  }
  else {
    return read_config;
  }
}
