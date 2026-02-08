#ifndef CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_
#define CRYSTALGPU_IMPL_GLAN_EXPRESSION_H_

#include <array>
#include <format>
#include <initializer_list>
#include <string>

#include "DType.h"
#include "procedure.h"
#include "swizzle_DType.h"
#include "type.h"

namespace crystal::gpu::impl::glan {

using std::string, std::format, std::initializer_list, std::array;

template <AnyType Type, AnyDType DType>
class Expr {
 public:
  Expr() = default;
  Expr(const Expr&) = default;
  Expr(Expr&&) = delete;
  Expr& operator=(const Expr&) = delete;
  Expr& operator=(Expr&&) = delete;

  /* Behavior */
  /* Assignemnt */
  template <AnyType RType>
  requires Type::kLVal && RType::kRVal
  Expr operator=(Expr<RType, DType> rhs) {
    GlobalProc().Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
    return *this;
  }
  /* Subscript */
  auto operator[](size_t idx)
      requires DType::kSubscript && DType::kContainer::kIsContainer
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
      requires(DType::kSubscript == Subscript::Swizzle)
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

} // namespace crystal::gpu::impl::glan

#endif