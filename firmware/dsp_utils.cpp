#include "dsp_utils.h"
float biquad(float x, float *z, const float *c){
    float y = c[0]*x + c[1]*z[0] + c[2]*z[1] - c[3]*z[2] - c[4]*z[3];
    z[1]=z[0]; z[0]=x; z[3]=z[2]; z[2]=y; return y;
}
void design_bandpass(float fs,float f0,float Q,float *c){
    float w0=2*M_PI*f0/fs; float alpha=sinf(w0)/(2*Q); float cosw=cosf(w0);
    c[0]=alpha; c[1]=0; c[2]=-alpha; c[3]=-2*cosw; c[4]=1-alpha;
}
