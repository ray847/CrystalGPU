#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_IMMEDIATE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_IMMEDIATE_H_

#include <format>

#include "atomic.h"
#include "type.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::format;

class IMMEDIATE : public ATOMIC {
 public:
  IMMEDIATE(string VAL, TYPE T) : ATOMIC(VAL, T) {
  }
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif