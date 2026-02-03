#ifndef CRYSTALGPU_IMPL_GLAN_VARIABLE_H_
#define CRYSTALGPU_IMPL_GLAN_VARIABLE_H_

#include <format>

#include "code_gen/tuple.h"
#include "code_gen/variable.h"
#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::format;

template <ANY_TYPE T>
class VARIABLE : public code_gen::VARIABLE {
 public:
  using TYPE = T; // semantic static checking
  VARIABLE() : code_gen::VARIABLE(T::CODE_GEN_TYPE) {
    /* Check for tuple. */
    if constexpr (ANY_TUPLE<T>::value)
      PROCEDURE::DEFINE_TUPLE(*code_gen::IS_TUPLE(T::CODE_GEN_TYPE));
    /* Declaration statement. */
    PROCEDURE::PUSH(
        format("var {}: {}", this->USAGE_, T::CODE_GEN_TYPE.KEYWORD()));
  }
  VARIABLE(const VARIABLE& OTHER) :
      VARIABLE(static_cast<EXPRESSION<T>>(OTHER)) {
  }
  VARIABLE(EXPRESSION<T> OTHER) : code_gen::VARIABLE(T::CODE_GEN_TYPE) {
    /* Check for tuple. */
    if constexpr (ANY_TUPLE<T>::value)
      PROCEDURE::DEFINE_TUPLE(code_gen::IS_TUPLE(T::CODE_GEN_TYPE));
    /* Declaration statement. */
    PROCEDURE::PUSH(
        format("var {}: {}", this->USAGE_, T::CODE_GEN_TYPE.KEYWORD()));
    /* Assign statement. */
    PROCEDURE::PUSH([](auto STACK){
      STACK.PUSH(format("{1} = {0}", STACK.POP(), STACK.POP()));
    });
  }
  VARIABLE(VARIABLE&& OTHER) = delete;
  const VARIABLE& operator=(const VARIABLE& RHS) const {
    return *this = static_cast<EXPRESSION<T>>(RHS);
  }
  const VARIABLE& operator=(EXPRESSION<T>) const {
    PROCEDURE::PUSH(this->USAGE_);
    PROCEDURE::PUSH([](auto STACK){
      STACK.PUSH(format("{1} = {0}", STACK.POP(), STACK.POP()));
    });
    return *this;
  }
  VARIABLE& operator=(VARIABLE&& RHS) = delete;
  operator EXPRESSION<T>() const {
    PROCEDURE::PUSH(this->USAGE_);
    return {};
  }
};
static_assert(ANY_EXPRESSION<VARIABLE<INT32>>);

} // namespace crystal::gpu::impl::glan

#endif