# Document

`CrystalGPU` is a library for running computational tasks with **hardware acceleration** on the gpu.

## Interface Design

1. This library provides ways to write gpu code inside regular `C++` code.
2. The gpu code written are organized in the units of `Procedure`s: Every procedure is a task that can be run on the gpu while producing some output based on the input.
3. `Procedure`s run asynchronously (both for the cpu preperation part and gpu execution part) to the caller thread.
4. `Procedure`s have the following options for input information: **SSBO**, **uniform**, and the following options for output: **SSBO**.

```c++
/* Usage Concept */
struct Constants {
  int a;
  float b;
};
struct Input {
  array<int, 64> x;
  array<float, 64> y, z;
};
using Output = array<float, 64>;

Constants constants;
Input input;
Output output;
void* window_handle;

/* CrystalGPU magic begins. */
/**
 * Retrieve GPU device.
 */
assert(glan::DeviceList().size() > 0);
glan::Device device = glan::DeviceList()[0];
/**
 * Describe GPU Procedure.
 */
glan::RO<glan::I32, INPUT_UNIFORM> A{constants.a};
glan::RO<glan::F32, INPUT_UNIFORM> B{constants.b};
glan::RO<glan::ARR<I32, 64>, INPUT_SSBO> X{input.x};
glan::RO<glan::ARR<F32, 64>, INPUT_SSBO> Y{input.y};
glan::RO<glan::ARR<F32, 64>, INPUT_SSBO> Z{input.z};
glan::VAR<glan::ARR<F32, 64>, OUTPUT_SSBO> OUT{output};
glan::TEXTURE<INTERSTAGE_SSBO> TEXTURE{64, 32};

glan::COMPUTE_PROCEDURE COMP_PROC = glan::BEGIN();
  glan::FUNCTION<glan::F32> COMP = glan::BEGIN();
    glan::VAR<glan::I32, PARAM> x;
    glan::VAR<glan::F32, PARAM> y, z;
    glan::IF(x > 32)::BEGIN();
      glan::RETURN(y * 0.5_F32);
    glan::END();
    glan::ELSE::BEGIN();
      glan::RETURN(y + z);
    glan::END();
  glan::END();

  glan::THREAD_IDX TI;
  OUT[TI] = COMP(X[TI], Y[TI], Z[TI]);
  glan::VAR<glan::I32> I = 0;
  glan::WHILE(I < 32_I32)::BEGIN();
    TEXTURE[IT][I] = OUT[TI];
    I++;
  glan::END();
glan::END();

glan::RENDER_PROCEDURE DISP_PROC = glan::BEGIN();
  SURFACE SURF(window_handle);
  SURF.CLEAR();
  glan::PRIMITIVE::TRIANGLE TRI{
    {0, 0},
    {1, 0},
    {0, 1},
    SURF,
  };
  TRI.FILL(TEXTURE);
glan::END();
/**
 * Run procedure.
 */
auto comp_flag = device.Run(COMP_PROC[1, 2, 4], 8, 16, 32);
device.Connectto(window_handle);
device.ClearSurface();
auto disp_flag = device.Run(DISP_PROC);
device.SwapSurface();
```

## Implementation Design

### The cost of abstractions & integration
The backend of such interface is similar to a JIT-compilation system: GPU code (shader code in this case) gets compiled and integrated into a pipeline and then gets submitted to the gpu together with the inputs for execution.
Reuse of the same `Procedure` should come at a significantly lower latency since the code has already been compiled and the pipelines are already created.

This design choice is made for the following reasons:
1. GPU code are repeatable by the nature of designated tasks so prioritizing the reusability of code is very important.
2. For a standard C++ compiler, the gpu backend information is not available so ahead-of-time compilation is not an option.
3. To avoid complex and **fragile** interactions between gpu bind groups, gpu piplines, and shader code compatibility, the library should hide this implementation detail from the interface making the code highly coupled to native C++ code execution.