#ifndef CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_
#define CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_

#include <concepts>

#include "semantic/expression.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

template <ANY_TYPE T>
class EXPRESSION;

template <typename T>
concept ANY_EXPRESSION =
    semantic::ANY_EXPRESSION<T>
    && std::convertible_to<T, EXPRESSION<typename T::TYPE>>;

/* Scalar Expression */
template <ANY_TYPE T>
class EXPRESSION {
 public:
  using TYPE = T;
  bool CONSTANT_ = false;
  EXPRESSION(bool CONSTANT = false) : CONSTANT_(CONSTANT) {}
  /* Bool operators for const checking. */
  EXPRESSION operator&&(const EXPRESSION& OTHER) const {
    return CONSTANT_ && OTHER.CONSTANT_ ? EXPRESSION(true) : EXPRESSION(false);
  }
};
static_assert(ANY_EXPRESSION<EXPRESSION<INT32>>);

} // namespace crystal::gpu::impl::glan

#endif