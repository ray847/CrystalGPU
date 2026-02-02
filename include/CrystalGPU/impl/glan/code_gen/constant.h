#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_CONSTANT_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_CONSTANT_H_

#include <format>

#include "atomic.h"
#include "type.h"
#include "tag.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;

class CONSTANT : public ATOMIC {
  public:
  CONSTANT(TYPE T) : ATOMIC(format("CONSTANT_{}", TAG_.GEN()), T) {
  }

 private:
  inline static TAG TAG_{};
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif