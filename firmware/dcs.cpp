#include "dcs.h"
#include <cmath>
std::vector<float> dcs_synth(uint16_t code,bool inv,float fs,float dur){
 int N=fs*dur; std::vector<float> y(N); float w=2*M_PI*134.4/fs; float phase=0;
 for(int n=0;n<N;++n){phase+=w; y[n]=0.2*sinf(phase);}
 return y;
}
