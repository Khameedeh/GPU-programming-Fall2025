
# Profiling Tools
**Author**: Raha Rahmanian  
**Date**: October 2025
## Introduction

Profiling tools are essential utilities that analyze program execution to identify performance bottlenecks and optimize code efficiency. Among these, **perf** leverages hardware performance counters for low-level, system-wide analysis with minimal overhead, capturing detailed metrics like CPU cycles and cache behavior. In contrast, **gprof** uses compile-time instrumentation to generate function-level timing reports and call graphs, making it ideal for understanding time distribution across functions and their relationships. While perf excels at hardware-aware profiling, gprof offers straightforward insights into program structure and execution flow.

## Getting Started

This document provides instructions for profiling C programs using perf and gprof. It includes installation steps, compilation instructions, and commands to generate profiling outputs.

There is a matrix_multiplication.c file in this folder, you can use this file to test the profiling tools.
- **Note**: These commands may not work on WSL2. If you are using WSL2, research alternative methods.

## Installation

Update package lists (recommended if you have freshly installed Linux):

```bash
  sudo apt update
```
Install perf:

```bash
  sudo apt install linux-tools-common linux-tools-generic linux-tools-$(uname -r)
```

Install gprof:

```bash
  sudo apt install binutils
```

Check installation:
```bash
  perf --version
  gprof --version
```


## Compilation and Profiling

### Using gprof

Compile with profiling enabled:

```bash
  gcc -pg matrix_multiplication.c -o matrix_multiplication_gprof
```
Run the program (creates gmon.out):

```bash
  ./matrix_multiplication_gprof
```
You will be expecting an output like this:
```bash
  Mode: CPU-bound (random matrices, N=1000)
  Result[0][0] = 20186 (this number may be different)
```



Generate the report:
```bash
  gprof matrix_multiplication_gprof gmon.out > analysis_gprof.txt
```



### Using perf

Compile with debug symbols and no optimization:

```bash
  gcc -g -O0 matrix_multiplication.c -o matrix_multiplication_perf
```
Run the program with perf recording:

```bash
  sudo perf record -F 99 -g -- ./matrix_multiplication_perf
```
You will be expecting an output like this:
```bash
  Mode: CPU-bound (random matrices, N=1000)
  Result[0][0] = 20186 (this number may be different)
```


Generate the report:
```bash
  sudo perf report --stdio > perf_report.txt
```
- **Documentation Note**: In your assignments document any flags used. If you change them, explain why.



