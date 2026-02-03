#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_READONLY_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_READONLY_H_

#include <format>

#include "atomic.h"
#include "tag.h"
#include "type.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;

class READONLY : public ATOMIC {
 public:
  READONLY(TYPE T) : ATOMIC(format("READONLY_{}", TAG_.GEN()), T) {
  }

 private:
  inline static TAG TAG_{};
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif