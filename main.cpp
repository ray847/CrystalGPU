#include <iostream>

#include <CrystalGPU/impl/glan/glan.h>

int main() {
  /* Create procedure. */
  using namespace crystal::gpu::impl::glan;

  Proc proc;
  ProxyBind(proc);

  Proxy<I32,
        Storage::LOCAL,
        Policy<DefLoc::Local, "var", true, false, false, false>>
      ivar;
  /* Same type. */
  Proxy<I32,
        Storage::LOCAL,
        Policy<DefLoc::Local, "var", true, false, false, false>>
      ivar1 = ivar;
  ivar1 = ivar;
  /* Different type. */
  Proxy<I32,
        Storage::LOCAL,
        Policy<DefLoc::Local, "var", false, false, false, false>>
      ivar2 = ivar;

  ProxyUnbind();
  
  std::cout << static_cast<string>(proc) << std::endl;

  return 0;
}