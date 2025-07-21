# RV32Zfinx-FPU-Integration-Note

This repository documents the integration of the Zurich CVFPU into an in-house RV32IM SoC using the RV32Zfinx extension. The goal was to enable scalar floating-point computation with minimal area overhead by reusing the integer register file (`x0–x31`) as specified in the Zfinx ISA extension.

## Motivation

Traditional RV32F requires a separate floating-point register file, complicating hardware and ABI support. RV32Zfinx allows reuse of integer registers, making it attractive for resource-constrained systems and enabling simpler toolchain support.

## Project Scope

This integration involved:
- Evaluating the potential of RV32Zf on an RV32IM pipeline using Spike
- Validating CVFPU behavior through standalone testbenches
- Integrating CVFPU into a 5-stage RV32IM pipeline using the Zfinx convention
- Debugging decoder changes, pipeline hazards, and FCSR logic
- Synthesizing the netlist and addressing backend timing issues
- Fixing toolchain bugs related to `.bss.*` section initialization

## Key Features

- ✅ Full support for RV32Zfinx instruction groups
- ✅ FCSR compatibility with Zicsr
- ✅ Integration via handshaking protocol and FSM control
- ✅ Hazard detection logic for FPU & CSR interlocks
- ✅ Verified by riscv-tests and self-developed directed tests

## Repository Structure
- /docs - Concept note (full integration report)
- /spike
  - /run - RTL simulation flow and testcase Makefiles
  - /src - Integration source code: FSM, pipeline, CSR, CVFPU interface
- /testbench - Standalone testbenches for CVFPU & instruction decoding
