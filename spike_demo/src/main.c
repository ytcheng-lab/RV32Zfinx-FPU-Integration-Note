#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "time_count.h"
#include "algo.h"

#ifdef DEBUG
// do not use external N
#else
extern int N;
#endif
extern float i[] __attribute__((aligned(8)));
extern float o_g[] __attribute__((aligned(8))); // output matrix for golden
extern float o_t[] __attribute__((aligned(8))); // output matrix for test

int main()
{
    #ifdef DEBUG
    int N = 10;
    #endif
    float err = 0;
    float max_err = 0;

    uint32_t cycles_baseline, cycles_speedup;
    printf("=======================================================\n");
    printf("Softmax Benchmark: %d float32 inputs [0, 10)\n", N);
#ifdef FPU
    printf("Target: RV32IMZf, Measured via mcycle\n");
#else
    printf("Target: RV32IM (no FPU), Measured via mcycle\n");
#endif
    printf("Method: Taylor series + LUT\n");
    printf("=======================================================\n");
 
    // --- CMATH: Test baseline and golden value ---
    cycles_baseline = read_cycle();
    softmax_golden(i, o_g, N);
    cycles_baseline = read_cycle() - cycles_baseline;

    // --- SPEEDUP: Test speedup version ---
    cycles_speedup = read_cycle();
    softmax_taylor_LUT(i, o_t, N);
    cycles_speedup = read_cycle() - cycles_speedup;

    // --- VERIFY: Compare Test with golden ---
    for (int k = 0; k < N; k++) {
	err = fabs(o_g[k] - o_t[k]);
        #ifdef DEBUG	
            printf("input: %f, golden: %f, speedup: %f\n", i[k], o_g[k], o_t[k]);
        #endif
	if(err > max_err) {
		max_err = err; 
	}
    }
   
    float result = cycles_speedup / 1.0 / cycles_baseline;
    float speedup_percentage = (1 - result) * 100.0f;

    printf("=======================================================\n");
#ifdef FPU
    printf("                   Result (with FPU)                   \n");
#else
    printf("                  Result (without FPU)                 \n");
#endif
    printf("=======================================================\n");
    printf("[Baseline]  glibc expf():       %u cycles\n", cycles_baseline);
    printf("[Optimized] Taylor3 + LUT:      %u cycles\n", cycles_speedup);
    printf("[Speedup]   %.2f%% time reduction\n", speedup_percentage);
    printf("[Accuracy]  Max absolute error: %.4f\n", max_err);

}
