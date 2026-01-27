#ifndef CRYSTALGPU_IMPL_GLAN_RETURN_H_
#define CRYSTALGPU_IMPL_GLAN_RETURN_H_

#include <format>
#include <string>

#include "code_gen/return.h"
#include "semantic/expression.h"
#include "semantic/variable.h"
#include "variable.h"
#include "type.h"
#include "statement.h"

namespace crystal::gpu::impl::glan {

using std::format, std::string;

template <ANY_TYPE T>
class VAR<T, RET> : public code_gen::RET {
 public:
  using TYPE = T;
  /* Constructor */
  VAR() : code_gen::RET(T::KEYWORD) {}
  VAR(const VAR& OTHER) : VAR() { ASSIGN(*this, OTHER); }
  template <semantic::ANY_EXPR EXPR>
  VAR(const EXPR& OTHER) : VAR() {
    static_assert(std::is_same_v<typename EXPR::TYPE, T>,
                  "Assignment must be of the same type.");
    ASSIGN(*this, OTHER);
  }
  VAR(VAR&& OTHER) = delete;
  ~VAR() = default;

  /* Assignment */
  VAR& operator=(const VAR& RHS) {
    ASSIGN(*this, RHS);
    return *this;
  }
  VAR& operator=(VAR&& RHS) = delete;
  template <semantic::ANY_EXPR EXPR>
  VAR& operator=(const EXPR& RHS) {
    static_assert(std::is_same_v<typename EXPR::TYPE, T>,
                  "Assignment must be of the same type.");
    ASSIGN(*this, RHS);
    return *this;
  }

 private:
};
static_assert(semantic::ANY_VAR<VAR<TYPE::I32, RET>>);

} // namespace crystal::gpu::impl::glan

#endif