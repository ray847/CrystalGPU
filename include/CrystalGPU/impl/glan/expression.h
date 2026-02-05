#ifndef CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_
#define CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_

#include "type.h"

namespace crystal::gpu::impl::glan {

template <typename T>
concept AnyExpr = requires {
  typename T::Type;
} && AnyType<typename T::TYPE>;

template <AnyType Type>
struct Expr {
  bool comp_time_const = false;
  Expr(bool comp_time_const) : comp_time_const(comp_time_const) {
  }
  Expr(const Expr& other) = default;
  Expr(Expr&& other) = delete;
  Expr& operator=(const Expr& rhs) = default;
  Expr& operator=(Expr&& rhs) = delete;

};

/* Logical operator for const arithmetic. */
template <AnyType Type>
Expr<Type> operator&&(const Expr<Type>& L, const Expr<Type>& R) {
  return {L.comp_time_const && R.comp_time_const};
}

} // namespace crystal::gpu::impl::glan

#endif