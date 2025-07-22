# Lightweight FPU Integration via RV32Zfinx
This project demonstrates how to enhance an RV32IM-based SoC with floating-point capability by integrating the Zurich CVFPU and enabling the RV32Zfinx extension.
It aims to evaluate the benefits of lightweight FPU support for softmax computation in edge AI workloads, especially in resource-constrained environments.

# Motivation
While implementing MobileBERT on a resource-limited IoT system, our team observed that the softmax function became a major performance bottleneck.
The exponential component (exp(x)) is particularly expensive on RV32IM platforms without FPU.
This project explores the potential of RV32Zfinx and CVFPU to accelerate softmax execution. I used Spike to benchmark performance, implemented CVFPU integration, and documented the results in this open-source, non-confidential report.

# Results
- Benchmark configuration: 512 float32 inputs, range [0, 10)
- Target platform: Spike simulator with RV32IMZf toolchain
- Measured via: mcycle CSR

|Configuration|	glibc expf() |	Optimized (Taylor3 + LUT)	| Speedup	Max Abs Error|
|-------------|-------------|-------------|-------------|
|RV32IM (no FPU)	  |1,265,689	|533,376	|57.86%	|0.0003|
|RV32IMZf (with FPU)|	66,291	  |51,246  	|22.70%	|0.0003|

# Observation
- Enabling RV32Zf alone (without software optimization) reduces execution time by ~95%.
- Software-side optimization (Taylor3 + LUT) offers ~23% additional improvement with RV32Zf.
- From an instruction-accurate perspective (using Spike), RV32Zf significantly improves softmax performance.

**Limitation:** Spike is a functional (ISA-level) simulator. It does not model pipeline stalls, cache behavior, or precise timing—thus cycle counts may differ from real hardware.

# Project Structure
RV32Zfinx-FPU-Integration-Note/
- spike_demo/
  - src/       # Source code: algo.c, main.c, etc.
  - run/       # Makefile and simulation outputs
- doc/           # Full report (markdown) and concept slides
- README.md      # This file

# Build & Run
To compile and run the benchmark:
```bash
cd spike_demo/run
make all                    # RV32IMZf (with FPU)
make clean build_base run   # RV32IM baseline (no FPU)
```

# Documentation
- Full technical report: doc/full-report.md
- Concept overview slide: doc/cvfpu_integration_slide.pdf
- RISC-V toolchain setup (Spike, pk, compiler): [Setup Guide (HackMD)](https://hackmd.io/Ouj3SnvZTQ-1iiaWfvpPMQ)

# Reference
- [CVFPU](https://github.com/openhwgroup/cvfpu) - Zurich Floating-Point Unit
- [riscv-isa-sim (Spike)](https://github.com/riscv/riscv-isa-sim)
