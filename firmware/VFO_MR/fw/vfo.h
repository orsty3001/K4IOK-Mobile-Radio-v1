# pragma once
#include <cstdint>
struct VFOState { double rx_MHz; double tx_MHz; double step_kHz; bool ptt; };
void vfo_step(VFOState&, int detents);