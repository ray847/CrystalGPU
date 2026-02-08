#ifndef CRYSTALGPU_IMPL_GLAN_OPERATOR_H_
#define CRYSTALGPU_IMPL_GLAN_OPERATOR_H_

#include <format>
#include <type_traits>

#include "dtype.h"
#include "expression.h"
#include "procedure.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::format, std::is_same_v;

template <AnyExpr L, AnyExpr R>
requires is_same_v<typename L::DType, typename R::DType>
auto operator+(L& l, R& r) {
  using DType = L::DType;
  auto l_expr = static_cast<Expr<typename L::Type, DType>>(l);
  auto r_expr = static_cast<Expr<typename R::Type, DType>>(r);
  GlobalProc().Push([](auto stk) {
    auto r = stk.Pop();
    auto l = stk.Pop();
    stk.Push(format("({} + {})", l, r));
  });
  return Expr<Intercect<typename L::Type, typename R::Type>, DType>{};
}

template <AnyExpr L, AnyExpr R>
requires is_same_v<typename L::DType, typename R::DType>
auto operator-(L& l, R& r) {
  using DType = L::DType;
  auto l_expr = static_cast<Expr<typename L::Type, DType>>(l);
  auto r_expr = static_cast<Expr<typename R::Type, DType>>(r);
  GlobalProc().Push([](auto stk) {
    auto r = stk.Pop();
    auto l = stk.Pop();
    stk.Push(format("({} - {})", l, r));
  });
  return Expr<Intercect<typename L::Type, typename R::Type>, DType>{};
}

} // namespace crystal::gpu::impl::glan

#endif