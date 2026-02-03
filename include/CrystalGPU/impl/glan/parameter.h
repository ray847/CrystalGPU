#ifndef CRYSTALGPU_IMPL_GLAN_PARAMETER_H_
#define CRYSTALGPU_IMPL_GLAN_PARAMETER_H_

#include <format>

#include "code_gen/tuple.h"
#include "code_gen/variable.h"
#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::format;

template <ANY_TYPE T>
class PARAMETER : public code_gen::VARIABLE {
 public:
  using TYPE = T; // semantic static checking
  PARAMETER() : code_gen::VARIABLE(T::CODE_GEN_TYPE) {
    /* Check for tuple. */
    if constexpr (ANY_TUPLE<T>::value)
      PROCEDURE::DEFINE_TUPLE(*code_gen::IS_TUPLE(T::CODE_GEN_TYPE));
    PROCEDURE::ADD_PARAM(*this);
  }
  PARAMETER(const PARAMETER& OTHER) = delete;
  PARAMETER(PARAMETER&& OTHER) = delete;
  const PARAMETER& operator=(const PARAMETER& RHS) const {
    return *this = static_cast<EXPRESSION<T>>(RHS);
  }
  const PARAMETER& operator=(EXPRESSION<T>) const {
    PROCEDURE::PUSH(this->USAGE_);
    PROCEDURE::PUSH([](auto STACK){
      STACK.PUSH(format("{1} = {0}", STACK.POP(), STACK.POP()));
    });
    return *this;
  }
  PARAMETER& operator=(PARAMETER&& RHS) = delete;
  operator EXPRESSION<T>() const {
    PROCEDURE::PUSH(this->USAGE_);
    return {};
  }
};
static_assert(ANY_EXPRESSION<PARAMETER<INT32>>);

} // namespace crystal::gpu::impl::glan

#endif