#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_EXPRESSION_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_EXPRESSION_H_

#include <string>

namespace crystal::gpu::impl::glan::code_gen {

class EXPR {
 public:
  virtual ~EXPR() = default;
  virtual operator std::string() const = 0;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif