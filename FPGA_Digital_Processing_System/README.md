# ⚡ Digital Convolution Processing System — FPGA & ASIC Design

A modular digital convolution-style processing system designed in **Verilog HDL** and verified through simulation and MATLAB.

The project demonstrates **RTL design, memory interfacing, sequential control logic, signed arithmetic datapaths, functional verification, and ASIC physical design**. The verified RTL was also taken through the **OpenLane RTL-to-GDS flow**, including synthesis, floorplanning, placement, routing, and final layout generation.

---

## Table of Contents

- [Project Overview](#project-overview)
- [System Architecture](#system-architecture)
- [RTL Modules](#rtl-modules)
- [Verilog Implementation](#verilog-implementation)
- [Functional Verification](#functional-verification)
- [MATLAB Verification](#matlab-verification)
- [ASIC Implementation](#asic-implementation)
- [Skills Demonstrated](#skills-demonstrated)
- [Future Improvements](#future-improvements)

---

## Project Overview

The goal of this project was to design and implement a modular digital processing architecture capable of performing convolution-style arithmetic operations in hardware.

The system uses separate memory modules to store **16-bit input data and coefficient values**. A controller sequentially accesses the memory locations and supplies the data to a signed multiplier.

The resulting arithmetic data can then be processed by additional datapath modules for output selection and accumulation.

The design was developed using **Verilog HDL** and functionally verified using Verilog testbenches and waveform analysis.

A MATLAB convolution implementation was also used as a reference to compare the hardware-generated results.

Finally, the RTL design was taken through the **OpenLane ASIC design flow** to generate a physical layout.

---

## System Architecture

```text
                 ┌──────────────┐
                 │   Memory A   │
                 │  Input Data  │
                 │  32 × 16-bit │
                 └──────┬───────┘
                        │
                        │ A
                        ▼
                 ┌──────────────┐
                 │  Multiplier  │
                 │ 16-bit ×     │
                 │ 16-bit       │
                 └──────┬───────┘
                        │
                        │ 32-bit Product
                        ▼
                 ┌──────────────┐
                 │     RCA2     │
                 │    Adder     │
                 └──────┬───────┘
                        │
                        ▼
                 ┌──────────────┐
                 │     MUX      │
                 │ Output Logic │
                 └──────┬───────┘
                        │
                        ▼
                      Output


                 ┌──────────────┐
                 │   Memory B   │
                 │ Coefficients │
                 │  32 × 16-bit │
                 └──────┬───────┘
                        │
                        └──────► Multiplier


                 ┌──────────────┐
                 │   Memory C   │
                 │ 32-bit Data  │
                 │   Storage    │
                 └──────────────┘

        ┌──────────────────────────────┐
        │          Top Module          │
        │                              │
        │ Address Generation           │
        │ Memory Control               │
        │ Sequential Processing        │
        │ Output Synchronization       │
        └──────────────────────────────┘
```

---

# RTL Modules

The design is divided into independent RTL modules to improve readability, verification, debugging, and scalability.

---

## Memory A

`MemoryA` stores the primary input sequence used by the processing system.

The module contains **32 memory locations**, with each location storing a **16-bit value**.

```text
Memory Depth : 32
Data Width   : 16 bits
Address Width: 5 bits
```

The module supports synchronous read and write operations controlled by the clock and write-enable signals.

During reset, the memory locations are initialized to zero to provide predictable startup behavior.

### Memory A RTL

<!-- Add Memory A code/schematic image here -->

---

## Memory B

`MemoryB` stores the secondary input values or coefficient data used during multiplication.

Like Memory A, the module contains:

```text
32 × 16-bit memory
```

A **5-bit address** selects one of the 32 memory locations.

Stored values are provided through the `Bout` output signal during read operations.

### Memory B RTL

<!-- Add Memory B code/schematic image here -->

### Memory B Verification

The `MemoryB_tb` testbench verifies that values can be correctly written to and retrieved from different memory addresses.

The simulation checks:

- Address selection
- Write operations
- Read operations
- Clock synchronization
- Data integrity

---

## Memory C

`MemoryC` provides storage for larger arithmetic results.

Unlike Memory A and Memory B, Memory C uses a **32-bit data width** to accommodate results generated by arithmetic operations.

```text
Memory Depth : 32
Data Width   : 32 bits
Address Width: 5 bits
```

The module supports synchronous memory operations and asynchronous reset behavior.

Memory C also provides a path for future expansion of the architecture to store accumulated convolution outputs or intermediate processing values.

### Memory C RTL

<!-- Add Memory C code/schematic image here -->

---

## Multiplier

The `Multiplier` module is the primary arithmetic unit within the datapath.

It performs **signed 16-bit multiplication** between values provided by Memory A and Memory B.

```text
        A[15:0]
           │
           ▼
      ┌──────────┐
      │          │
      │ Signed   │
      │ 16 × 16  │
      │ Multiply │
      │          │
      └────┬─────┘
           │
           ▼
     Product[31:0]
```

The operation is:

```text
Product = A × B
```

Because two signed 16-bit values are multiplied, the result is represented using a **32-bit signed output**.

---

## RCA2 Adder

The `RCA2` module provides an arithmetic addition stage.

It adds the current multiplication product to a previous input value to generate a new output.

Conceptually:

```text
Previous Value ─────┐
                    ▼
                 ┌─────┐
Product ─────────►  +  ├────► Result
                 └─────┘
```

This module provides arithmetic functionality that can be used for accumulation operations in convolution-based processing architectures.

---

## MUX

The `MUX` module controls whether data is allowed to propagate to the output.

When the `valid` signal is asserted, the input value is forwarded to `Bi`.

```text
valid = 1

Input ─────────────► Bi
```

When `valid` is inactive:

```text
valid = 0

Bi = 0
```

This allows the controller to prevent invalid intermediate data from appearing as valid output.

---

## CompareB

The `CompareB` module performs sequential comparison operations using an internal counter.

The module repeatedly compares the input against an incrementing value and generates a processed output.

Once the comparison sequence has completed, the module asserts a:

```verilog
done
```

signal to indicate completion.

---

## Top Module

The **Top Module** coordinates the operation of the complete system.

Its responsibilities include:

- Memory addressing
- Memory read control
- Sequential address generation
- Arithmetic operation coordination
- Multiplier synchronization
- Output generation
- Computation control

Internal control signals such as:

```verilog
computing
wait_read
```

are used to coordinate memory accesses and arithmetic operations across multiple clock cycles.

The controller ensures that memory data is available before arithmetic operations are performed.

---

# Verilog Implementation

Each subsystem was implemented as an independent Verilog module.

The modular architecture separates:

```text
Memory
   │
Control
   │
Arithmetic
   │
Output Logic
```

Memory modules use register arrays indexed by **5-bit address signals**.

Sequential logic is implemented using positive-edge-triggered blocks:

```verilog
always @(posedge clk)
```

while arithmetic datapath operations are implemented using combinational logic.

This modular structure allows individual components to be independently simulated and verified before integration into the Top Module.

---

# Functional Verification

Functional verification was performed using **Verilog testbenches and waveform simulation**.

The verification process tested both individual RTL modules and the integrated processing system.

## Testbench Flow

```text
Initialize Design
       │
       ▼
Load Memory A
       │
       ▼
Load Memory B
       │
       ▼
Assert Start
       │
       ▼
Sequential Memory Access
       │
       ▼
Arithmetic Processing
       │
       ▼
Generate Valid Outputs
       │
       ▼
Compare Results
```

The `TopModule_tb` testbench loads values into Memory A and Memory B before initiating the computation using the `start` signal.

Waveform analysis was used to verify synchronization between:

- Clock signals
- Memory addresses
- Memory outputs
- Control signals
- Arithmetic operations
- Output generation

### Simulation Waveform

<!-- Add waveform image here -->

---

# Convolution Verification

The system was tested using the following input sequence:

```text
x[n] =
[1, 2, 3, 4, 5, 6, 7, 8,
 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24,
 25, 26, 27, 28, 29, 30, 31, 32]
```

with:

```text
h[n] = [2]
```

For this test case, each input sample is multiplied by the coefficient `2`.

The expected result is therefore:

```text
y[n] =
[2, 4, 6, 8, 10, 12, 14, 16,
 18, 20, 22, 24, 26, 28, 30, 32,
 34, 36, 38, 40, 42, 44, 46, 48,
 50, 52, 54, 56, 58, 60, 62, 64]
```

### Verilog Simulation Result

<!-- Add final convolution output image here -->

---

# MATLAB Verification

A MATLAB implementation was used as an independent reference for validating the hardware simulation.

The same input sequences used by the Verilog testbench were processed using MATLAB's convolution operation.

```matlab
x = 1:32;
h = [2];

y = conv(x, h);
```

The MATLAB results were compared against the corresponding outputs produced by the Verilog simulation.

```text
        Verilog RTL
             │
             ▼
       Simulation Output
             │
             │
             ▼
          Compare
             ▲
             │
             │
       MATLAB conv()
             ▲
             │
       Reference Input
```

The corresponding results matched for the tested input sequence, providing an independent verification of the implemented arithmetic behavior.

### MATLAB Results

<!-- Add MATLAB verification image here -->

---

# ASIC Implementation

After functional RTL verification, the design was processed through the **OpenLane RTL-to-GDS flow**.

The ASIC implementation flow included:

```text
Verilog RTL
     │
     ▼
Synthesis
     │
     ▼
Floorplanning
     │
     ▼
Placement
     │
     ▼
Routing
     │
     ▼
Physical Layout
     │
     ▼
GDS
```

## Synthesis

The Verilog RTL was synthesized into a gate-level representation of the design.

<!-- Add synthesis image/report here -->

---

## Floorplanning

Floorplanning established the physical dimensions and initial organization of the design.

<!-- Add floorplan image here -->

---

## Placement

Standard cells generated from synthesis were physically placed within the floorplan.

<!-- Add placement image here -->

---

## Routing

Routing established the physical interconnections between the placed cells.

<!-- Add routing image here -->

---

## Final ASIC Layout

The completed physical design represents the final stage of the OpenLane flow used in this project.

<!-- Add final OpenLane layout image here -->

---

# Skills Demonstrated

This project demonstrates experience with:

- Verilog HDL
- RTL design
- FPGA-oriented digital design
- Modular hardware architecture
- Sequential logic
- Combinational logic
- Memory interfacing
- Signed binary arithmetic
- Hardware multipliers
- Arithmetic datapaths
- Control logic
- Testbench development
- Waveform analysis
- Functional verification
- MATLAB reference-model verification
- RTL synthesis
- ASIC physical design
- OpenLane RTL-to-GDS flow
- Floorplanning
- Placement
- Routing
- GDS layout generation

---

# Future Improvements

Potential extensions to the architecture include:

- Multi-tap convolution kernels
- Complete multiply-accumulate (MAC) processing
- Pipelined arithmetic stages
- Parallel multiplier architectures
- Increased processing throughput
- Parameterized memory depth and data width
- Additional test vectors
- Automated RTL/reference-model comparison
- FPGA resource and timing optimization
- ASIC timing and area optimization
