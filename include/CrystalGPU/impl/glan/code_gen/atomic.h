#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_ATOMIC_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_ATOMIC_H_

#include <string>

#include "type.h"
#include "tuple.h"

namespace crystal::gpu::impl::glan::code_gen {

using std::string;

class ATOMIC {
 public:
  const string USAGE_;
  TYPE TYPE_;
  ATOMIC(const string& USAGE, TYPE T) : USAGE_(USAGE), TYPE_(T) {
  }
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif