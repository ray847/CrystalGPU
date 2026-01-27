#ifndef CRYSTALGPU_IMPL_GLAN_FUNCTION_H_
#define CRYSTALGPU_IMPL_GLAN_FUNCTION_H_

#include <initializer_list>

#include "block.h"
#include "code_gen/function.h"
#include "expression.h"
#include "type.h"


namespace crystal::gpu::impl::glan {

template <ANY_TYPE T>
class FUNCTION {
 public:
  FUNCTION(BEGIN_BLOCK_RETURN) {
    END(); // end previous beginned block
    F_ = &code_gen::BEGIN_FUNCTION(T::KEYWORD);
  }
  FUNCTION(const FUNCTION&) = delete;
  FUNCTION(FUNCTION&&) = delete;
  FUNCTION& operator=(const FUNCTION&) = delete;
  FUNCTION& operator=(FUNCTION&&) = delete;

  template <typename... PARAMS>
  EXPR<T> operator()(const PARAMS&... P) const {
    string PARAM_STR{};
    (void)std::initializer_list<int>{ (
        PARAM_STR += format("{}, ", static_cast<string>(P)), 0)... };
    return EXPR<T>(format("{}({})", F_->SIGNATURE_.NAME_, PARAM_STR));
  }

 private:
  code_gen::FUNCTION* F_;
};

/* Specialization for void return. */
template <>
class FUNCTION<TYPE::VOID> {
 public:
  FUNCTION(BEGIN_BLOCK_RETURN) {
    END(); // end previous beginned block
    F_ = &code_gen::BEGIN_FUNCTION(TYPE::VOID::KEYWORD);
  }
  FUNCTION(const FUNCTION&) = delete;
  FUNCTION(FUNCTION&&) = delete;
  FUNCTION& operator=(const FUNCTION&) = delete;
  FUNCTION& operator=(FUNCTION&&) = delete;

  template <typename... PARAMS>
  void operator()(PARAMS... P) const {
    string PARAM_STR{};
    (void)std::initializer_list<int>{ (
        PARAM_STR += format("{}, ", static_cast<string>(P)), 0)... };
    EXPR<TYPE::VOID>(format("{}({})", F_->SIGNATURE_.NAME_, PARAM_STR));
  }

 private:
  code_gen::FUNCTION* F_;
};

} // namespace crystal::gpu::impl::glan

#endif