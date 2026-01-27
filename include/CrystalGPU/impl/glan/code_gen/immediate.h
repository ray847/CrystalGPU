#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_IMMEDIATE_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_IMMEDIATE_H_

#include <string>

#include "expression.h"

namespace crystal::gpu::impl::glan::code_gen {

class IMMEDIATE : public EXPR {
 public:
  IMMEDIATE(std::string VAL) : VAL_(VAL) {}
  ~IMMEDIATE() = default;
  operator std::string() const override { return VAL_; }

 private:
  std::string VAL_;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif