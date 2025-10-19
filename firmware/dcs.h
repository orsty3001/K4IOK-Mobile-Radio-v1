#pragma once
#include <vector>
#include <cstdint>
std::vector<float> dcs_synth(uint16_t code, bool inverted, float fs, float dur_s);
