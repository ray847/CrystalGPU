#ifndef CRYSTALGPU_IMPL_GLAN_FUNCTION_H_
#define CRYSTALGPU_IMPL_GLAN_FUNCTION_H_

#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

struct BLOCK_BEGIN_RETURN;

template <ANY_TYPE T>
class FUNCTION {
 public:
  template <typename U>
  requires std::is_same_v<U, BLOCK_BEGIN_RETURN>
  FUNCTION(U) {
    KEY_ = PROCEDURE::BEGIN_FUNCTION<T>();
  }
  EXPRESSION<T> operator()() const {
    /* Push a function call atomic. */
    return {};
  }

 private:
  size_t KEY_; // key to the function in the current procedure.
};
} // namespace crystal::gpu::impl::glan

#endif