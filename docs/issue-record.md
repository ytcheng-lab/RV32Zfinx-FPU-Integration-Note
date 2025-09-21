# Issue Record: RV32Zfinx FPU Integration

This document records debugging issues encountered during the RV32Zfinx + CVFPU integration process. Each issue includes a description, root cause analysis, and resolution.

---

## Issue 1 – Illegal Instruction Trap

**Symptom:** Trap with `mcause = 0x2`

**Root Cause:**

* In the EX stage, unsupported opcodes are flagged as illegal.
* `illegal_instruction` signal is asserted and routed to the control unit.
* Control unit triggers exception handling and writes `mcause`.

**Details:**

* The opcode list must include all legal RV32IM\[Zf] instructions.
* Overlapping opcodes (e.g., MUL/DIV vs. RV32I) need conditional handling.

**Fix:**

* Ensure all Zf-type opcodes are conditionally included.

---

## Issue 2 – `ra` Accidentally Written

**Symptom:** Trap with `mcause = 0x0` (Instruction address misaligned)

**Root Cause:**

* `mepc = 0x180`, `mtval = 0x17c` shows jump return (JALR) executed.
* `ra` (return address register) was overwritten by illegal value.

**Fix:**

* Prevent `ra` pollution by verifying all exception paths initialize correctly.

---

## Issue 3 – Unaligned Load/Store in Test Program

**Symptom:** Trap with `mcause = 0x6`, `mtval = 0x1001`

**Root Cause:**

* Test program tried to load from unaligned memory address.

**Fix:**

* Rewrite test program to use word-aligned memory accesses.

---

## Issue 4 – rs2 Forwarding Failure

**Symptom:** rs2 value always read as zero.

**Root Cause:**

* ID stage did not validate rs2 for Zf-type instructions.

**Fix:**

* Extend rs2 validity LUT to include: store, branch, R-type, and F-type.
* This enables correct forwarding for rs2.

---

## Issue 5 – MEM Fails to Forward FPU Result

**Symptom:** FPU result lost at WB stage.

**Root Cause:**

* MEM arbitrates outputs to WB; FPU path not considered.

**Fix:**

* Add flag in EX stage to indicate FPU output.
* Use this flag in MEM stage to forward correct result to WB.

---

## Issue 6 – FCSR RAW / WAW Hazards

**Symptom:** Incorrect `fflags` observed during CSR operations.

**Root Cause:**

* **WAW:** When a CSR write follows an FPU operation, the CSR write value may be incorrect.
* **RAW:** When CSR read follows FPU, it may read incorrect in-flight value.

**Fix:**

* Ensure FPU exception handling lasts only one cycle.
* Update CSR module to prevent reading stale or in-flight `fflags`.

---
