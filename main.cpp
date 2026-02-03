#include <iostream>

#include <CrystalGPU/impl/glan/glan.h>

int main() {
  /* Create procedure. */
  using namespace crystal::gpu::impl::glan;
  {
    PROCEDURE proc = BEGIN();

    VARIABLE<TUPLE<INT32>> BEFORE;

    FUNCTION<INT32> fn = BEGIN();
      VARIABLE<VECTOR<3, INT32>> A;
    END();

    VARIABLE<INT32> AFTER = fn();

    END();

    auto code = proc.TO_CODE();
    if (code) { std::cout << *code << std::endl; }
  }

  {
    PROCEDURE proc = BEGIN();
    VARIABLE<TUPLE<INT32>> D;
    VARIABLE<TUPLE<INT32, INT32>> A;
    END();

    auto code = proc.TO_CODE();
    if (code) { std::cout << *code << std::endl; }
  }

  return 0;
}