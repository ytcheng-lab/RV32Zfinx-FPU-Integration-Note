# Lightweight FPU Integration via RV32Zfinx

## 📌 Summary
> **This project integrates the Zurich CVFPU into an RV32IM-based SoC to enable the RV32Zfinx extension, improving softmax performance by over 95% compared to software-only execution on RV32IM.**


## Motivation
While implementing MobileBERT on a resource-limited IoT platform, softmax became a significant performance bottleneck due to its dependence on `exp(x)`, which is costly without floating-point support.  
This project explores the integration of CVFPU to enable RV32Zfinx, allowing the core to execute floating-point operations using integer registers.  
The goal is to validate whether lightweight FPU support can significantly improve AI inference workloads such as softmax on RV32IM-class systems.

## Results
- Input: 512 `float32` values in range `[0, 10)`
- Target: Spike simulator using RV32IMZf toolchain
- Measurement: Cycle count via `mcycle` CSR

| Configuration           | glibc expf() | Optimized (Taylor3 + LUT) | Speedup    | Max Abs Error |
|-------------------------|--------------|----------------------------|------------|----------------|
| RV32IM (no FPU)         | 1,265,689    | 533,376                    | 57.86%     | 0.0003         |
| RV32IMZf (with FPU)     | 66,291       | 51,246                     | 22.70%     | 0.0003         |

## Observation
- RV32Zf alone reduces softmax execution time by approximately 95%.
- Combined with software-level optimization (Taylor3 + LUT), additional 23% improvement is achieved.
- Spike-based benchmarking confirms substantial instruction-level performance gain.
- Limitation: Spike does not model real hardware stalls or cache effects; results are functional-cycle estimates.

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
make all                     # Run with RV32IMZf (with FPU)
make clean build_base run    # Run baseline RV32IM (no FPU)
```

# Documentation
- Full technical report: doc/full-report.md
- Concept overview slide: doc/cvfpu_integration_slide.pdf
- RISC-V toolchain setup (Spike, pk, compiler): [Setup Guide (HackMD)](https://hackmd.io/Ouj3SnvZTQ-1iiaWfvpPMQ)

# Reference
- [CVFPU](https://github.com/openhwgroup/cvfpu) - Zurich Floating-Point Unit
- [riscv-isa-sim (Spike)](https://github.com/riscv/riscv-isa-sim)
