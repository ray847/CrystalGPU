#include <iostream>
#include <string>

#include <CrystalGPU/glan/glan.h>

int main() {

  using std::string;
  using namespace crystal::gpu::glan;

  /* Create procedure. */
  Proc proc;
  BindProc(proc);

  Var<I32> ivar;
  RO<I32> iro = ivar;
  Const<I32> iconst;
  Const<I32> iconst1 = iconst;
  ivar = iro;

  UnbindProc();
  
  std::cout << static_cast<string>(proc) << std::endl;

  return 0;
}