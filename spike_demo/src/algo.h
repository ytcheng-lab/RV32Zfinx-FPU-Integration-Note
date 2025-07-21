#include <stdint.h>

void softmax_golden(float* in, float* out, int N);
void softmax_taylor(float* in, float* out, int N);
float expth3 (float x);
void softmax_taylor_LUT (float* in, float* out, int N);
float expth3_LUT (float x);
