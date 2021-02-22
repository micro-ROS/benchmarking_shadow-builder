Shadow Builder
===========================

### Concept

The shadow builder (SB) is a tool that can transparently instrument a code to perform
benchmarking. The tool outputs the code with benchmark insertions, which is ready to be compiled as a regular code. 

The following steps describe the shadow builder process flow:

 * get the configuration file from the user (Benchmarking Configuration File),
 * get source files for benchmarking,
 * execute Trace Framework Abstraction,
 * parse the source files for code injection,
 * compile the targeted binaries.

Currently,the LLVM and [Clang](https://releases.llvm.org/10.0.0/tools/clang/docs/index.html) are used to parse and adapt the code.

SB is as transparent as possible for the user. If the benchmarking is not activated, then it is bypassed.  

### Inputs

SB receives two inputs to work with:
 * Benchmarking Configuration File,
 * source code to benchmark.

Details of the configuration can be found here [Configuration Datails](./config/readme.md)
In short, the configuration describes:

 * what sources are to be benchmarked,
 * benchmark tests' target platforms,
 * what the benchmark types are.

### Parser

Once the input is received, SB parses the configuration file and gets:

 * the benchmarking to be performed,
 * the target platforms.

At first, SB checks consistency within the configuration file and capabilities 
of plugins registered in the TFA module.

### TFA Execution

Once the parsing step and checking against the TFA module capabilities are fullfilled, SB will be in charge of generating the source code with benchmark insertions based on the provided configuration. This step is achieved in cooperation with the TFA module, detailes of what can be found here [TFA core](./tfa_core/radme.md). At the end of this step, the new forged source code is ready for compilation. In addition to the patched source code, the TFA generates scripts that will provide the benchmarks.

### Compilation

The compilation is performed for every benchmark type and
target platform, so one or more binaries per benchmark session can be generated.

## Tree view

shadow-builder \
	├── inc\shadow-builder  --> Header files \
	└── src 	--> Source files
