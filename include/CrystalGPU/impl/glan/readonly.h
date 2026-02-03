#ifndef CRYSTALGPU_IMPL_GLAN_READONLY_H_
#define CRYSTALGPU_IMPL_GLAN_READONLY_H_

#include <format>

#include "code_gen/readonly.h"
#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::format;

template <ANY_TYPE T>
class READONLY : public code_gen::READONLY {
 public:
  using TYPE = T; // semantic static checking

  READONLY(const READONLY& OTHER) :
      READONLY(static_cast<EXPRESSION<T>>(OTHER)) {
  }
  template <ANY_EXPRESSION EXPR>
  requires std::is_same_v<typename EXPR::TYPE, T>
  READONLY(const EXPR& OTHER) : code_gen::READONLY(T::CODE_GEN_TYPE) {
    auto _ = static_cast<EXPRESSION<T>>(OTHER);
    /* Check for tuple. */
    if constexpr (ANY_TUPLE<T>::value)
      PROCEDURE::DEFINE_TUPLE(code_gen::IS_TUPLE(T::CODE_GEN_TYPE));
    /* Declaration part. */
    PROCEDURE::PUSH(
        format("let {}: {}", this->USAGE_, T::CODE_GEN_TYPE.KEYWORD()));
    /* Assignment part. */
    PROCEDURE::PUSH([](auto STACK) {
      STACK.PUSH(format("{1} = {0}", STACK.POP(), STACK.POP()));
    });
  }
  READONLY(READONLY&& OTHER) = delete;
  /* No assigning since it is read only. */
  READONLY& operator=(const READONLY&) = delete;
  READONLY& operator=(READONLY&&) = delete;
  /* Usage. */
  operator EXPRESSION<T>() const {
    PROCEDURE::PUSH(this->USAGE_);
    return {};
  }
};
static_assert(ANY_EXPRESSION<READONLY<INT32>>);

} // namespace crystal::gpu::impl::glan

#endif