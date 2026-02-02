#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_VARIABLE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_VARIABLE_H_

#include <string>
#include <format>

#include "atomic.h"
#include "type.h"
#include "tag.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::string, std::format;

/**
 * Variable atomic with automated name generation.
 */
class VARIABLE : public ATOMIC {
 public:
  VARIABLE(TYPE T) : ATOMIC(format("VARIABLE_{}", TAG_.GEN()), T) {
  }

 private:
  inline static TAG TAG_{};
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif