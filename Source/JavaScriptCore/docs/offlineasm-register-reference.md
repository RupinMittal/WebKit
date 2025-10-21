# JavaScriptCore Offlineasm Register Reference

**NOTE: This document was largely AI-generated based on offlineasm/\*.rb sources and llint/\*.asm files. Exercise healthy skepticism.**

This document describes the correspondence between offlineasm logical registers, interpreter-specific aliases (LowLevelInterpreter and InPlaceInterpreter), and physical platform registers.

## Overview

JavaScriptCore's offline assembler (offlineasm) uses portable logical register names that are mapped to platform-specific physical registers during code generation. The LowLevelInterpreter (LLInt) and InPlaceInterpreter (IPInt) define their own register aliases for clarity and portability.

## General Purpose Registers (GPRs)

### Temporary Registers

| Offlineasm | LLInt Alias | IPInt Alias | ARM64/ARM64E | X86_64 | Purpose |
|------------|-------------|-------------|--------------|--------|---------|
| t0 | a0, wa0, r0 | wa0 | x0/w0 | rax | Argument 0 / Return value / Temp |
| t1 | a1, wa1, r1 | wa1 | x1/w1 | rsi | Argument 1 / Temp |
| t2 | a2, wa2 | wa2 | x2/w2 | rdx | Argument 2 / Temp |
| t3 | a3, wa3 | wa3 | x3/w3 | rcx | Argument 3 / Temp |
| t4 | a4, wa4 | wa4 | x4/w4 | r8 | Argument 4 / Temp |
| t5 | a5, wa5 | wa5, PL (X86_64) | x5/w5 | r10 | Argument 5 / Temp / Pointer to Locals |
| t6 | a6, wa6 | wa6, PL (ARM64) | x6/w6 | rdi | Argument 6 / Temp / Pointer to Locals |
| t7 | a7, wa7 | wa7, PL (ARMv7) | x7/w7 | r9 | Argument 7 / Temp / Pointer to Locals |
| t8 | - | - | x8/w8 | - | Temp |
| t9 | ws0 | ws0, sc0 | x9/w9 | - | Wasm scratch 0 / Safe call 0 |
| t10 | ws1 | ws1, sc1 | x10/w10 | - | Wasm scratch 1 / Safe call 1 |
| t11 | ws2 | ws2 | x11/w11 | - | Wasm scratch 2 |
| t12 | ws3 | ws3 | x12/w12 | - | Wasm scratch 3 |

### Special Purpose Registers

| Offlineasm | LLInt Alias | IPInt Alias | ARM64/ARM64E | X86_64 | Purpose |
|------------|-------------|-------------|--------------|--------|---------|
| cfr | - | - | x29 | rbp | Call Frame Register |
| sp | - | - | sp | rsp | Stack Pointer |
| lr | - | - | x30 (lr) | - | Link Register (ARM64 only) |
| pc | - | - | - | - | Program Counter (logical) |

### Callee-Save Registers

| Offlineasm | LLInt Alias | IPInt Alias | ARM64/ARM64E | X86_64 | Purpose |
|------------|-------------|-------------|--------------|--------|---------|
| csr0 | - | WI (wasmInstance) | x19 | rbx | Callee-save / wasmInstance (IPInt) |
| csr1 | - | MC (X86_64) | x20 | r12 | Callee-save / Metadata Counter (IPInt X86_64) |
| csr2 | - | PC (X86_64) | x21 | r13 | Callee-save / Program Counter (IPInt X86_64) |
| csr3 | - | MB (memoryBase) | x22 | r14 | Callee-save / Memory Base (IPInt) |
| csr4 | - | BC (boundsCheckingSize) | x23 | r15 | Callee-save / Bounds Check (IPInt) |
| csr5 | - | - | x24 | - | Callee-save (ARM64 only) |
| csr6 | metadataTable | MC (ARM64) | x25 | - | Metadata Table (LLInt) / Metadata Counter (IPInt ARM64) |
| csr7 | PB | PC (ARM64) | x26 | - | PB register (LLInt) / Program Counter (IPInt ARM64) |
| csr8 | numberTag | - | x27 | - | Number tag (LLInt ARM64) |
| csr9 | notCellMask | sc2 (RISCV64) | x28 | - | Not cell mask (LLInt ARM64) / Safe call 2 (IPInt RISCV64) |
| csr10 | - | PL (RISCV64), sc3 (RISCV64) | - | - | Pointer to Locals / Safe call 3 (IPInt RISCV64) |

