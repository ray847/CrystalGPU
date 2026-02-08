#include <iostream>
#include <string>
#include "CrystalGPU/impl/glan/expression.h"

#include <CrystalGPU/glan/glan.h>

int main() {

  using std::string;
  using namespace crystal::gpu::glan;

  /* Create procedure. */
  Proc proc;
  BindProc(proc);

  Var<Vec<2, I32>> vec;
  Var<Vec<2, I32>> vec1 = vec + vec;
  vec1[0, 1] = vec[0, 1];

  UnbindProc();
  
  std::cout << static_cast<string>(proc) << std::endl;

  return 0;
}