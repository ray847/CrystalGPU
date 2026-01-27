# Document

`CrystalGPU` is a library for running computational tasks with **hardware acceleration** on the gpu.

## Interface Design

1. This library provides ways to write gpu code inside regular `C++` code.
2. The gpu code written are organized in the units of `Procedure`s: Every procedure is a task that can be run on the gpu while producing some output based on the input.
3. `Procedure`s run asynchronously (both for the cpu preperation part and gpu execution part) to the caller thread.


## Implementation Design

### The cost of abstractions & integration
The backend of such interface is similar to a JIT-compilation system: GPU code (shader code in this case) gets compiled and integrated into a pipeline and then gets submitted to the gpu together with the inputs for execution.
Reuse of the same `Procedure` should come at a significantly lower latency since the code has already been compiled and the pipelines are already created.

This design choice is made for the following reasons:
1. GPU code are repeatable by the nature of designated tasks so prioritizing the reusability of code is very important.
2. For a standard C++ compiler, the gpu backend information is not available so ahead-of-time compilation is not an option.
3. To avoid complex and **fragile** interactions between gpu bind groups, gpu piplines, and shader code compatibility, the library should hide this implementation detail from the interface making the code highly coupled to native C++ code execution.