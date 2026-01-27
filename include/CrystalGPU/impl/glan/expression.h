#ifndef CRSYTALGPU_IMPL_GLAN_EXPRESSION_H_
#define CRSYTALGPU_IMPL_GLAN_EXPRESSION_H_

#include <format>
#include <string>

#include "code_gen/expression.h"
#include "semantic/expression.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::string, std::format;

void STATE_EXPR(const code_gen::EXPR& EXPR);

template <ANY_TYPE T>
class EXPR : public code_gen::EXPR {
 public:
  using TYPE = T;
  EXPR(string STR) : STR_(STR) {}
  ~EXPR() override {
    if (!USED_) STATE_EXPR(*this);
  }
  operator string() const override {
    USED_ = true;
    return STR_;
  }

 private:
  mutable bool USED_ = false;
  string STR_;
};
static_assert(semantic::ANY_EXPR<EXPR<TYPE::I32>>);

} // namespace crystal::gpu::impl::glan

#endif