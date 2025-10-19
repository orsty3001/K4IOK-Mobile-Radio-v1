#include "ctcss.h"
#include <cmath>
void ctcss_generate(float f,float fs,float dur,std::vector<float>&out){
 int N=fs*dur; out.resize(N);
 for(int n=0;n<N;++n) out[n]=0.25*sinf(2*M_PI*f*n/fs);
}
