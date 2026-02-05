#ifndef CRYSTALGPU_IMPL_GLAN_PROXY_H_
#define CRYSTALGPU_IMPL_GLAN_PROXY_H_

#include <array>
#include <cwchar>
#include <initializer_list>

#include "expression.h"
#include "policy.h"
#include "procedure.h"
#include "storage.h"
#include "type.h"
#include "utility.h"

namespace crystal::gpu::impl::glan {

using std::array, std::initializer_list;

inline static Proc* proxy_proc = nullptr;
/**
 * Bind to the input procedure.
 *
 * After binding to a procedure, all symbol used will be tracked in the symbol
 * table of the bound procedure.
 *
 * @param P: A reference to the procedure.
 */
inline void ProxyBind(Proc& P) {
  proxy_proc = &P;
}
/**
 * Unbind the procedure.
 */
inline void ProxyUnbind() {
  proxy_proc = nullptr;
}

template <AnyType Type, Storage storage, AnyPolicy policy>
class Proxy {
 public:
  /* Definition */
  Proxy() :
      sym_({ .name = GenName("sym"),
             .type = Type{},
             .storage = storage,
             .behavior = policy{} }) {
    /* Append symbol to symbol table with metadata. */
    proxy_proc->SymbolTable().Insert(sym_);
    /* Local Definition */
    if constexpr (policy::kDefLoc == DefLoc::Local) proxy_proc->PushDef(sym_);
  }
  /* Initialization */
  Proxy(const Proxy& rhs) : Proxy(static_cast<Expr<Type>>(rhs)) {
  }
  template <Storage other_storage, AnyPolicy other_policy>
  Proxy(const Proxy<Type, other_storage, other_policy>& rhs) :
      Proxy(static_cast<Expr<Type>>(rhs)) {
  }
  Proxy(const Expr<Type>& rhs) : Proxy() {
    /* Check expression comp-time-constness if this is comp-time-const. */
    if constexpr (policy::kCompTimeConst) assert(rhs.comp_time_const);
    /* Push lhs if not local definition. */
    if constexpr (policy::kDefLoc != DefLoc::Local) proxy_proc->Push(sym_.name);
    /* Initialization is not constrained by policy::kAssignable. */
    proxy_proc->Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
  }
  Proxy(Proxy&& other) = delete;
  Proxy& operator=(Proxy&& rhs) = delete;

  /* Conversion to Expr */
  operator Expr<Type>() const {
    /* Procedure Operation */
    proxy_proc->Push(sym_.name);
    return Expr<Type>{ policy::kCompTimeConst };
  }

  /* Policy Implementation */
  /* Assignable */
  const Proxy& operator=(const Proxy& rhs) const requires policy::kAssignable
  {
    /* Procedure Operation */
    auto _rhs = static_cast<Expr<Type>>(rhs); // push rhs
    auto _lhs = static_cast<Expr<Type>>(*this); // push lhs (this)
    proxy_proc->Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
    return *this;
  }
  template <Storage other_storage, AnyPolicy other_policy>
  requires policy::kAssignable
  const Proxy& operator=(
      const Proxy<Type, other_storage, other_policy>& rhs) const {
    /* Procedure Operation */
    auto _rhs = static_cast<Expr<Type>>(rhs); // push rhs
    auto _lhs = static_cast<Expr<Type>>(*this); // push lhs (this)
    proxy_proc->Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
    return *this;
  }
  const Proxy& operator=(const Expr<Type>& rhs) const
      requires policy::kAssignable
  {
    /* Procedure Operation */
    auto __ = static_cast<Expr<Type>>(*this); // push lhs (this)
    proxy_proc->Push([](auto stk) {
      auto lhs = stk.Pop();
      auto rhs = stk.Pop();
      stk.Push(format("{} = {}", lhs, rhs));
    });
    return *this;
  }
  /* Subscriptable */
  template <bool _ = false>  // unused
  requires policy::kSubscriptable && (AnyVector<Type> || AnyMatrix<Type>)
  auto operator[](size_t idx) const {
    /* Procedure Operation */
    auto __ = static_cast<Expr<Type>>(*this);  // push symbol name
    proxy_proc->Push([=](auto stk) {
      stk.Push(format("{}[{}]", stk.Pop(), idx));
    });  // push subscript operator
    return Expr<typename Type::DType>{ policy::kCompTimeConst };
  }
  /* Vector Swizzle */
  template <typename... Indicies>
  requires policy::kSwizzle
        && AnyVector<Type> && (std::same_as<Indicies, size_t> && ...)
  auto operator[](Indicies... indicies) {
    /* Procedure Operation */
    auto _ = static_cast<Expr<Type>>(*this);  // push symbol name
    proxy_proc->Push([indicies...](auto stk) {
      static constexpr array<char, 4> kLabels = { 'x', 'y', 'z', 'w' };
      string swizzle_idx;
      initializer_list<int>{ (swizzle_idx += kLabels[indicies], 0)... };
      stk.Push(format("{}.{}", stk.Pop(), swizzle_idx));
    });  // push swizzle
    return Expr<Vec<sizeof...(Indicies), typename Type::DType>>{
      policy::kCompTimeConst
    };
  }
  /* Matrix Swizzle */
  template <typename... Indicies>
  requires policy::kSwizzle
        && AnyMatrix<Type> && (std::same_as<Indicies, size_t> && ...)
  auto operator[]() {
    static_assert(false, "Matrix swizzle not supported.");
    return Expr<Mat<sizeof...(Indicies), Type::N, typename Type::DType>>{
      policy::kCompTimeConst
    };
  }

 private:
  /* Member Variable */
  Symbol sym_;
};

}  // namespace crystal::gpu::impl::glan

#endif