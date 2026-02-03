#ifndef CRYSTALGPU_IMPL_GLAN_FUNCTION_H_
#define CRYSTALGPU_IMPL_GLAN_FUNCTION_H_

#include <initializer_list>

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
  template <typename... ARGUMENTS>
  EXPRESSION<T> operator()(const ARGUMENTS&... ARGS) const {
    assert(sizeof...(ARGUMENTS)
           == PROCEDURE::GET_FUNCTION(KEY_)->SIGNATURE().PARAMS_.size());
    std::initializer_list<int> _{ (EVAL_ARG(ARGS), 0)... };
    //EVAL_ARGS(ARGS...);
    PROCEDURE::PUSH([&](auto STACK) {
      string ARG_LIST;
      if (PROCEDURE::GET_FUNCTION(KEY_)->SIGNATURE().PARAMS_.size()) {
        ARG_LIST = STACK.POP();
        for (int I = 1;
             I < PROCEDURE::GET_FUNCTION(KEY_)->SIGNATURE().PARAMS_.size();
             ++I) {
          ARG_LIST += ", " + STACK.POP();
        }
      }
      /* Push a function call atomic. */
      STACK.PUSH(format("{}({})",
                        PROCEDURE::GET_FUNCTION(KEY_)->SIGNATURE().NAME_,
                        ARG_LIST));
    });
    return {};
  }

 private:
  size_t KEY_; // key to the function in the current procedure.

  template <ANY_EXPRESSION EXPR>
  void EVAL_ARG(const EXPR& E) const {
    auto _ = static_cast<EXPRESSION<typename EXPR::TYPE>>(E);
  }
};
} // namespace crystal::gpu::impl::glan

#endif