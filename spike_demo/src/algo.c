#include <stdio.h>
#include <math.h>
#include "algo.h"

#define C -0.693147180 //ln2 for fast log2
#define T3 (0.16666666666666666666666666666667f)
#define T2 (0.5f)
#define T1 (1.0f)

extern float buf[] __attribute__((aligned(8)));

uint32_t LUT[40] = {
    0x3f800000,
    0x402df854,
    0x40ec7326,
    0x41a0af2e,
    0x425a6481,
    0x431469c5,
    0x43c9b6e3,
    0x44891443,
    0x453a4f54,
    0x45fd38ac,
    0x46ac14ee,
    0x4769e224,
    0x481ef0b3,
    0x48d805ad,
    0x4992cd62,
    0x4a478665,
    0x4b07975f,
    0x4bb849a4,
    0x4c7a7910,
    0x4d2a36c8,
    0x4de75844,
    0x4e9d3710,
    0x4f55ad6e,
    0x5011357a,
    0x50c55bfe,
    0x51861e9d,
    0x52364993,
    0x52f7c118,
    0x53a85dd2,
    0x5464d572,
    0x551b8238,
    0x55d35bb3,
    0x568fa1fe,
    0x5743379a,
    0x5804a9f1,
    0x58b44f11,
    0x597510ad,
    0x5a2689fe,
    0x5ae2599a,
    0x5b99d21f
};

union UIntFloat { uint32_t i; float f;};

void softmax_golden(float* in, float* out, int N) {
    float exp_sum = .0f;
    for (int i = 0; i < N; i++) {
        buf[i] = expf(in[i]);
	exp_sum += buf[i];
        #ifdef DEBUG
	    printf("exp(%f) = %f\n", in[i], buf[i]);
        #endif
    }
    for (int i = 0; i < N; i++) {
        out[i] = buf[i]/exp_sum;
    }
    return;
}

void softmax_taylor(float* in, float* out, int N){
    float exp_sum = .0f;
    for (int i = 0; i < N; i++) {
        buf[i] = expth3(in[i]);
	exp_sum += buf[i];
        #ifdef DEBUG
            printf("exp(%f) = %f\n", in[i], buf[i]);
        #endif
    }
    for (int i = 0; i < N; i++) {
        out[i] = buf[i]/exp_sum;
    }
 
    return;
}

void softmax_taylor_LUT(float* in, float* out, int N){
    float exp_sum = .0f;
    for (int i = 0; i < N; i++) {
        buf[i] = expth3_LUT(in[i]);
	exp_sum += buf[i];
        #ifdef DEBUG
            printf("exp(%f) = %f\n", in[i], buf[i]);
        #endif
    }
    for (int i = 0; i < N; i++) {
        out[i] = buf[i]/exp_sum;
    } 
    return;
}

float expth3 (float x) {
    float ans = x*T3 + T2;
    ans = x*ans + T1;
    ans = x*ans + 1;
    return ans;
}


float expth3_LUT (float x) {
    int n = (int)x;
    union UIntFloat u;
    u.i = LUT[n];
    float exp_n = u.f;
    #ifdef DEBUG
        printf("LUT %d = %f\n", n, exp_n);
    #endif
    float f = x-n;
    float exp_f = f*T3 + T2;
    exp_f = f*exp_f + T1;
    exp_f = f*exp_f + 1;    
    return exp_n*exp_f;
}
