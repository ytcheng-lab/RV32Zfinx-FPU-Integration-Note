# RTL Simulation: Softmax Benchmark Result on RV32IM and RV32IMZf

**Environment:** RTL simulation using `ncverilog 15.20`

**Input:** `N = 512`, input range `[0, 10)`

| Configuration           | glibc expf() | Optimized (Taylor3 + LUT) | Speedup    |
|-------------------------|--------------|----------------------------|------------|
| RV32IM (no FPU)         |  6,658,844  | 2,757,466                    | 58.6%    |
| RV32IMZf (with FPU)     | 2,077,046       | 8,085,280                | x (slower)  |


### 🔍 Interpretation

* Under **RTL simulation**, the use of CVFPU did **not** result in performance gain for this softmax implementation.
* **FPU pipeline stalls** were too long and the lack of pipelining caused latency to accumulate.

### ✅ Conclusion

* On RV32IM:

  * C-level optimization (Taylor + LUT) is effective (\~58.6% reduction).
* On RV32IMZf:

  * glibc version performs better than approximation due to **FPU integration latency**.
* Therefore, **do not apply approximate softmax directly on Zf machines** without pipelined FPU or latency-aware scheduling.
* **Overall reduction with FPU (\~68.6%) is possible**, but only using glibc + FPU, **not approximation**.
