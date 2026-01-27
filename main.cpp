#include <cstdio>
#include <iostream>
#include <span>
#include <string>
#include <fstream>
#include "include/CrystalGPU/impl/glan/function.h"
#include "include/CrystalGPU/impl/glan/immediate.h"

#include <CrystalGPU/impl/glan/glan.h>
#include <CrystalGPU/impl/device.h>

int main() {

  /* Create procedure. */
  using namespace crystal::gpu::impl::glan;
  PROCEDURE proc;
  VEC<2, F32, ARG> A;
  VEC<2, F32, RET> B;
  B = B + A;
  B = B + A;
  string code = proc[1, 1, 1];
  std::ofstream os("proc.wgsl");
  os << code;
  os.close();

  /* Run procedure. */
  namespace gpu = crystal::gpu::impl;
  auto res = gpu::context.Init();
  if (!res) {
    std::cout << res.error() << std::endl;
    return 0;
  }
  gpu::Device device{};

  std::array<float, 2> arg{1, 2}, ret;
  {
    auto arg_raw = std::as_bytes(std::span{&arg, 1});
    auto ret_raw = std::as_writable_bytes(std::span{&ret, 1});
    auto res = device.Run(code, arg_raw, ret_raw);
    if (!res) {
      std::cout << res.error() << std::endl;
      return 0;
    }
    std::cout << ret[0] << ", " << ret[1] << std::endl;
  }

  return 0;
}