#ifndef CRYSTALGPU_IMPL_GLAN_CONSTANT_H_
#define CRYSTALGPU_IMPL_GLAN_CONSTANT_H_

#include <cassert>

#include <format>
#include <string>

#include "code_gen/constant.h"
#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::format, std::string;

template <ANY_TYPE T>
class CONSTANT : public code_gen::CONSTANT {
 public:
  using TYPE = T; // semantic static checking

  CONSTANT(const CONSTANT& OTHER) :
      CONSTANT(static_cast<EXPRESSION<T>>(OTHER)) {
  }
  template <ANY_EXPRESSION EXPR>
  requires std::is_same_v<typename EXPR::TYPE, T>
  CONSTANT(const EXPR& OTHER) : code_gen::CONSTANT(T::CODE_GEN_TYPE) {
    /* Check for tuple. */
    if constexpr (ANY_TUPLE<T>::value)
      PROCEDURE::DEFINE_TUPLE(code_gen::IS_TUPLE(T::CODE_GEN_TYPE));
    auto E = static_cast<EXPRESSION<T>>(OTHER);
    assert(E.CONSTANT_
           && "Glan constants must be initialized with constant expressions.");
    /* Declaration */
    PROCEDURE::PUSH(
        format("const {}: {}", this->USAGE_, T::CODE_GEN_TYPE.KEYWORD()));
    /* Assignment */
    PROCEDURE::PUSH([](const string& RHS, const string& LHS) -> string {
      return format("{} = {}", LHS, RHS);
    });
  }
  CONSTANT(CONSTANT&& OTHER) = delete;
  /* No assigning since it is constant. */
  CONSTANT& operator=(const CONSTANT&) = delete;
  CONSTANT& operator=(CONSTANT&&) = delete;
  /* Usage */
  operator EXPRESSION<T>() const {
    PROCEDURE::PUSH(this->USAGE_);
    return EXPRESSION<T>(true); // constant expression
  }
};
} // namespace crystal::gpu::impl::glan

#endif