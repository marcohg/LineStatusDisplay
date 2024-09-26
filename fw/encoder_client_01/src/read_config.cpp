
/*!
 * @file read_config.cpp
 * Reads or creates a config.txt file for communication
 *
 *
 *
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>

extern int slave_address;
extern std::string serial_port;
extern int baud;

int ReadConfiguration(const char *fname) {
  std::ifstream infile;
    infile.open(fname);
    if(infile.fail()) {
      std::cout << "Create a file with defaults\n";
      slave_address = 2;

      std::ofstream outfile(fname);
      outfile << "SlaveAddress " << 2 << "\n";
      outfile << "Port " << "/dev/ttyUSB0\n";
      outfile << "Baud " << 115200 << "\n";
      outfile.close();
      return 1;
    }
    else {
      std::string line;
      while (getline(infile, line)) {
        std::string name, value;
        std::istringstream ss(line);
        ss >> name >> value;
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
        std::cout << line << "->[" << name << "],[" << value << "]\n";

        //
        if(name.find("slaveaddress") != std::string::npos)
          slave_address = std::stoi(value);
        if(name.find("port") != std::string::npos)
          serial_port = value;
        if(name.find("baud") != std::string::npos)
          baud= std::stoi(value);
      }
      std::cout << "Slave: " << slave_address << ", port: " << serial_port << ", baud: "  << baud << "\n";
    }
    return 0;
}
