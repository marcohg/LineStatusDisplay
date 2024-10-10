
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
#include "typedefs.h"
#include <chrono>
#include <thread>
//  Default file
//    std::cout << "Create a file with defaults\n";
//    std::ofstream outfile(fname);
//    outfile << "Port " << cfg->serial_port << "\n";
//    outfile << "Baud " << cfg->baud << "\n\n";
//
//    outfile << "---\tNode\tAddress\tTime_base_ms\n";
//    for (int i = 0; i < nn; ++i)
//      outfile << "Node\t" << i + 1 << "\t" << node[i].node_address << "\t " << node[i].time_base << "\n";
//    outfile << "--end--\n";
//    outfile.close();

bool file_doesnot_exist(const char *fname) {
  std::ifstream infile;
  infile.open(fname);
  return infile.fail();
}

void ReadConfiguration(const char *fname, node_t *node, config_t *cfg, const int nn) {

  while(file_doesnot_exist(fname) ) {
    static bool splash = false;
    if(!splash) {
      splash = true;
      std::cout << "Waiting for: " << fname << " to be created.\r\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }

  std::ifstream infile;
  infile.open(fname);
  std::string line;
  while (getline(infile, line)) {
    std::string n, v1, v2, v3;
    std::istringstream ss(line);
    ss >> n >> v1 >> v2 >> v3;
    std::transform(n.begin(), n.end(), n.begin(), [](unsigned char c) {
      return std::tolower(c);
    });
    std::cout << "[" << n << "],[" << v1 << "],[" << v2 << "],[" << v3 << "]\n";
    if (n.find("node") != std::string::npos) {
      int index = std::stoi(v1) - 1;
      if (index >= 0 && index < nn) {
        node[index].node_address = std::stoi(v2);
        node[index].time_base = std::stoi(v3);
      }

    }
    if (n.find("port") != std::string::npos)
      cfg->serial_port = v1;
    if (n.find("baud") != std::string::npos)
      cfg->baud = std::stoi(v1);
  }
  infile.close();
//      std::cout << "Slave: " << mb_address << ", port: " << serial_port << ", baud: "  << baud << "\n";
}
