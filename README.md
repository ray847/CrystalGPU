# CrystalGPU

GPU programming module for the crystal project.

## QuickStart

> **TODO:** This example is directly copied from main. Write a decent example after all basic features are implemented.
```c++
#include <iostream>
#include <span>
#include <string>
#include <fstream>

#include <CrystalGPU/impl/glan/glan.h>
#include <CrystalGPU/impl/device.h>

int main() {

  using namespace crystal::gpu::impl::glan;
  PROCEDURE rad2deg;
  F32<ARG> A;
  F32<RET> B;
  B = A * 180.0_F32 / 3.1415926535_F32;
  string code = rad2deg[1, 1, 1];
  std::ofstream os("proc.wgsl");
  os << code;
  os.close();

  namespace gpu = crystal::gpu::impl;

  auto res = gpu::context.Init();
  if (!res) {
    std::cout << res.error() << std::endl;
    return 0;
  }
  gpu::Device device{};

  float arg = 3.1415926 / 2, ret;
  {
    auto arg_raw = std::as_bytes(std::span{&arg, 1});
    auto ret_raw = std::as_writable_bytes(std::span{&ret, 1});
    auto res = device.Run(code, arg_raw, ret_raw);
    if (!res) {
      std::cout << res.error() << std::endl;
      return 0;
    }
    std::cout << ret << std::endl;
  }

  return 0;
}
```
