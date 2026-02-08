#ifndef CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_
#define CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_

#include <array>
#include <format>
#include <initializer_list>
#include <string>
#include <type_traits>

#include "dtype.h"
#include "procedure.h"
#include "swizzle_dtype.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::string, std::format, std::initializer_list, std::array,
    std::is_convertible_v;

/* AnyExpr Concept */
template <AnyType Type, AnyDType DType>
class Expr;
template <typename T>
concept AnyExpr =
    requires {
      typename T::Type;
      typename T::DType;
    } && AnyType<typename T::Type> && AnyDType<typename T::DType>
    && is_convertible_v<T, Expr<typename T::Type, typename T::DType>>;

/* Expr Class */
template <AnyType ExprType, AnyDType ExprDType>
class Expr {
 public:
  using Type = ExprType;
  using DType = ExprDType;

  Expr() = default;
  Expr(const Expr&) = delete;
  Expr(Expr&&) = default;
  Expr& operator=(const Expr&) = delete;
  Expr& operator=(Expr&&) = delete;

  /* Behavior */
  /* Assignemnt */
  Expr& operator=(Expr&& rhs) requires Type::kLVal && Type::kRVal
  {
    GlobalProc().Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
    return *this;
  }
  template <AnyType RType>
  requires Type::kLVal && RType::kRVal
  Expr& operator=(Expr<RType, DType>&& rhs) {
    GlobalProc().Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
    return *this;
  }
  /* Subscript */
  auto operator[](size_t idx) requires(DType::kSubscript != Subscript::None)
                                   && DType::kContainer::kIsContainer
  {
    GlobalProc().Push([=](auto stk) {
      auto obj = stk.Pop();
      stk.Push(format("{}[{}]", obj, idx));
    });
    return Expr<Type, typename DType::kContainer::kInner>{};
  }
  /* Swizzle */
  template <typename... Indicies>
  auto operator[](Indicies... indicies)
      requires(sizeof...(Indicies) > 1)
           && (DType::kSubscript == Subscript::Swizzle)
           && DType::kContainer::kIsContainer
  {
    constexpr array<char, 4> kComponents = { 'x', 'y', 'z', 'w' };
    string swizzle_str;
    initializer_list<int>{ (swizzle_str += kComponents[indicies], 0)... };
    GlobalProc().Push([=](auto stk) {
      auto obj = stk.Pop();
      stk.Push(format("{}.{}", obj, swizzle_str));
    });
    return Expr<Type,
                Swizzle<sizeof...(Indicies),
                        typename DType::kContainer::kInner>>{};
  }
};
static_assert(AnyExpr<Expr<Type<>, DType<>>>);

} // namespace crystal::gpu::impl::glan

#endif