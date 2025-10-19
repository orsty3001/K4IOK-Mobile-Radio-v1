#pragma once
#include <vector>
#include <cmath>
float biquad(float x, float *z, const float *coef);
void design_bandpass(float fs, float f0, float Q, float *coef);
