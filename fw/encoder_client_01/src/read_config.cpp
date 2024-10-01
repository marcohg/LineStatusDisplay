
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

int ReadConfiguration(const char *fname, node_t *node, config_t *cfg, const int nn) {
  std::ifstream infile;
  infile.open(fname);
  if (infile.fail()) {
    std::cout << "Create a file with defaults\n";
    std::ofstream outfile(fname);
    outfile << "Port " << cfg->serial_port << "\n";
    outfile << "Baud " << cfg->baud << "\n\n";

    outfile << "---\tNode\tAddress\tTime_base_ms\n";
    for (int i = 0; i < nn; ++i)
      outfile << "Node\t" << i + 1 << "\t" << node[i].node_address << "\t " << node[i].time_base << "\n";
    outfile << "--end--\n";
    outfile.close();
    return 1;
  } else {
    std::string line;
    while (getline(infile, line)) {
      std::string n, v1, v2, v3;
      std::istringstream ss(line);
      ss >> n >> v1 >> v2 >> v3;
      std::transform(n.begin(), n.end(), n.begin(),[](unsigned char c) {return std::tolower(c);});
      std::cout << "[" << n << "],[" << v1 << "],[" << v2 << "],[" << v3 << "]\n";
      if (n.find("node") != std::string::npos) {
        int index = std::stoi(v1) - 1;
        if (index >= 0 && index < nn) {
          node[index].node_address = std::stoi(v2);
          node[index].time_base = std::stoi(v3);
        }

      }
      if (n.find("port") != std::string::npos)
        cfg->serial_port= v1;
      if (n.find("baud") != std::string::npos)
        cfg->baud = std::stoi(v1);
    }
    infile.close();
//      std::cout << "Slave: " << mb_address << ", port: " << serial_port << ", baud: "  << baud << "\n";
  }
  return 0;
}
