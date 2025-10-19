# include "vfo.h"
void vfo_step(VFOState& v, int d){ v.rx_MHz += v.step_kHz*d/1000.0; v.tx_MHz = v.rx_MHz; }