## Floating Point Registers (FPRs)

### Temporary FP Registers

| Offlineasm | LLInt Alias | IPInt Alias | ARM64/ARM64E | X86_64 | Purpose |
|------------|-------------|-------------|--------------|--------|---------|
| ft0 | fa0, wfa0, fr | - | q0/d0/s0 | xmm0 | FP argument 0 / return / temp |
| ft1 | fa1, wfa1 | - | q1/d1/s1 | xmm1 | FP argument 1 / temp |
| ft2 | fa2, wfa2 | - | q2/d2/s2 | xmm2 | FP argument 2 / temp |
| ft3 | fa3, wfa3 | - | q3/d3/s3 | xmm3 | FP argument 3 / temp |
| ft4 | wfa4 | - | q4/d4/s4 | xmm4 | Wasm FP temp |
| ft5 | wfa5 | - | q5/d5/s5 | xmm5 | Wasm FP temp |
| ft6 | wfa6 | - | q6/d6/s6 | xmm6 | Wasm FP temp |
| ft7 | wfa7 | - | q7/d7/s7 | xmm7 | Wasm FP temp / scratch |

### Callee-Save FP Registers

| Offlineasm | LLInt Alias | IPInt Alias | ARM64/ARM64E | X86_64 | Purpose |
|------------|-------------|-------------|--------------|--------|---------|
| csfr0 | - | - | q8/d8 | - | Callee-save FP (ARM64 only) |
| csfr1 | - | - | q9/d9 | - | Callee-save FP (ARM64 only) |
| csfr2 | - | - | q10/d10 | - | Callee-save FP (ARM64 only) |
| csfr3 | - | - | q11/d11 | - | Callee-save FP (ARM64 only) |
| csfr4 | - | - | q12/d12 | - | Callee-save FP (ARM64 only) |
| csfr5 | - | - | q13/d13 | - | Callee-save FP (ARM64 only) |
| csfr6 | - | - | q14/d14 | - | Callee-save FP (ARM64 only) |
| csfr7 | - | - | q15/d15 | - | Callee-save FP (ARM64 only) |
| csfr8-11 | - | - | - | - | (Reserved) |

## Vector Registers

### SIMD/Vector Registers

