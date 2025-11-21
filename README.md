# Lightweight FPU Integration via RV32Zfinx

## 📌 Summary
> **This project integrates the Zurich CVFPU into an RV32IM-based SoC to enable the RV32Zfinx extension, improving softmax performance by ruducing 69% execution cycles in RTL simulation (ncverilog)**

## Motivation
While implementing MobileBERT on a resource-limited IoT platform, softmax became a significant performance bottleneck due to its dependence on `exp(x)`, which is costly without floating-point support.  
This project explores the integration of CVFPU to enable RV32Zfinx, allowing the core to execute floating-point operations using integer registers.  
The goal is to validate whether lightweight FPU support can significantly improve AI inference workloads such as softmax on RV32IM-class systems.

## Results
**Spike simulation**
To evaluate the impact of enabling floating-point instructions (F) as well as the remaining need for approximation-based optimizations, I conducted two Spike-based experiments.
All tests use:
- Input: 512 `float32` values in range `[0, 10)`
- Target: Spike simulator using RV32IMF toolchain
- Measurement: Cycle count via `mcycle` CSR

Experiment 1:
- Goal: Determine whether the Taylor3+LUT approximation still provides meaningful speedup when F instructions are supported.
- Observation: Enabling F instructions makes the baseline expf() execution much faster, which reduces the relative benefit of approximation. While approximation is highly effective on RV32IM, its **contribution becomes smaller on RV32IMF** because the FPU already removes most of the bottleneck.

| Configuration           | glibc expf() | Optimized (Taylor3 + LUT) | Cycle Reduction    | Max Abs Error |
|-------------------------|--------------|----------------------------|------------|----------------|
| RV32IM (no F)           | 1,265,689    | 533,376                    | 57.86%     | 0.0003         |
| RV32IMZf (with F)       | 66,291       | 51,246                     | 22.70%     | 0.0003         |

Experiment 2:
- Goal: Evaluate the direct effect of F instructions on the baseline glibc expf().
- Observation: With F instructions enabled, Spike simulation shows that expf() becomes **~95% faster** compared with RV32IM software emulation.
This result highlights the importance of adding floating-point support for workloads dominated by exponential functions.

| Configuration           | glibc expf() | Cycle Reduction  |
|-------------------------|--------------| -----------|
| RV32IM (no F)         | 1,265,689    | baseline   |
| RV32IMZf (with F)     | 66,291       |   95%      |

**RTL simulation:** 
- Input: 512 `float32` values in range `[0, 10)`
- Target: ncverilog with RTL design
- Measurement: Cycle count via `mtime` CSR
- Compare the execution speed of `expf()` with FPU and without FPU

| Configuration           | glibc expf() | Speedup        |
|-------------------------|--------------|----------------|
| RV32IM (no FPU)         | 6,658,844    | -              |
| RV32IMZf (with FPU)     |  2,077,046   | 68.81%         |

More details and observation of rtl simulation refer to [rtl_evaluation](https://github.com/ytcheng-lab/RV32Zfinx-FPU-Integration-Note/blob/main/docs/rtl_evaluation.md)

## Observation
- The RV32Zf extension reduces the softmax execution time by approximately 69% when using glibc’s expf() in cycle-accurate RTL simulation.
- Theoretically, software-level optimization (Taylor-series of order 3 + LUT-based approximation) has the potential to provide an additional ~23% improvement. However, the performance observed in the Spike-based testing environment, which measures instruction-level efficiency, does not directly correlate with the cycle-accurate RTL simulation results.
- Limitation: Spike does not model real hardware stalls, memory latency, or cache effects; its results represent functional-cycle estimates only.

## Project Structure
RV32Zfinx-FPU-Integration-Note/
- `spike_demo/`
  - `src/` — Source code (`algo.c`, `main.c`, etc.)
  - `run/` — Makefile and simulation outputs
- `doc/` — Full report (`full-report.md`) and concept slide
- `README.md` — This document

## Build & Run
To compile and benchmark:
```bash
cd spike_demo/run
make all                     # Run with RV32IMZf (with F instructions)
make clean build_base run    # Run baseline RV32IM (without F instructions)
```

# Documentation
- Full technical report: doc/full-report.md
- Concept overview slide: doc/cvfpu_integration_slide.pdf
- RISC-V toolchain setup (Spike, pk, compiler): [Setup Guide (HackMD)](https://hackmd.io/Ouj3SnvZTQ-1iiaWfvpPMQ)

# Reference
- [CVFPU](https://github.com/openhwgroup/cvfpu) - Zurich Floating-Point Unit
- [riscv-isa-sim (Spike)](https://github.com/riscv/riscv-isa-sim)
