#ifndef CRYSTALGPU_IMPL_GLAN_CODE_GEN_STATEMENT_H_
#define CRYSTALGPU_IMPL_GLAN_CODE_GEN_STATEMENT_H_

#include <string>

namespace crystal::gpu::impl::glan::code_gen {

class STATEMENT {
 public:
  virtual ~STATEMENT() = default;
  virtual operator std::string() const = 0;
};

} // namespace crystal::gpu::impl::glan::code_gen

#endif