| Offlineasm | LLInt Alias | IPInt Alias | ARM64/ARM64E | X86_64 | Purpose |
|------------|-------------|-------------|--------------|--------|---------|
| v0 | - | - | v16 (q16) | xmm0 | Vector register 0 |
| v0_b | - | - | v16.b | xmm0 (byte) | Vector 0, byte elements |
| v0_h | - | - | v16.h | xmm0 (half) | Vector 0, halfword elements |
| v0_i | - | - | v16.s | xmm0 (int) | Vector 0, word elements |
| v0_q | - | - | v16.d | xmm0 (quad) | Vector 0, doubleword elements |
| v1 | - | - | v17 (q17) | xmm1 | Vector register 1 |
| v1_b | - | - | v17.b | xmm1 (byte) | Vector 1, byte elements |
| v1_h | - | - | v17.h | xmm1 (half) | Vector 1, halfword elements |
| v1_i | - | - | v17.s | xmm1 (int) | Vector 1, word elements |
| v1_q | - | - | v17.d | xmm1 (quad) | Vector 1, doubleword elements |
| v2 | - | - | v18 (q18) | xmm2 | Vector register 2 |
| v2_b | - | - | v18.b | xmm2 (byte) | Vector 2, byte elements |
| v2_h | - | - | v18.h | xmm2 (half) | Vector 2, halfword elements |
| v2_i | - | - | v18.s | xmm2 (int) | Vector 2, word elements |
| v2_q | - | - | v18.d | xmm2 (quad) | Vector 2, doubleword elements |
| v3 | - | - | v19 (q19) | xmm3 | Vector register 3 |
| v3_b | - | - | v19.b | xmm3 (byte) | Vector 3, byte elements |
| v3_h | - | - | v19.h | xmm3 (half) | Vector 3, halfword elements |
| v3_i | - | - | v19.s | xmm3 (int) | Vector 3, word elements |
| v3_q | - | - | v19.d | xmm3 (quad) | Vector 3, doubleword elements |
| v4 | - | - | v20 (q20) | xmm4 | Vector register 4 |
| v4_b | - | - | v20.b | xmm4 (byte) | Vector 4, byte elements |
| v4_h | - | - | v20.h | xmm4 (half) | Vector 4, halfword elements |
| v4_i | - | - | v20.s | xmm4 (int) | Vector 4, word elements |
| v4_q | - | - | v20.d | xmm4 (quad) | Vector 4, doubleword elements |
| v5 | - | - | v21 (q21) | xmm5 | Vector register 5 |
| v5_b | - | - | v21.b | xmm5 (byte) | Vector 5, byte elements |
| v5_h | - | - | v21.h | xmm5 (half) | Vector 5, halfword elements |
| v5_i | - | - | v21.s | xmm5 (int) | Vector 5, word elements |
| v5_q | - | - | v21.d | xmm5 (quad) | Vector 5, doubleword elements |
| v6 | - | - | v22 (q22) | xmm6 | Vector register 6 |
| v6_b | - | - | v22.b | xmm6 (byte) | Vector 6, byte elements |
| v6_h | - | - | v22.h | xmm6 (half) | Vector 6, halfword elements |
| v6_i | - | - | v22.s | xmm6 (int) | Vector 6, word elements |
| v6_q | - | - | v22.d | xmm6 (quad) | Vector 6, doubleword elements |
| v7 | - | - | v23 (q23) | xmm7 | Vector register 7 |
| v7_b | - | - | v23.b | xmm7 (byte) | Vector 7, byte elements |
| v7_h | - | - | v23.h | xmm7 (half) | Vector 7, halfword elements |
| v7_i | - | - | v23.s | xmm7 (int) | Vector 7, word elements |
| v7_q | - | - | v23.d | xmm7 (quad) | Vector 7, doubleword elements |

## Platform-Specific Notes

### ARM64/ARM64E

- **Register widths**:
  - `xN` = 64-bit GPR
  - `wN` = 32-bit GPR (low half of xN)
  - `qN` = 128-bit FPR/vector
  - `dN` = 64-bit FPR (double precision)
  - `sN` = 32-bit FPR (single precision)
  - `vN` = vector register with element type qualifier

- **Scratch registers**: x13, x16, x17 (not exposed in offlineasm)
- **FPR scratch**: q31
- **Vector registers**: Shared with FPRs (q0-q15 overlap with ft0-csfr7, q16-q23 used for v0-v7)

### X86_64

- **Register widths**:
  - 64-bit: rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8-r15
  - 32-bit: eax, ebx, ecx, edx, esi, edi, ebp, esp, r8d-r15d
  - 16-bit: ax, bx, cx, dx, si, di, bp, sp, r8w-r15w
  - 8-bit: al, bl, cl, dl, sil, dil, bpl, spl, r8b-r15b

- **Scratch register**: r11 (not exposed in offlineasm)
- **FPR scratch**: xmm7
- **Limited callee-save**: Only rbx, r12-r15 are callee-save

## Register Conventions Summary

### LowLevelInterpreter (LLInt) Conventions

| Purpose | Offlineasm | ARM64/ARM64E Physical | X86_64 Physical | Description |
|---------|------------|----------------------|-----------------|-------------|
| **PC** (Program Counter) | csr7 | x26 | r13 | Bytecode program counter |
| **PB** (Program Base) | csr7 | x26 | r13 | Same as PC in LLInt |
| **Metadata Table** | csr6 | x25 | r12 | Metadata pointer |
| **Number Tag** | csr8 | x27 | - | Number type tag (ARM64 only) |
| **Not Cell Mask** | csr9 | x28 | - | Cell type mask (ARM64 only) |
| **Tag Type Number** | csr3 | x22 | r14 | Type number for tagging |
| **Tag Mask** | csr4 | x23 | r15 | Mask for type tags |
| **Wasm Instance** | csr0 | x19 | rbx | WebAssembly instance pointer |
| **Arguments** | t0-t7 | x0-x7 | rax,rsi,rdx,rcx,r8,r10,rdi,r9 | Function argument registers |
| **FP Arguments** | ft0-ft3 | q0-q3 | xmm0-xmm3 | Floating-point arguments |
| **Wasm Scratch** | t9-t12 (ws0-ws3) | x9-x12 | - | Wasm temporary registers |

