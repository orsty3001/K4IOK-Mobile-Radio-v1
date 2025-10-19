# pragma once
#include <vector>
#include <string>
struct Memory { std::string name; double rx_MHz; double tx_MHz; std::string mode; };
std::vector<Memory> load_csv(const std::string& path);