### InPlaceInterpreter (IPInt) Conventions

IPInt is the WebAssembly in-place interpreter with its own specialized register assignments:

| Purpose | Offlineasm | ARM64/ARM64E Physical | X86_64 Physical | ARMv7 Physical | RISCV64 Physical | Description |
|---------|------------|----------------------|-----------------|----------------|------------------|-------------|
| **PC** (Program Counter) | csr7 | x26 | csr2 (r13) | csr1 | csr7 | IPInt bytecode position |
| **MC** (Metadata Counter) | csr6 | x25 | csr1 (r12) | t6 | csr6 | Metadata pointer |
| **PL** (Pointer to Locals) | t6 | x6 | t5 (r10) | t7 | csr10 | Address of local 0 |
| **WI** (Wasm Instance) | csr0 | x19 | rbx | csr0 | csr0 | JSWebAssemblyInstance pointer |
| **MB** (Memory Base) | csr3 | x22 | r14 | t2 (ARMv7) | csr3 | Wasm memory base address |
| **BC** (Bounds Check) | csr4 | x23 | r15 | t3 (ARMv7) | csr4 | Wasm memory bounds size |
| **sc0** (safe for call) | ws0 (t9) | x9 | t9 | t4 | ws0 (t9) | Safe call register 0 |
| **sc1** (safe for call) | ws1 (t10) | x10 | t10 | t5 | ws1 (t10) | Safe call register 1 |
| **sc2** (safe for call) | ws2 (t11) | x11 | csr3 (r14) | csr0 | csr9 | Safe call register 2 |
| **sc3** (safe for call) | ws3 (t12) | x12 | csr4 (r15) | t7 | csr10 | Safe call register 3 |

#### IPInt Special Notes

- **PC (Program Counter)**: Records interpreter position in Wasm bytecode
- **MC (Metadata Counter)**: Tracks corresponding position in generated metadata
- **PL (Pointer to Locals)**: Fast access to local variables (points to local 0)
- **WI (Wasm Instance)**: Current JSWebAssemblyInstance object (callee-save)
- **MB (Memory Base)**: Current Wasm memory base address (callee-save)
- **BC (Bounds Check)**: Size of Wasm memory region for bounds checking (callee-save)
- **Safe call registers (sc0-sc3)**: Guaranteed not to overlap with argument registers, safe across calls

#### Platform-Specific IPInt Notes

**ARM64/ARM64E:**
- Uses ldp/stp instructions for efficient register pair operations
- Memory registers (MB, BC) loaded via `loadpairq` from instance

**X86_64:**
- PC maps to csr2 instead of csr7 (different from LLInt)
- MC maps to csr1 instead of csr6 (different from LLInt)
- sc2 and sc3 reuse csr3/csr4 (MB/BC) as they're already in use
- Fewer argument registers (6 vs 8 on ARM64)

**ARMv7:**
- MB and BC use temporary registers (t2, t3) instead of callee-save
- More limited register set requires creative reuse

**RISCV64:**
- Similar to ARM64 in register allocation
- PL uses csr10 instead of t6

## Source Files

- **Register definitions**: `Source/JavaScriptCore/offlineasm/registers.rb`
- **ARM64 mappings**: `Source/JavaScriptCore/offlineasm/arm64.rb`
- **X86_64 mappings**: `Source/JavaScriptCore/offlineasm/x86.rb`
- **LLInt code**: `Source/JavaScriptCore/llint/LowLevelInterpreter.asm`
- **IPInt code**: `Source/JavaScriptCore/llint/InPlaceInterpreter.asm`
